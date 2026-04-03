#include "example_interfaces/srv/set_bool.hpp"
#include "rclcpp/rclcpp.hpp"

class ResetSrv : public rclcpp::Node {
public:
  ResetSrv() : rclcpp::Node("reset_counter_srv") {
    RCLCPP_INFO(this->get_logger(), "Client Node Started");
    this->reset_count_client_ =
        this->create_client<example_interfaces::srv::SetBool>("reset_counter");
  }

  void resetCounter(bool flag) {
    auto req = std::make_shared<example_interfaces::srv::SetBool::Request>();
    req->data = flag;
    while (rclcpp::ok() && !reset_count_client_->wait_for_service(std::chrono::seconds(1))) {
      RCLCPP_WARN(this->get_logger(), "Waiting for server...");
    }
    auto future = reset_count_client_->async_send_request(
        req, [this, req](
                 rclcpp::Client<example_interfaces::srv::SetBool>::SharedFuture
                     future) { this->clientCallback(future, req); });
  }

private:
  void clientCallback(
      rclcpp::Client<example_interfaces::srv::SetBool>::SharedFuture future,
      example_interfaces::srv::SetBool::Request::SharedPtr req) {
    auto result = future.get();

    RCLCPP_INFO(this->get_logger(), "%d  %d = %s", (int)req->data,
                (int)result->success, result->message.c_str());
  }

  rclcpp::Client<example_interfaces::srv::SetBool>::SharedPtr
      reset_count_client_;
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<ResetSrv>();
  node->resetCounter(1);
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
