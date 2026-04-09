#include "rclcpp/rclcpp.hpp"
#include "turtle_project_interfaces/msg/spawned_turtle.hpp"
#include "turtle_project_interfaces/msg/target_coordinate.hpp"
#include "turtle_project_interfaces/srv/remove_turtle.hpp"
#include "turtle_project_interfaces/srv/turtle_coords.hpp"
#include "turtlesim/msg/pose.hpp"
#include <chrono>
#include <cmath>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class TargetFinder : public rclcpp::Node {
public:
  TargetFinder() : rclcpp::Node("target_finder") {
    turtle_spawned_subscription_ = this->create_subscription<
        turtle_project_interfaces::msg::SpawnedTurtle>(
        "turtle_spawn_info", 10,
        [this](turtle_project_interfaces::msg::SpawnedTurtle msg) {
          this->handleNewTurtleSpawn(msg);
        });

    master_turtle_pose_subscription_ =
        this->create_subscription<turtlesim::msg::Pose>(
            "/master_turtle/pose", 10,
            [this](turtlesim::msg::Pose msg) { this->updateMasterCoord(msg); });

    turtle_coords_client_ =
        this->create_client<turtle_project_interfaces::srv::TurtleCoords>(
            "/get_turtle_coords");

    target_coord_publisher_ = this->create_publisher<
        turtle_project_interfaces::msg::TargetCoordinate>("/target_coord", 10);

    remove_turtle_service_ = this->create_service<
        turtle_project_interfaces::srv::RemoveTurtle>(
        "/remove_turtle",
        [this](const turtle_project_interfaces::srv::RemoveTurtle::Request::
                   SharedPtr request,
               turtle_project_interfaces::srv::RemoveTurtle::Response::SharedPtr
                   response) { this->removeTurtleService(request, response); });

    timer_ = create_wall_timer(std::chrono::milliseconds(500),
                               [this]() { this->timerCallback(); });
  }

private:
  void removeTurtleService(
      const turtle_project_interfaces::srv::RemoveTurtle::Request::SharedPtr
          request,
      turtle_project_interfaces::srv::RemoveTurtle::Response::SharedPtr
          response) {
    std::string name = request->name;
    this->coords_.erase(name);
    this->calculateTarget();
    response->success = true;
  }

  void timerCallback() {
    auto msg = turtle_project_interfaces::msg::TargetCoordinate();
    msg.x = this->targetX_;
    msg.y = this->targetY_;
    msg.name = this->targetName_;
    target_coord_publisher_->publish(msg);
  }
  void updateMasterCoord(turtlesim::msg::Pose msg) {
    masterPose_['x'] = msg.x;
    masterPose_['y'] = msg.y;
    masterPose_['Z'] = msg.theta;
  }

  void handleNewTurtleSpawn(turtle_project_interfaces::msg::SpawnedTurtle msg) {
    if (!msg.spawned_turtle) {
      return;
    }

    auto request = std::make_shared<
        turtle_project_interfaces::srv::TurtleCoords::Request>();

    while (!turtle_coords_client_->wait_for_service(std::chrono::seconds(1))) {
      RCLCPP_WARN(get_logger(), "Waiting for spawn server");
    }

    auto future = turtle_coords_client_->async_send_request(
        request,
        [this](rclcpp::Client<turtle_project_interfaces::srv::TurtleCoords>::
                   SharedFuture future) { handleNewTurtleCoords(future); });
  }

  void handleNewTurtleCoords(
      rclcpp::Client<turtle_project_interfaces::srv::TurtleCoords>::SharedFuture
          future) {
    auto result = future.get();
    std::string name = result->name;
    std::vector<double> coords{result->x, result->y, result->yaw_in_rad};

    // this->coords_.push_back(coords);
    this->coords_[name] = coords;
    this->calculateTarget();
  }

  void calculateTarget() {
    double x1 = masterPose_['x'];
    double y1 = masterPose_['y'];

    double targetX = 0.0, targetY = 0.0;
    double currentMin = INFINITY;
    // for (auto coord : coords_) {
    // for (int i{}; i < this->coords_.size(); i += 1) {
    for (auto coord : coords_) {
      //   auto coord = this->coords_[i];
      std::string name = coord.first;
      auto ordinates = coord.second;
      double x2 = ordinates[0];
      double y2 = ordinates[1];

      double newDistance = this->distance(x1, y1, x2, y2);
      if (currentMin > newDistance) {
        currentMin = newDistance;
        targetX = x2;
        targetY = y2;
        targetName_ = name;
      }
    }
    this->targetX_ = targetX;
    this->targetY_ = targetY;
  }

  double distance(double x1, double y1, double x2, double y2) {
    return std::hypot(x1 - x2, y1 - y2);
  }

  rclcpp::Subscription<turtle_project_interfaces::msg::SpawnedTurtle>::SharedPtr
      turtle_spawned_subscription_;
  rclcpp::Client<turtle_project_interfaces::srv::TurtleCoords>::SharedPtr
      turtle_coords_client_;
  rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr
      master_turtle_pose_subscription_;
  rclcpp::Publisher<turtle_project_interfaces::msg::TargetCoordinate>::SharedPtr
      target_coord_publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Service<turtle_project_interfaces::srv::RemoveTurtle>::SharedPtr
      remove_turtle_service_;

  std::map<char, double> masterPose_;

  //   std::list<std::vector<double>> coords_;
  std::unordered_map<std::string, std::vector<double>> coords_;
  // std::vector<std::vector<double>> coords_;

  double targetX_, targetY_;
  std::string targetName_;
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<TargetFinder>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
