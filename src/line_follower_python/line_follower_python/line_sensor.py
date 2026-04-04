import rclpy
from rclpy.node import Node

import random

from example_interfaces.msg import Int8
from example_interfaces.msg import Bool


class LineSensor(Node):
    def __init__(self):
        super().__init__("line_sensor")
        self.center_pos_ = 7
        self.line_detected_ = False
        self.abs_pos_ = 0
        self.pos_error_ = self.abs_pos_ - self.center_pos_

        self.line_pos_publisher_ = self.create_publisher(Int8, "line_pos", 10)
        self.line_error_publisher_ = self.create_publisher(Int8, "line_error", 10)
        self.line_detect_publisher_ = self.create_publisher(Bool, "line_detect", 10)

        self.create_timer(
            1 / 500,
            lambda: (
                self.publishEvents(),
                self.readLine(),
                self.calcError(),
                self.isThereALine(),
            ),
        )

        self.get_logger().info("Line Sensor Node Started")

    def readLine(self):
        # sensor read logic
        self.abs_pos_ = random.randint(0, 15)

    def calcError(self):
        self.pos_error_ = self.abs_pos_ - self.center_pos_

    def isThereALine(self):
        self.line_detected_ = True

    def publishEvents(self):

        msg_line_detected = Bool()
        msg_line_detected.data = self.line_detected_

        msg_abs_pos = Int8()
        msg_abs_pos.data = self.abs_pos_

        msg_pos_error = Int8()
        msg_pos_error.data = self.pos_error_

        self.line_detect_publisher_.publish(msg_line_detected)
        self.line_pos_publisher_.publish(msg_abs_pos)
        self.line_error_publisher_.publish(msg_pos_error)

        self.get_logger().info("Published!")


def main(args=None):
    rclpy.init(args=args)
    node = LineSensor()
    rclpy.spin(node)
    rclpy.shutdown()


if __name__ == "__main__":
    main()
