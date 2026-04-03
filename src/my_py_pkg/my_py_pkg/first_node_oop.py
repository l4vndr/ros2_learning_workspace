#!/usr/bin/env python3
import rclpy
from rclpy.node import Node


class PY_NODE(Node):

    def __init__(self):
        super().__init__("py_test")
        self.counter_ = 0
        self.get_logger().info("Hello World")
        self.create_timer(1.0, self.timer_callback)

    def timer_callback(self):
        self.get_logger().info("HELLO " + str(self.counter_))
        self.counter_ += 1


def main(args=None):
    rclpy.init(args=args)
    node = PY_NODE()
    rclpy.spin(node)
    rclpy.shutdown()


if __name__ == "__main__":
    main()


# import rclpy
# from rclpy.node import Node


# class MyNode(Node):
#     def __init__(self):
#         super().__init__("node")
#         self.get_logger().info("Hello World")
#         self.create_timer(1.0, self.timer_callback)

#     def timer_callback(self):
#         self.get_logger().info("Hello")


# def main(args=None):
#     rclpy.init(args=args)
#     node = MyNode()
#     rclpy.spin(node)
#     rclpy.shutdown()


# if __name__ == "__main__":
#     main()
