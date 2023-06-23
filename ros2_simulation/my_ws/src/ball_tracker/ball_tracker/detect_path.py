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

import rclpy
import cv2
from rclpy.node import Node
from sensor_msgs.msg        import Image
from geometry_msgs.msg      import Point
from cv_bridge              import CvBridge, CvBridgeError

import ball_tracker.process_image_path as proc

class DetectPath(Node):

    def __init__(self):
        super().__init__('detect_stop_sign')

        self.get_logger().info('Looking for the path...')
        self.image_sub = self.create_subscription(Image,"/image_in",self.callback,rclpy.qos.QoSPresetProfiles.SENSOR_DATA.value)
        self.image_out_pub = self.create_publisher(Image, "/image_out_path", 1)
        self.image_tuning_pub = self.create_publisher(Image, "/image_tuning_path", 1)
        self.path_pub  = self.create_publisher(Point,"/detected_path",1)

        self.declare_parameter('tuning_mode', False)

        self.declare_parameter("x_min",0)
        self.declare_parameter("x_max",100)
        self.declare_parameter("y_min",0)
        self.declare_parameter("y_max",100)
        self.declare_parameter("h_min",0)
        self.declare_parameter("h_max",180)
        self.declare_parameter("s_min",0)
        self.declare_parameter("s_max",255)
        self.declare_parameter("v_min",0)
        self.declare_parameter("v_max",255)
        self.declare_parameter("sz_min",0)
        self.declare_parameter("sz_max",100)
        
        self.tuning_mode = self.get_parameter('tuning_mode').get_parameter_value().bool_value
        self.tuning_params = {
            'x_min': self.get_parameter('x_min').get_parameter_value().integer_value,
            'x_max': self.get_parameter('x_max').get_parameter_value().integer_value,
            'y_min': self.get_parameter('y_min').get_parameter_value().integer_value,
            'y_max': self.get_parameter('y_max').get_parameter_value().integer_value,
            'h_min': self.get_parameter('h_min').get_parameter_value().integer_value,
            'h_max': self.get_parameter('h_max').get_parameter_value().integer_value,
            's_min': self.get_parameter('s_min').get_parameter_value().integer_value,
            's_max': self.get_parameter('s_max').get_parameter_value().integer_value,
            'v_min': self.get_parameter('v_min').get_parameter_value().integer_value,
            'v_max': self.get_parameter('v_max').get_parameter_value().integer_value,
            'sz_min': self.get_parameter('sz_min').get_parameter_value().integer_value,
            'sz_max': self.get_parameter('sz_max').get_parameter_value().integer_value
        }

        # temporary
        self.tuning_params = {
            'x_min': 0,
            'x_max': 100,
            'y_min': 53,
            'y_max': 100,
            'h_min': 26,
            'h_max': 59,
            's_min': 95,
            's_max': 255,
            'v_min': 0,
            'v_max': 255,
            'sz_min': 0,
            'sz_max': 19
        }

        self.bridge = CvBridge()

        if(self.tuning_mode):
            proc.create_tuning_window(self.tuning_params)

    def callback(self,data):
        try:
            cv_image = self.bridge.imgmsg_to_cv2(data, "bgr8")
        except CvBridgeError as e:
            print(e)

        try:
            if (self.tuning_mode):
                self.tuning_params = proc.get_tuning_params()

            keypoints_norm, out_image, tuning_image = proc.find_path(cv_image, self.tuning_params)

            

            # Convert the image to black and white
            gray_image = cv2.cvtColor(out_image, cv2.COLOR_BGR2GRAY)

            contours, _ = cv2.findContours(gray_image, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

            # show the contours (outline) of the shapes in the image
            cv2.drawContours(out_image, contours, -1, (0, 255, 0), 2)

            point_out = Point()
            
            curve_center_x = 0
            curve_center_y = 0
            
            if len(contours) > 0:
                # Sort contours based on area (descending order)
                contours = sorted(contours, key=cv2.contourArea, reverse=True)

                # Calculate the middle point of the largest contour
                M = cv2.moments(contours[0])
                if M["m00"] != 0:
                    curve_center_x = int(M["m10"] / M["m00"])
                    curve_center_y = int(M["m01"] / M["m00"])
                    cv2.circle(out_image, (curve_center_x, curve_center_y), 5, (255, 0, 0), -1)

                point_out.x = float(curve_center_x)
                point_out.y = float(curve_center_y)
                point_out.z = float(0)

                self.path_pub.publish(point_out)
            else:
                point_out.x = float(0)
                point_out.y = float(0)
                point_out.z = float(0)

                self.path_pub.publish(point_out)

            img_to_pub = self.bridge.cv2_to_imgmsg(out_image, "bgr8")
            img_to_pub.header = data.header
            self.image_out_pub.publish(img_to_pub)

            img_to_pub = self.bridge.cv2_to_imgmsg(tuning_image, "bgr8")
            img_to_pub.header = data.header
            self.image_tuning_pub.publish(img_to_pub)
        except CvBridgeError as e:
            print(e)  


def main(args=None):

    rclpy.init(args=args)

    detect_path = DetectPath()
    while rclpy.ok():
        rclpy.spin_once(detect_path)
        proc.wait_on_gui()

    detect_path.destroy_node()
    rclpy.shutdown()

