import RPi.GPIO as GPIO
from time import sleep
import rclpy
from rclpy.node import Node

from std_msgs.msg import String



GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
Ena, In1, In2 = 22, 13, 19
GPIO.setup(Ena, GPIO.OUT)
GPIO.setup(In1, GPIO.OUT)
GPIO.setup(In2, GPIO.OUT)
pwm = GPIO.PWM(Ena, 100)
pwm.start(0)

speed = 50;

while True:
    GPIO.output(In1,GPIO.LOW)
    GPIO.output(In2,GPIO.HIGH)
    if speed > 100 :
        speed = 100
    pwm.ChangeDutyCycle(speed)
# Copyright 2016 Open Source Robotics Foundation, Inc.
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

class MinimalSubscriber(Node):

    def __init__(self):
        super().__init__('minimal_subscriber')
        self.subscription = self.create_subscription(
            String,
            'topic',
            self.listener_callback,
            10)
        self.subscription  # prevent unused variable warning

    def listener_callback(self, msg):
        self.get_logger().info('I heard: "%s"' % msg.data)
        print('here is the speed: ': int(msg.data))
        speed = int(msg.data)



def main(args=None):
    rclpy.init(args=args)

    minimal_subscriber = MinimalSubscriber()

    rclpy.spin(minimal_subscriber)

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    minimal_subscriber.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
