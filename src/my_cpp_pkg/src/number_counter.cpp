#include "example_interfaces/msg/int64.hpp"
#include "example_interfaces/srv/set_bool.hpp"

#include "rclcpp/rclcpp.hpp"

class NumberCounter : public rclcpp::Node {
public:
  NumberCounter() : rclcpp::Node("number_counter") {
    RCLCPP_INFO(this->get_logger(), "Number Counter CPP Node has Strated!");
    this->num_count_publisher_ =
        this->create_publisher<example_interfaces::msg::Int64>("number_count",
                                                               10);
    this->num_reciever_ =
        this->create_subscription<example_interfaces::msg::Int64>(
            "number", 10, [this](example_interfaces::msg::Int64 msg) {
              this->numRecCallback(msg);
            });
    this->counter_reset_service_ =
        this->create_service<example_interfaces::srv::SetBool>(
            "reset_counter",
            [this](example_interfaces::srv::SetBool::Request::SharedPtr req,
                   example_interfaces::srv::SetBool::Response::SharedPtr res) {
              this->resetCounter(req, res);
            });
  }

private:
  rclcpp::Publisher<example_interfaces::msg::Int64>::SharedPtr
      num_count_publisher_;
  rclcpp::Subscription<example_interfaces::msg::Int64>::SharedPtr num_reciever_;
  int count_{0};
  rclcpp::Service<example_interfaces::srv::SetBool>::SharedPtr
      counter_reset_service_;

  void numRecCallback(example_interfaces::msg::Int64 msg) {
    RCLCPP_INFO(this->get_logger(), "Number Recd: %d", (int)msg.data);
    this->count_ += 1;
    this->publishNumberCount();
  }

  void publishNumberCount() {
    auto msg = example_interfaces::msg::Int64();
    msg.data = count_;
    this->num_count_publisher_->publish(msg);
    RCLCPP_INFO(this->get_logger(), "Number Count Published");
  }

  void
  resetCounter(const example_interfaces::srv::SetBool::Request::SharedPtr req,
               example_interfaces::srv::SetBool::Response::SharedPtr res) {

    if (req->data) {
      this->count_ = 0;
      res->success = true;
      res->message = "Counter Reset Successful";
    }
  }
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<NumberCounter>();
  rclcpp::spin(node);
  rclcpp::shutdown();
}
