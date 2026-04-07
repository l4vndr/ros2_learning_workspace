#include "example_interfaces/msg/string.hpp"
#include "rcl_interfaces/msg/set_parameters_result.hpp"
#include "rclcpp/rclcpp.hpp"
#include <rclcpp/parameter.hpp>
#include <string>
#include <vector>

class RoboNews : public rclcpp::Node {
public:
  RoboNews() : Node("robot_news"), name{"C-BOT"} {
    RCLCPP_INFO(get_logger(), "Robo News Started (CPP)");
    publisher_ = this->create_publisher<example_interfaces::msg::String>(
        "robot_news", 10);

    this->declare_parameter("name", name);

    param_callback_handler = this->add_post_set_parameters_callback(
        [this](const std::vector<rclcpp::Parameter> &params)
            -> rcl_interfaces::msg::SetParametersResult {
          return this->handleParamCallback(params);
        });

    timer_ =
        this->create_wall_timer(std::chrono::milliseconds(500),
                                std::bind(&RoboNews::publisherCallback, this));
  }

private:
  rclcpp::Publisher<example_interfaces::msg::String>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  PostSetParametersCallbackHandle::SharedPtr param_callback_handler;
  std::string name;

  rcl_interfaces::msg::SetParametersResult
  handleParamCallback(const std::vector<rclcpp::Parameter> &param) {
    rcl_interfaces::msg::SetParametersResult result;
    result.successful = true;
    result.reason = "Success";

    for (auto it : param) {
      if (it.get_name() == "name") {
        name = it.as_string();
      }
    }

    return result;
  }

  void publisherCallback() {
    auto msg = example_interfaces::msg::String();
    msg.data = std::string("HELLO from " + name);
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
