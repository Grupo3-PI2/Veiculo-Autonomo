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
from sensor_msgs.msg  import Image

class FollowBall(Node):

    def __init__(self):
        super().__init__('follow_ball')
        self.subscription = self.create_subscription(
            Point,
            '/detected_path',
            self.center_path_callback,
            10)
        self.subscription = self.create_subscription(
            Point,
            '/detected_ball',
            self.detected_ball_callback,
            10)
        
        self.publisher_ = self.create_publisher(Twist, '/cmd_vel', 10)

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

    def timer_callback(self):
        twist_cmd = Twist()

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
            steering_angle = (self.center_x - self.position_car) / self.position_car

            # Apply a moving average filter to smooth the steering angle
            steering_angle_history.append(steering_angle)
            if len(steering_angle_history) > history_length:
                steering_angle_history = steering_angle_history[-history_length:]
            smoothed_steering_angle = np.mean(steering_angle_history)

            # Control the car based on the calculated steering angle
            twist_cmd = Twist()
            twist_cmd.linear.x = 0.5  # Set the linear speed
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
        print("Ball distance from car: ", self.ball_distance);

        self.lastrcvtime = time.time()

def main(args=None):
    rclpy.init(args=args)
    follow_ball = FollowBall()
    rclpy.spin(follow_ball)
    follow_ball.destroy_node()
    rclpy.shutdown()
