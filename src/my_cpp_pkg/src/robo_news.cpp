#include "example_interfaces/msg/string.hpp"
#include "rclcpp/rclcpp.hpp"

class RoboNews : public rclcpp::Node {
public:
  RoboNews() : Node("robot_news") {
    RCLCPP_INFO(get_logger(), "Robo News Started (CPP)");
    publisher_ = this->create_publisher<example_interfaces::msg::String>(
        "robot_news", 10);
    timer_ = this->create_wall_timer(std::chrono::milliseconds(500), std::bind(&RoboNews::publisherCallback, this));
  }

private:
  rclcpp::Publisher<example_interfaces::msg::String>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;

  void publisherCallback() {
    auto msg = example_interfaces::msg::String();
    msg.data = std::string("HELLO WORLD");
    // msg.set__data("HELLOW WORLD");
    this->publisher_->publish(msg);
  }
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<RoboNews>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
