#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp/rclcpp.hpp"
#include "turtle_project_interfaces/msg/target_coordinate.hpp"
#include "turtle_project_interfaces/srv/remove_turtle.hpp"
#include "turtlesim/msg/pose.hpp"
#include "turtlesim/srv/kill.hpp"
#include <chrono>
#include <cmath>
#include <memory>
#include <vector>

class MasterTurtleCommander : public rclcpp::Node {
public:
  MasterTurtleCommander() : rclcpp::Node("master_turtle_commander") {
    master_pose_subscription_ = this->create_subscription<turtlesim::msg::Pose>(
        "/master_turtle/pose", 10,
        [this](turtlesim::msg::Pose msg) { this->updateMasterPos(msg); });

    target_coords_subscription_ = this->create_subscription<
        turtle_project_interfaces::msg::TargetCoordinate>(
        "/target_coord", 10,
        [this](turtle_project_interfaces::msg::TargetCoordinate msg) {
          this->updateTargetCoord(msg);
        });

    velocity_publisher_ = this->create_publisher<geometry_msgs::msg::Twist>(
        "/master_turtle/cmd_vel", 10);

    kill_turtle_client_ = this->create_client<turtlesim::srv::Kill>("/kill");
    remove_turtle_client_ =
        this->create_client<turtle_project_interfaces::srv::RemoveTurtle>(
            "/remove_turtle");

    timer_ = this->create_wall_timer(std::chrono::milliseconds(100),
                                     [this]() { this->control_loop(); });
  }

private:
  void control_loop() {
    this->updateTargetDistance();
    this->updateTargetYaw();
    this->publishVelocityCommand();
    this->handleTargetReach();
  }

  void handleTargetReach() {
    if (targetHandled_)
      return;
    std::string currentTargetName = this->targetName_;
    if (targetDistance_ <= 1) {
      targetHandled_ = true;
      auto killReq = std::make_shared<turtlesim::srv::Kill::Request>();
      auto removeReq = std::make_shared<
          turtle_project_interfaces::srv::RemoveTurtle::Request>();

      killReq->name = currentTargetName;
      removeReq->name = currentTargetName;

      kill_turtle_client_->async_send_request(killReq);
      remove_turtle_client_->async_send_request(removeReq);
    }
  }

  void publishVelocityCommand() {
    auto velCmd = geometry_msgs::msg::Twist();
    double kv = 2;
    double kav = 2;
    double minSpeed = 2;

    double velocitX = kv * targetDistance_;
    if (abs(targetYaw_) < 0.3) {
      velocitX = kv * targetDistance_;
    } else if (abs(targetYaw_) > 0.3 && targetDistance_ <= 0.5) {
      velocitX = 0.25 * minSpeed;
    } else {
      velocitX = minSpeed;
    }
    // velocitX = velocitX * std::cos(targetYaw_);
    double anglularVelocity = kav * targetYaw_;

    velCmd.linear.x = velocitX;
    velCmd.angular.z = anglularVelocity;

    velocity_publisher_->publish(velCmd);
  }

  void updateMasterPos(turtlesim::msg::Pose msg) {
    masterX_ = msg.x;
    masterY_ = msg.y;
    masterYaw_ = msg.theta;
  }

  void updateTargetCoord(turtle_project_interfaces::msg::TargetCoordinate msg) {
    if (msg.name != targetName_) {
      targetHandled_ = false;
    }
    targetX_ = msg.x;
    targetY_ = msg.y;
    targetName_ = msg.name;
  }

  void updateTargetDistance() {
    targetDistance_ = std::hypot(masterX_ - targetX_, masterY_ - targetY_);
  }

  double magnitude(std::vector<double> V) {
    double sum = 0.0;
    for (auto it : V) {
      sum += it * it;
    }
    return std::sqrt(sum);
  }

  double normalize(double dtheta) {
    double PI = M_PI;
    while (dtheta > PI)
      dtheta -= 2 * PI;
    while (dtheta < -PI)
      dtheta += 2 * PI;
    return dtheta;
  }

  void updateTargetYaw() {

    std::vector<double> M{masterX_, masterY_};
    std::vector<double> T{targetX_, targetY_};
    std::vector<double> DEL{T[0] - M[0], T[1] - M[1]};

    double thetaTarget = std::atan2(DEL[1], DEL[0]);
    double dtheta = this->normalize(thetaTarget - masterYaw_);
    targetYaw_ = dtheta;
  }

  rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr
      master_pose_subscription_;
  rclcpp::Subscription<turtle_project_interfaces::msg::TargetCoordinate>::
      SharedPtr target_coords_subscription_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr velocity_publisher_;
  rclcpp::Client<turtle_project_interfaces::srv::RemoveTurtle>::SharedPtr
      remove_turtle_client_;
  rclcpp::Client<turtlesim::srv::Kill>::SharedPtr kill_turtle_client_;
  rclcpp::TimerBase::SharedPtr timer_;
  double masterX_, masterY_, masterYaw_, targetDistance_, targetYaw_;
  double targetX_, targetY_;
  bool targetHandled_;
  std::string targetName_;
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<MasterTurtleCommander>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
