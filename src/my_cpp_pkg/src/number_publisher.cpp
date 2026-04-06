#include "example_interfaces/msg/int64.hpp"
#include "rclcpp/rclcpp.hpp"
#include <chrono>

class NumberPublisher : public rclcpp::Node {
public:
  NumberPublisher() : rclcpp::Node("number_publisher") {
    RCLCPP_INFO(this->get_logger(), "Number Publisher CPP Node has Strated!");

    this->declare_parameter("number", 2);
    this->declare_parameter("freq", 0.5);

    number_ = this->get_parameter("number").as_int();
    freq_ = this->get_parameter("freq").as_double();

    this->num_publisher_ =
        this->create_publisher<example_interfaces::msg::Int64>("number", 10);
    this->timer_ = this->create_wall_timer(std::chrono::duration<double>(freq_),
                                           [this]() { this->publishNumber(); });
  }

private:
  rclcpp::Publisher<example_interfaces::msg::Int64>::SharedPtr num_publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  int number_;
  double freq_;

  void publishNumber() {
    auto msg = example_interfaces::msg::Int64();
    msg.data = number_;
    this->num_publisher_->publish(msg);
    RCLCPP_INFO(this->get_logger(), "Number Published");
  }
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<NumberPublisher>();
  rclcpp::spin(node);
  rclcpp::shutdown();
}
