#include "rclcpp/rclcpp.hpp"

#include "custom_interfaces/msg/led_state.hpp"
#include "custom_interfaces/srv/set_led.hpp"
#include <chrono>
#include <vector>

class LedPanel : public rclcpp::Node {
public:
  LedPanel() : rclcpp::Node("led_panel"), ledState(3, false) {
    RCLCPP_INFO(this->get_logger(), "Led Panel Node Started");

    led_state_publisher =
        this->create_publisher<custom_interfaces::msg::LedState>(
            "led_panel_state", 10);
    led_setter_service_ = this->create_service<custom_interfaces::srv::SetLed>(
        "set_led",
        [this](custom_interfaces::srv::SetLed::Request::SharedPtr request,
               custom_interfaces::srv::SetLed::Response::SharedPtr response) {
          this->setLed(request, response);
        });
    this->timer_ = create_wall_timer(std::chrono::seconds(1),
                                     [this]() { this->publishLEDState(); });
  }

private:
  void publishLEDState() {
    auto ledStateMsg = custom_interfaces::msg::LedState();
    for (auto it : ledState) {
      ledStateMsg.led_state.push_back(it);
    }
    led_state_publisher->publish(ledStateMsg);
  }

  void setLed(const custom_interfaces::srv::SetLed::Request::SharedPtr request,
              custom_interfaces::srv::SetLed::Response::SharedPtr response) {
    bool newLedStatus = request->power_on;
    int numLed = request->led_number;

    if (numLed < 0 || numLed >= static_cast<int>(std::size(ledState))) {
      response->success = false;
      response->msg = "Invalid LED Number";
      return;
    }

    if (newLedStatus) {
      for (auto i{0}; i <= static_cast<int>(std::size(ledState)); i++) {
        this->ledState[i] = i <= numLed;
      }
    } else {
      for (auto i{0}; i <= static_cast<int>(std::size(ledState)); i++) {
        this->ledState[i] = false;
      }
    }
    response->success = true;
    response->msg = "Success";
  }

  rclcpp::Publisher<custom_interfaces::msg::LedState>::SharedPtr
      led_state_publisher;
  rclcpp::Service<custom_interfaces::srv::SetLed>::SharedPtr
      led_setter_service_;
  //   bool *ledState{false, false, false};
  std::vector<bool> ledState;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<LedPanel>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
