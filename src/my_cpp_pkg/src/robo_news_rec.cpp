#include "example_interfaces/msg/string.hpp"
#include "rclcpp/rclcpp.hpp"
class RoboNewsRec : public rclcpp::Node {
public:
  RoboNewsRec() : rclcpp::Node("robot_news_rec_cpp") {
    subscriber_ = this->create_subscription<example_interfaces::msg::String>(
        "robot_news", 10,
        std::bind(&RoboNewsRec::subscriptionCallback, this,
                  std::placeholders::_1));
    RCLCPP_INFO(get_logger(), "reciever started");
  }

private:
  rclcpp::Subscription<example_interfaces::msg::String>::SharedPtr subscriber_;
  void subscriptionCallback(example_interfaces::msg::String::SharedPtr msg) {
    RCLCPP_INFO(get_logger(), "%s", msg->data.c_str());
  }
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<RoboNewsRec>();
  rclcpp::spin(node);
  rclcpp::shutdown();
}
