#include "example_interfaces/srv/add_two_ints.hpp"
#include "rclcpp/rclcpp.hpp"
#include <memory>
#include <rclcpp/executors.hpp>
#include <rclcpp/utilities.hpp>

class SumServer : public rclcpp::Node {
public:
  SumServer() : rclcpp::Node("sum_server_cpp") {
    this->server_ = this->create_service<example_interfaces::srv::AddTwoInts>(
        "sum_up", std::bind(&SumServer::callbackService, this,
                            std::placeholders::_1, std::placeholders::_2));
    RCLCPP_INFO(this->get_logger(), "Server Started");
  }

private:
  void callbackService(
      const example_interfaces::srv::AddTwoInts::Request::SharedPtr request,
      example_interfaces::srv::AddTwoInts::Response::SharedPtr response) {

    response->sum = request->a + request->b;
    RCLCPP_INFO(this->get_logger(), "SUMMATION PERFORMED %d",
                (int)response->sum);
  }
  //   rclcpp::Client<example_interfaces::srv::AddTwoInts>::SharedPtr client_;
  rclcpp::Service<example_interfaces::srv::AddTwoInts>::SharedPtr server_;
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<SumServer>();
  rclcpp::spin(node);
  rclcpp::shutdown();
}
