import rclpy
from rclpy.node import Node

from example_interfaces.srv import AddTwoInts

import sys


class TestingClient(Node):
    def __init__(self):
        super().__init__("testing_client")
        self.pid_client_ = self.create_client(AddTwoInts, "update_pid")
        self.max_speed_client_ = self.create_client(AddTwoInts, "set_max_speed")

        self.pidReq = AddTwoInts.Request()
        self.maxSpeedReq = AddTwoInts.Request()

    def makeReq(self, whichSrv, a=0, b=0):
        match whichSrv:
            case 1:
                self.pidReq.a = a
                self.pidReq.b = b

                while not self.pid_client_.wait_for_service(timeout_sec=1):
                    self.get_logger().info("Waiting for Service")

                return self.pid_client_.call_async(self.pidReq)

            case 2:
                self.maxSpeedReq.a = a

                while not self.max_speed_client_.wait_for_service(timeout_sec=1):
                    self.get_logger().info("Waiting for Service")

                return self.max_speed_client_.call_async(self.maxSpeedReq)

            case _:
                self.get_logger().info(
                    "Wrong service called... Choose 1 or 2.. Calling default 1"
                )
                self.pidReq.a = a
                self.pidReq.b = b

                while not self.pid_client_.wait_for_service(timeout_sec=1):
                    self.get_logger().info("Waiting for Service")

                return self.pid_client_.call_async(self.pidReq)


def main(args=None):
    rclpy.init()
    node = TestingClient()

    srv = 1
    a = 2
    b = 3

    future = node.makeReq(int(sys.argv[1]), int(sys.argv[2]), int(sys.argv[3]))
    rclpy.spin_until_future_complete(node, future)
    response = future.result()

    if response is None:
        node.get_logger().error("Service call failed")
        rclpy.shutdown()
        return

    node.get_logger().info(
        f"Called Service #{sys.argv[1]} with a={sys.argv[2]}, b={sys.argv[3]}, result is {response.sum}"
    )

    node.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()
