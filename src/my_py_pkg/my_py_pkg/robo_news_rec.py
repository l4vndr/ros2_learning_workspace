import rclpy
from rclpy.node import Node
from example_interfaces.msg import String


class RoboRec(Node):
    def __init__(self):
        super().__init__("robo_news_rec")
        self.subscriber_ = self.create_subscription(
            String, "robot_news", self.newsRecCallback, 10
        )
        self.get_logger().info("News Receiver Started")

    def newsRecCallback(self, msg: String):
        self.get_logger().info(msg.data)


def main(args=None):
    rclpy.init(args=args)
    node = RoboRec()
    rclpy.spin(node)
    rclpy.shutdown()


if __name__ == "__main__":
    main()
