#include "example_interfaces/srv/add_two_ints.hpp"
#include "rclcpp/rclcpp.hpp"

class SumClientOOP : public rclcpp::Node {
public:
  SumClientOOP() : rclcpp::Node("sum_client_oop") {
    this->client_ =
        this->create_client<example_interfaces::srv::AddTwoInts>("sum_up");
  }

  void getService(int a, int b) {
    auto req = std::make_shared<example_interfaces::srv::AddTwoInts::Request>();
    req->a = a;
    req->b = b;

    while (!this->client_->wait_for_service(std::chrono::seconds(1))) {
      RCLCPP_WARN(this->get_logger(), "SERVER UNAVL. Waiting...");
    }

    auto future = client_->async_send_request(
        req,
        [this,
         req](rclcpp::Client<example_interfaces::srv::AddTwoInts>::SharedFuture future) { futureCallback(future, req); });
  }

private:
  rclcpp::Client<example_interfaces::srv::AddTwoInts>::SharedPtr client_;
  void futureCallback(
      rclcpp::Client<example_interfaces::srv::AddTwoInts>::SharedFuture future,
      example_interfaces::srv::AddTwoInts::Request::SharedPtr r) {
    auto result = future.get();
    RCLCPP_INFO(this->get_logger(), "%d + %d = %d", (int)r->a, (int)r->b,
                (int)result->sum);
  }
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<SumClientOOP>();
  node->getService(69, 2);
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
