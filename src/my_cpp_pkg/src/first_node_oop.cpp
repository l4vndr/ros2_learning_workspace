#include "rclcpp/rclcpp.hpp"

class NewNode : public rclcpp::Node {
public:
  NewNode() : Node("cpp_test_oop") {
    RCLCPP_INFO(this->get_logger(), "Hello World");
    counter_ = 0;
    timer_ = this->create_wall_timer(std::chrono::seconds(1),
                                     std::bind(&NewNode::timer_callback, this));
  }

private:
  void timer_callback() {
    RCLCPP_INFO(this->get_logger(), "Hello %d", counter_);
    counter_ += 1;
  }
  rclcpp::TimerBase::SharedPtr timer_;
  int counter_;
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<NewNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}

// #include "rclcpp/rclcpp.hpp"

// class MyNode : public rclcpp::Node {
// public:
//   MyNode() : rclcpp::Node("New_node") {
//     RCLCPP_INFO(get_logger(), "Hello World");
//     timer_ = this->create_wall_timer(std::chrono::seconds(1),
//                                      std::bind(&MyNode::timerCallback,
//                                      this));
//   }

// private:
//   void timerCallback() { RCLCPP_INFO(get_logger(), "Hello"); }
//   rclcpp::TimerBase::SharedPtr timer_;
// };

// int main(int argc, char **argv) {
//   rclcpp::init(argc, argv);
//   auto node = std::make_shared<MyNode>();
//   rclcpp::spin(node);
//   rclcpp::shutdown();
//   return 0;
// }
