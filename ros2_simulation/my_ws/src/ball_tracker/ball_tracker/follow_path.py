# Copyright 2023 Josh Newans
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import numpy as np
import rclpy
import time
import math
from rclpy.node import Node
from geometry_msgs.msg import Point
from geometry_msgs.msg import Twist
from sensor_msgs.msg import Range
from sensor_msgs.msg  import Image
from std_srvs.srv import Empty
from std_msgs.msg import String
from std_msgs.msg import Float64

from sensor_msgs.msg import JointState

class FollowPath(Node):

    def __init__(self):
        super().__init__('follow_path')
        self.subscription_path = self.create_subscription(
            Point,
            '/detected_path',
            self.center_path_callback,
            20)
        self.subscription_sensor_distance = self.create_subscription(
            Range,
            '/ultrasonic_sensor_1/out',
            self.detected_object_callback,
            20)
        self.subscription_ball = self.create_subscription(
            Point,
            '/detected_ball',
            self.detected_ball_callback,
            20)
        # listen to "/joint_states" topic to get the current velocity of the car
        self.subscription_joint_states = self.create_subscription(
            JointState,
            '/joint_states',
            self.joint_states_callback,
            20)
        self.start_service = self.create_service(Empty, 'start_follower', self.start_follower_callback)
        self.stop_service = self.create_service(Empty, 'stop_follower', self.stop_follower_callback)
        self.reset_progress = self.create_service(Empty, 'reset_progress', self.reset_progress_callback)
        self.publisher_ = self.create_publisher(Twist, '/cmd_vel', 10)
        self.object_detected_publisher = self.create_publisher(Range, '/object_detected', 1)
        self.object_removed_publisher = self.create_publisher(Range, '/object_removed', 1)
        self.current_velocity_publisher = self.create_publisher(Float64, '/current_velocity', 1)
        self.current_distance_publisher = self.create_publisher(Float64, '/current_distance', 1)

        self.declare_parameter("rcv_timeout_secs", 1.0)
        self.declare_parameter("angular_chase_multiplier", 0.7)
        self.declare_parameter("forward_chase_speed", 0.1)
        self.declare_parameter("search_angular_speed", 0.5)
        self.declare_parameter("min_distance", 0.9)
        self.declare_parameter("filter_value", 0.9)
        self.declare_parameter("h_fov",1.089)
        self.declare_parameter("ball_radius",0.06)


        self.rcv_timeout_secs = self.get_parameter('rcv_timeout_secs').get_parameter_value().double_value
        self.angular_chase_multiplier = self.get_parameter('angular_chase_multiplier').get_parameter_value().double_value
        self.forward_chase_speed = self.get_parameter('forward_chase_speed').get_parameter_value().double_value
        self.search_angular_speed = self.get_parameter('search_angular_speed').get_parameter_value().double_value
        self.min_distance = self.get_parameter('min_distance').get_parameter_value().double_value
        self.filter_value = self.get_parameter('filter_value').get_parameter_value().double_value
        self.h_fov = self.get_parameter('h_fov').get_parameter_value().double_value
        self.ball_radius = self.get_parameter('ball_radius').get_parameter_value().double_value

        self.center_x = 0.0
        self.center_y = 0.0
        self.position_car = 320.0 # The center of the car is at 320 pixels

        timer_period = 0.1  # seconds
        self.timer = self.create_timer(timer_period, self.timer_callback)
        self.target_val = 0.0
        self.target_size = 0.0
        self.lastrcvtime = time.time() - 10000
        self.wait_time = 0
        self.current_time = time.time()

        self.is_object_detected = False
        self.should_move = False
        self.wheel_radius = 0.033
        self.start_distance = 0.0

    def timer_callback(self):
        twist_cmd = Twist()

        if self.is_object_detected or self.should_move == False:
            twist_cmd.linear.x = 0.0
            self.publisher_.publish(twist_cmd)
            return

        steering_angle_history = []
        history_length = 10  # Define the length of the history for the moving average filter
        
        if (time.time() - self.lastrcvtime < self.rcv_timeout_secs): # if we have received a ball in the last second
            if (time.time() - self.current_time < self.wait_time): 
                print("Waiting before searching again for a ball")
            elif (self.ball_distance < self.min_distance):
                print("Stopping")
                # Publish the steering command to the car
                twist_cmd.linear.x = 0.0
                twist_cmd.angular.z = 0.0
                self.publisher_.publish(twist_cmd)

                time.sleep(3) # stop for 3 seconds
                self.wait_time = 3
                self.current_time = time.time()

        # check if the wait time has expired
        if (time.time() - self.current_time > self.wait_time):
            self.wait_time = 0


        # print("Center: ", self.center_x, " Center Y: ", self.center_y)
        if not (self.center_x == 0.0 and self.center_y == 0.0):

            # Calculate the desired steering angle based on the curve and car position
            steering_angle = 1.1*((self.center_x - self.position_car) / self.position_car)

            # Apply a moving average filter to smooth the steering angle
            steering_angle_history.append(steering_angle)
            if len(steering_angle_history) > history_length:
                steering_angle_history = steering_angle_history[-history_length:]
            smoothed_steering_angle = np.mean(steering_angle_history)

            # Control the car based on the calculated steering angle
            twist_cmd = Twist()
            twist_cmd.linear.x = 0.4  # Set the linear speed
            twist_cmd.angular.z = -smoothed_steering_angle  # Set the steering angle (negative sign if required)

            # Publish the steering command to the car
            self.publisher_.publish(twist_cmd)
        else:
            # No path found, set steering angle to 0
            steering_angle = 0.0

    def center_path_callback(self, msg):
        self.center_x = msg.x
        self.center_y = msg.z

    def detected_ball_callback(self, data : Point):
        ang_size = data.z*self.h_fov
        self.ball_distance = self.ball_radius/(math.atan(ang_size/2))

        self.lastrcvtime = time.time()

    def detected_object_callback(self, data : Range):
        if (data.range < 1 and not self.is_object_detected):
            print("Object detected")
            self.is_object_detected = True
            self.object_detected_publisher.publish(data)
        elif (data.range >= 1 and self.is_object_detected):
            self.is_object_detected = False
            self.object_removed_publisher.publish(data)

    def start_follower_callback(self, request, response):
        print("Starting follower")
        self.should_move = True
        return response
    
    def stop_follower_callback(self, request, response):
        print("Stopping follower")
        self.should_move = False
        return response
    
    def reset_progress_callback(self, request, response):
        print("Resetting progress")
        self.start_distance = 0.0
        self.should_move = False
        return response
    
    def joint_states_callback(self, data):
        current_velocity = (data.velocity[0] + data.velocity[1])/2
        current_velocity = current_velocity * self.wheel_radius

        current_distance = (data.position[0] + data.position[1])/2
        current_distance = current_distance - self.start_distance
        
        if self.start_distance == 0:
            self.start_distance = current_distance

        if (current_velocity < 0.01):
            return
    
        msg_velocity = Float64()
        msg_velocity.data = current_velocity
        self.current_velocity_publisher.publish(msg_velocity)

        msg_distance = Float64()
        msg_distance.data = (current_distance // self.wheel_radius) / 1000
        self.current_distance_publisher.publish(msg_distance)


def main(args=None):
    rclpy.init(args=args)
    follow_path = FollowPath()
    rclpy.spin(follow_path)
    follow_path.destroy_node()
    rclpy.shutdown()
