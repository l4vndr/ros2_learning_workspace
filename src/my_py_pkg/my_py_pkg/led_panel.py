import rclpy
from rclpy.node import Node

from custom_interfaces.msg import LedState
from custom_interfaces.srv import SetLed


class LedPanel(Node):
    def __init__(self):
        super().__init__("led_panel")
        self.led_state_ = [False, False, False]

        self.led_state_publisher_ = self.create_publisher(
            LedState, "led_panel_state", 10
        )

        self.led_setter_service_ = self.create_service(SetLed, "set_led", self.setLed)

        self.get_logger().info("Led Panel has started")
        self.create_timer(1, self.publishLEDState)

    def publishLEDState(self):
        led_state_msg_ = LedState()
        led_state_msg_.led_state = self.led_state_[:]

        self.led_state_publisher_.publish(led_state_msg_)

    def setLed(self, request, response):
        ledNumber = request.led_number
        newPowerStatus = request.power_on

        if ledNumber >= len(self.led_state_) or ledNumber < 0:
            response.success = False
            response.msg = "Invalid led number"
            return response

        if newPowerStatus:
            for i in range(len(self.led_state_)):
                self.led_state_[i] = i <= ledNumber
        else:
            for i in range(len(self.led_state_)):
                self.led_state_[i] = False

        response.success = True
        response.msg = "Success"
        return response


def main(args=None):
    rclpy.init(args=args)
    node = LedPanel()
    rclpy.spin(node)
    rclpy.shutdown()


if __name__ == "__main__":
    main()
