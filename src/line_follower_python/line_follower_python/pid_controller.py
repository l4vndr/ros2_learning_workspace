import rclpy
from rclpy.node import Node

from example_interfaces.msg import Int8
from example_interfaces.msg import Bool

# from example_interfaces.srv


class PIDController(Node):
    def __init__(self):
        super().__init__("pid_controller")
        self.PID = {"Kp": 0.5, "Kd": 1, "Ki": 0}
        self.line_error_ = 0
        self.line_detected_ = False

        self.Velocity = {"linear": 0, "angular": 0}

        self.line_error_subscription_ = self.create_subscription(
            Int8, "line_error", self.updateLineError, 10
        )

        self.line_detected_subscription_ = self.create_subscription(
            Bool, "line_detect", self.updateDetectedLineStatus, 10
        )

        self.linear_velocity_publisher_ = self.create_publisher(Int8, "linear_vel", 10)
        self.angular_velocity_publisher_ = self.create_publisher(
            Int8, "angular_vel", 10
        )

        self.get_logger().info("PID Controller Started")
        self.create_timer(
            1 / 500, lambda: (self.updateVelocities(), self.publishVelocity())
        )

    def updateLineError(self, msg: Int8):
        self.line_error_ = msg.data
        self.get_logger().info(str(msg.data))

    def updateDetectedLineStatus(self, msg: Bool):
        self.line_detected_ = msg.data
        self.get_logger().info(str(msg.data))

    def updateVelocities(self):
        self.Velocity["linear"] = self.PID["Kp"] * self.line_error_
        self.Velocity["angular"] = (
            self.PID["Kp"] * self.line_error_ - self.PID["Kd"] * self.line_error_
        )

    def publishVelocity(self):
        linear = Int8()
        angular = Int8()

        linear.data = int(self.Velocity["linear"])
        angular.data = int(self.Velocity["angular"])

        self.linear_velocity_publisher_.publish(linear)
        self.angular_velocity_publisher_.publish(angular)

        self.get_logger().info("Velocity published")


def main(args=None):
    rclpy.init(args=args)
    node = PIDController()
    rclpy.spin(node)
    rclpy.shutdown


if __name__ == "__main__":
    main()
