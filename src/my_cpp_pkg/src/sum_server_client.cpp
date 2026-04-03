#include "example_interfaces/srv/add_two_ints.hpp"
#include "rclcpp/rclcpp.hpp"

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<rclcpp::Node>("sum_server_client");

  auto req = std::make_shared<example_interfaces::srv::AddTwoInts::Request>();
  req->a = 5;
  req->b = 10;

  rclcpp::Client<example_interfaces::srv::AddTwoInts>::SharedPtr client =
      node->create_client<example_interfaces::srv::AddTwoInts>("sum_up");

  while (!client->wait_for_service(std::chrono::seconds(1))) {
    RCLCPP_WARN(node->get_logger(), "SERVER UNAVL. Waiting...");
  }

  auto future = client->async_send_request(req);

  rclcpp::spin_until_future_complete(node, future);

  auto result = future.get();

  RCLCPP_INFO(node->get_logger(), "Sum recd: %d", (int)result->sum);

  rclcpp::shutdown();

  return 0;
}
