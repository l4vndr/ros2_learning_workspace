import rclpy
from rclpy.node import Node
from example_interfaces.msg import Int64


class NumberPublisher(Node):
    def __init__(self):
        super().__init__("number_publisher")
        self.num_publisher_ = self.create_publisher(Int64, "number", 10)
        self.timer_ = self.create_timer(5.0, self.publishNumber)
        self.get_logger().info("Number Publisher Node Started!")

    def publishNumber(self):
        msg = Int64()
        msg.data = 69
        self.num_publisher_.publish(msg)
        self.get_logger().info("Number Published!")


def main(args=None):
    rclpy.init(args=args)
    node = NumberPublisher()
    rclpy.spin(node)
    rclpy.shutdown()


if __name__ == "__main__":
    main()
