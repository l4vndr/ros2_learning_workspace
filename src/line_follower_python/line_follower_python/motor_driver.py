import rclpy
from rclpy.node import Node

from example_interfaces.msg import Int8
from example_interfaces.srv import AddTwoInts


class MotorDriver(Node):
    def __init__(self):
        super().__init__("motor_driver")
        self.leftPWM_ = self.rightPWM_ = 0
        self.baseSpeed_ = 0
        self.max_speed_ = 0
        self.linear_velocity_subscriber_ = self.create_subscription(
            Int8, "linear_vel", self.setBaseSpeed, 10
        )
        self.angular_velocity_subscriber_ = self.create_subscription(
            Int8, "angular_vel", self.setTurnSpeed, 10
        )

        self.left_pwm_publisher_ = self.create_publisher(Int8, "left_pwm", 10)
        self.right_pwm_publisher_ = self.create_publisher(Int8, "rigth_pwm", 10)

        self.max_speed_service_ = self.create_service(
            AddTwoInts, "set_max_speed", self.setMaxSpeed
        )

        self.get_logger().info("Motor Driver Started!")
        self.create_timer(1 / 500, self.publishPWM)

    def setBaseSpeed(self, msg: Int8):
        self.baseSpeed = msg.data

    def setTurnSpeed(self, msg: Int8):
        self.leftPWM_ = self.baseSpeed + msg.data
        self.rightPWM_ = self.baseSpeed - msg.data

    def publishPWM(self):
        left = right = Int8()
        left.data = self.leftPWM_
        right.data = self.rightPWM_

        self.left_pwm_publisher_.publish(left)
        self.right_pwm_publisher_.publish(right)

    def setMaxSpeed(self, request: AddTwoInts.Request, response: AddTwoInts.Response):
        self.max_speed_ = request.a
        response.sum = 1
        self.get_logger().info(f"Max Speed Set to {self.max_speed_}")
        return response


def main(args=None):
    rclpy.init(args=args)
    node = MotorDriver()
    rclpy.spin(node)
    rclpy.shutdown


if __name__ == "__main__":
    main()
