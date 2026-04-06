#include "rclcpp/rclcpp.hpp"

#include "custom_interfaces/msg/led_state.hpp"
#include "custom_interfaces/srv/set_led.hpp"
#include <chrono>
#include <cmath>
#include <vector>

class BatteryCpp : public rclcpp::Node {
public:
  BatteryCpp()
      : rclcpp::Node("battery"), batteryLevel{100}, led_panel_status(3, false) {
    RCLCPP_INFO(get_logger(), "Battery Node Started");

    this->led_panel_state_subscription_ =
        create_subscription<custom_interfaces::msg::LedState>(
            "led_panel_state", 10,
            [this](custom_interfaces::msg::LedState::SharedPtr msg) {
              this->updateLedState(msg);
            });
    this->led_state_client_ =
        this->create_client<custom_interfaces::srv::SetLed>("set_led");

    this->timer_ = create_wall_timer(std::chrono::seconds(1), [this]() {
      this->updateBatteryLevel();
      this->callLedSetterSrv();
    });
  }

private:
  void updateLedState(custom_interfaces::msg::LedState::SharedPtr msg) {
    int i = 0;
    for (auto it : msg->led_state) {
      led_panel_status[i] = it;
      i += 1;
    }
    RCLCPP_INFO(get_logger(), "LED Current State: %d %d %d",
                static_cast<int>(msg->led_state[0]),
                static_cast<int>(msg->led_state[1]),
                static_cast<int>(msg->led_state[2]));
  }

  void updateBatteryLevel() {
    double end = fmod(std::time(nullptr), 100);

    if (batteryLevel > 0) {
      batteryLevel -= static_cast<int>(100 / 3) + 1;

      if (batteryLevel <= 0) {
        start = fmod(std::time(nullptr), 100);
      }
      return;
    } else if (end - start >= 5) {
      batteryLevel = 100;
    }
  }

  void callLedSetterSrv() {
    auto req = std::make_shared<custom_interfaces::srv::SetLed::Request>();
    if (batteryLevel >= 100 - 100 / 3) {
      req->led_number = 2;
      req->power_on = true;
    } else if (batteryLevel >= 100 - 2 * 100 / 3) {
      req->led_number = 1;
      req->power_on = true;
    } else if (batteryLevel > 0) {
      req->led_number = 0;
      req->power_on = true;
    } else {
      req->led_number = 0;
      req->power_on = false;
    }

    while (!led_state_client_->wait_for_service(std::chrono::seconds(1))) {
      RCLCPP_WARN(get_logger(), "Waiting for server...");
    }

    auto future = led_state_client_->async_send_request(
        req,
        [this, req](rclcpp::Client<custom_interfaces::srv::SetLed>::SharedFuture
                        future) { this->afterServiceCall(future); });
  }

  void afterServiceCall(
      rclcpp::Client<custom_interfaces::srv::SetLed>::SharedFuture future) {
    auto result = future.get();
    RCLCPP_INFO(get_logger(), "Success: %d\nMsg: %s", result->success,
                result->msg.c_str());
  }

  int batteryLevel;
  std::vector<bool> led_panel_status;
  rclcpp::Subscription<custom_interfaces::msg::LedState>::SharedPtr
      led_panel_state_subscription_;
  rclcpp::Client<custom_interfaces::srv::SetLed>::SharedPtr led_state_client_;
  rclcpp::TimerBase::SharedPtr timer_;
  double start;
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<BatteryCpp>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
