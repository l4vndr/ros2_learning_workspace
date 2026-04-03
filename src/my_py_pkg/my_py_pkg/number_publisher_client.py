import rclpy
from rclpy.node import Node
from example_interfaces.srv import SetBool


class ResetSrv(Node):

    def __init__(self):
        super().__init__("reset_counter_srv")
        self.reset_count_client = self.create_client(SetBool, "reset_counter")

    def resetCounter(self, flag: bool):
        req = SetBool.Request()
        req.data = flag

        while not self.reset_count_client.wait_for_service(1):
            self.get_logger().warn("Waiting for server...")

        future = self.reset_count_client.call_async(request=req)
        future.add_done_callback(lambda f: self.srvCompleteCallback(future, req))

    def srvCompleteCallback(self, future, request: SetBool.Request):
        response: SetBool.Response = future.result()
        self.get_logger().info(f"Service Called: {response.message}")


def main(args=None):
    rclpy.init(args=args)
    node = ResetSrv()
    node.resetCounter(True)
    rclpy.spin(node)
    rclpy.shutdown()


if __name__ == "__main__":
    main()
