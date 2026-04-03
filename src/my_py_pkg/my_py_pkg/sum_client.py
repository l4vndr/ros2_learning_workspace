# import rclpy
# from rclpy.node import Node
# from example_interfaces.srv import AddTwoInts


# def main(args=None):
#     rclpy.init(args=args)
#     node = Node("sum_client")

#     request = AddTwoInts.Request()
#     request.a = 5
#     request.b = 3

#     client = node.create_client(AddTwoInts, "sum_up")

#     while not client.wait_for_service(1):
#         node.get_logger().warn("Server Not Running\nWaiting for Server...\n")

#     future = client.call_async(request=request)

#     rclpy.spin_until_future_complete(node, future)

#     response = future.result()

#     node.get_logger().info("a + b = " + str(response.sum))

#     rclpy.shutdown()


# if __name__ == "__main__":
#     main()


import rclpy
from rclpy.node import Node
from example_interfaces.srv import AddTwoInts


def main(args=None):
    rclpy.init(args=args)
    node = Node("sum_client_rec")
    req = AddTwoInts.Request()
    req.a = 5
    req.b = 3

    client = node.create_client(AddTwoInts, "sum_up")

    while not client.wait_for_service():
        node.get_logger().warn("SERVER NOT RUNNING...")

    future = client.call_async(request=req)

    rclpy.spin_until_future_complete(node, future)

    response = future.result()

    if response is None:
        node.get_logger().error("Service call failed")
        rclpy.shutdown()
        return

    node.get_logger().info(f"RESPONSE RECD: SUM = {response.sum}")

    rclpy.shutdown()


if __name__ == "__main__":
    main()
