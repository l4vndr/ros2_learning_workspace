#include "rclcpp/rclcpp.hpp"
#include "turtle_project_interfaces/msg/spawned_turtle.hpp"
#include "turtle_project_interfaces/srv/turtle_coords.hpp"
#include <chrono>
#include <memory>
#include <rclcpp/client.hpp>
#include <vector>

class TargetFinder : public rclcpp::Node {
public:
  TargetFinder() : rclcpp::Node("target_finder") {
    turtle_spawned_subscription_ = this->create_subscription<
        turtle_project_interfaces::msg::SpawnedTurtle>(
        "turtle_spawn_info", 10, [this]() { this->handleNewTurtleSpawn(); });

    turtle_coords_client_ =
        this->create_client<turtle_project_interfaces::srv::TurtleCoords>(
            "/get_turtle_coords");
  }

private:
  void handleNewTurtleSpawn() {
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
    std::vector<double> coords{result->x, result->y, result->yaw_in_rad};

    this->coords_.push_back(coords);
  }
  rclcpp::Subscription<turtle_project_interfaces::msg::SpawnedTurtle>::SharedPtr
      turtle_spawned_subscription_;
  rclcpp::Client<turtle_project_interfaces::srv::TurtleCoords>::SharedPtr
      turtle_coords_client_;

  std::vector<std::vector<double>> coords_;
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<TargetFinder>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
