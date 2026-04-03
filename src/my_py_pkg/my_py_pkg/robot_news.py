import rclpy
from rclpy.node import Node
from example_interfaces.msg import String


class RoboNews(Node):
    def __init__(self):
        super().__init__("robot_news")
        self.publisher_ = self.create_publisher(String, "robot_news", 10)
        self.timer_ = self.create_timer(0.5, self.publish_news)
        self.get_logger().info("Robo News Started")

    def publish_news(self):
        msg = String()
        msg.data = "Hello World"
        self.publisher_.publish(msg)


def main(args=None):
    rclpy.init(args=args)
    node = RoboNews()
    rclpy.spin(node)
    rclpy.shutdown


if __name__ == "__main__":
    main()
