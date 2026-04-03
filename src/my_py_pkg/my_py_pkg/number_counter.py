import rclpy
from rclpy.node import Node
from example_interfaces.msg import Int64
from example_interfaces.srv import SetBool


class NumberCounter(Node):
    def __init__(self):
        super().__init__("number_counter")
        self.num_count_ = 0
        self.num_count_publisher_ = self.create_publisher(Int64, "number_count", 10)
        self.num_reciever = self.create_subscription(
            Int64, "number", self.numRecCallback, 10
        )
        self.counter_reset_service_ = self.create_service(
            SetBool, "reset_counter", self.resetCounter
        )
        self.get_logger().info("Number Counter Node Started!")

    def publishNumberCount(self):
        msg = Int64()
        msg.data = self.num_count_
        self.num_count_publisher_.publish(msg)
        self.get_logger().info("Number Count Published")

    def numRecCallback(self, msg: Int64):
        self.get_logger().info(f"Number Recd: {msg.data}")
        self.num_count_ += 1
        self.publishNumberCount()

    def resetCounter(self, request: SetBool.Request, response: SetBool.Response):
        if request.data:
            self.num_count_ = 0
            response.success = True
            response.message = "Counter Reset Successfully"
        return response


def main(args=None):
    rclpy.init(args=args)
    node = NumberCounter()
    rclpy.spin(node)
    rclpy.shutdown()


if __name__ == "__main__":
    main()
