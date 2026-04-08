#include "rclcpp/rclcpp.hpp"
#include "turtle_project_interfaces/msg/spawned_turtle.hpp"
#include "turtle_project_interfaces/srv/turtle_coords.hpp"
#include <memory>

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
  void handleNewTurtleSpawn() {}
  rclcpp::Subscription<turtle_project_interfaces::msg::SpawnedTurtle>::SharedPtr
      turtle_spawned_subscription_;
  rclcpp::Client<turtle_project_interfaces::srv::TurtleCoords>::SharedPtr
      turtle_coords_client_;
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<TargetFinder>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
