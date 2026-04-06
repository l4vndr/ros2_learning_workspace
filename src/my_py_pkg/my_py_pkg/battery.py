import rclpy
from rclpy.node import Node

from custom_interfaces.msg import LedState
from custom_interfaces.srv import SetLed

import time


class Battery(Node):
    def __init__(self):
        super().__init__("battery")

        self.batteryLevel = 100
        self.led_panel_state = [False, False, False]
        self.get_logger().info("Battery Node Started")

        self.led_panel_state_subscriber = self.create_subscription(
            LedState, "led_panel_state", self.updateLedState, 10
        )
        self.led_state_client = self.create_client(SetLed, "set_led")

        self.create_timer(1.0, lambda: (self.updateBatteryLevel(), self.callLedSetterSrv()))

    def updateLedState(self, msg: LedState):
        self.led_panel_state = list(msg.led_state)[:]
        self.get_logger().info(
            f"LED Current State: {self.led_panel_state[0]} {self.led_panel_state[1]} {self.led_panel_state[2]}"
        )

    def updateBatteryLevel(self):
        self.get_logger().info(f"{self.batteryLevel}")

        end = time.time() % 100

        if self.batteryLevel > 0:
            self.batteryLevel -= int(100 / 3) + 1
            if self.batteryLevel <= 0:
                self.start = time.time() % 100
            return

        elif end - self.start >= 5:
            self.batteryLevel = 100

    def callLedSetterSrv(self):
        request = SetLed.Request()

        self.get_logger().info(f"{self.batteryLevel}")

        if self.batteryLevel >= 100 - 100 / 3:
            request.led_number = 2
            request.power_on = True
        elif self.batteryLevel >= 100 - 2 * 100 / 3:
            request.led_number = 1
            request.power_on = True
        elif self.batteryLevel > 0:
            request.led_number = 0
            request.power_on = True
        else:
            request.led_number = 0
            request.power_on = False

        while not self.led_state_client.wait_for_service(timeout_sec=1):
            self.get_logger().warn("Waiting for server...")

        future = self.led_state_client.call_async(request=request)
        future.add_done_callback(lambda future: self.afterServiceCall(future))

    def afterServiceCall(self, future):
        if future.result() is None:
            self.get_logger().warn("Some Error")
            return
        response: SetLed.Response = future.result()
        self.get_logger().info(f"Success: {response.success}\nMsg: {response.msg}")


def main(args=None):
    rclpy.init(args=args)
    node = Battery()
    rclpy.spin(node)
    rclpy.shutdown()


if __name__ == "__main__":
    main()
