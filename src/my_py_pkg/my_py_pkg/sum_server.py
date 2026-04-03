# import rclpy
# from rclpy.node import Node
# from example_interfaces.srv import AddTwoInts


# class SumServer(Node):
#     def __init__(self):
#         super().__init__("sum_server")
#         self.server_ = self.create_service(AddTwoInts, "sum_up", self.callback_sum_up)
#         self.get_logger().info("Sum server started")

#     def callback_sum_up(
#         self, request: AddTwoInts.Request, response: AddTwoInts.Response
#     ):
#         sum = request.a + request.b
#         response.sum = sum
#         self.get_logger().info("SUMMATION DONE " + str(response.sum))
#         return response


# def main(args=None):
#     rclpy.init(args=args)
#     node = SumServer()
#     rclpy.spin(node)
#     rclpy.shutdown()


# if __name__ == "__main__":
#     main()

import rclpy
from rclpy.node import Node
from example_interfaces.srv import AddTwoInts


class SumServer(Node):
    def __init__(self):
        super().__init__("sum_server")
        self.service_ = self.create_service(AddTwoInts, "sum_up", self.serviceCallback)
        self.get_logger().info("Service Started")

    def serviceCallback(
        self, request: AddTwoInts.Request, response: AddTwoInts.Response
    ):
        sum = request.a + request.b
        response.sum = sum
        self.get_logger().info("Sum = " + str(sum))
        return response


def main(args=None):
    rclpy.init(args=args)
    node = SumServer()
    rclpy.spin(node)
    rclpy.shutdown


if __name__ == "__main__":
    main()
