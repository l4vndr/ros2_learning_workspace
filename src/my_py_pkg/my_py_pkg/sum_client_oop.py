# import rclpy
# from rclpy.node import Node
# from example_interfaces.srv import AddTwoInts


# class SumClient(Node):
#     def __init__(self):
#         super().__init__("sum_client_oops")
#         self.client_ = self.create_client(AddTwoInts, "sum_up")
#         self.sum_ = 0

#     def callService(self, a, b):

#         request = AddTwoInts.Request()
#         request.a = a
#         request.b = b
#         while not self.client_.wait_for_service(1.0):
#             self.get_logger().warn("Server Not Started...\n")

#         future = self.client_.call_async(request=request)
#         future.add_done_callback(self.callbackResult)

#     def callbackResult(self, future):
#         response = future.result()
#         self.get_logger().info("Response recd " + str(response.sum))
#         self.sum_ = response.sum


# def main(args=None):
#     rclpy.init(args=args)

#     node = SumClient()
#     node.callService(5, 3)

#     node.get_logger().info("Sum complete ")

#     rclpy.spin(node)
#     rclpy.shutdown()


# if __name__ == "__main__":
#     main()

import rclpy
from rclpy.node import Node
from example_interfaces.srv import AddTwoInts


class SumClient(Node):
    def __init__(self):
        super().__init__("sum_client_oop")
        self.client_ = self.create_client(AddTwoInts, "sum_up")

    def addTwoInts(self, a, b):
        req = AddTwoInts.Request()
        req.a = a
        req.b = b

        while not self.client_.wait_for_service():
            self.get_logger().warn("Server Not Running")

        future = self.client_.call_async(request=req)
        future.add_done_callback(lambda f: self.callbackSrv(f, req))

    def callbackSrv(self, future, request: AddTwoInts.Request):
        response = future.result()
        self.get_logger().info(f"{request.a} + {request.b} = {response.sum}")


def main(args=None):
    rclpy.init(args=args)
    node = SumClient()
    a, b = 5, 6
    node.addTwoInts(a, b)
    rclpy.spin(node)
    rclpy.shutdown()


if __name__ == "__main__":
    main()
