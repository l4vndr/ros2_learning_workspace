#include "rclcpp/rclcpp.hpp"
#include "turtle_project_interfaces/msg/spawned_turtle.hpp"
#include "turtle_project_interfaces/srv/turtle_coords.hpp"
#include "turtlesim/srv/spawn.hpp"
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <random>
#include <string_view>
#include <vector>

class TurtleSpawnner : public rclcpp::Node {
public:
  TurtleSpawnner() : rclcpp::Node("turtle_spawnner") {
    turtle_spawn_client_ = this->create_client<turtlesim::srv::Spawn>("/spawn");
    spawned_turtle_info_publisher_ =
        this->create_publisher<turtle_project_interfaces::msg::SpawnedTurtle>(
            "turtle_spawn_info", 10);
    turtle_coordinates_service_ = this->create_service<
        turtle_project_interfaces::srv::TurtleCoords>(
        "/get_turtle_coords",
        [this](const turtle_project_interfaces::srv::TurtleCoords::Request::
                   SharedPtr request,
               turtle_project_interfaces::srv::TurtleCoords::Response::SharedPtr
                   response) { this->getTurtleCoords(request, response); });

    this->timer_ = this->create_wall_timer(std::chrono::seconds(2),
                                           [this]() { this->timerCallback(); });
  }

private:
  void getTurtleCoords(
      const turtle_project_interfaces::srv::TurtleCoords::Request::SharedPtr
          request,
      turtle_project_interfaces::srv::TurtleCoords::Response::SharedPtr
          response) {
    // int index{request->i};
    // if (index >= static_cast<int>(this->coordinates.size()) || index < 0) {
    //   response->success = false;
    //   response->msg = "Out of bounds";
    //   return;
    // }
    // index = this->coordinates.size() - index - 1;
    // response->success = true;
    // response->x = this->coordinates[index][0];
    // response->y = this->coordinates[index][1];
    // response->yaw_in_rad = this->coordinates[index][2];
    response->success = true;
    response->x = currentX_;
    response->y = currentY_;
    response->yaw_in_rad = currentYawInRads;
    response->msg = "Successful Transaction";
    return;
  }

  void timerCallback() { this->spawnTurtle(); }

  double getRandomNumber(double i = 1.0, double j = 10.0) {
    std::random_device rd;
    // 2. Initialize Mersenne Twister engine
    std::mt19937 gen(rd());
    // 3. Define the range [1.0, 10.0)
    std::uniform_real_distribution<double> dis(i, j);

    double randomNum = dis(gen);
    return randomNum;
  }

  void spawnTurtle(std::string_view name = "") {

    auto request = std::make_shared<turtlesim::srv::Spawn::Request>();
    request->x = getRandomNumber();
    request->y = getRandomNumber();
    request->theta = getRandomNumber();
    request->name = name;

    while (!this->turtle_spawn_client_->wait_for_service(
        std::chrono::seconds(1))) {
      RCLCPP_WARN(get_logger(), "Waiting for spawn server");
    }

    auto future = turtle_spawn_client_->async_send_request(
        request,
        [this,
         request](rclcpp::Client<turtlesim::srv::Spawn>::SharedFuture future) {
          this->spawnFutureCallback(future, request);
        });
  }

  void spawnFutureCallback(
      rclcpp::Client<turtlesim::srv::Spawn>::SharedFuture future,
      turtlesim::srv::Spawn::Request::SharedPtr request) {

    auto msg = turtle_project_interfaces::msg::SpawnedTurtle();
    msg.spawned_turtle = true;
    this->spawned_turtle_info_publisher_->publish(msg);

    auto result = future.get();
    currentX_ = request->x;
    currentY_ = request->y;
    currentYawInRads = request->theta;
    RCLCPP_INFO(get_logger(), "Turtle Spawned at (%.2f, %.2f)\nName: %s",
                currentX_, currentY_, result->name.c_str());

    // this->updateRecords();
  }

  // void updateRecords() {
  //   std::vector<double> coords{currentX_, currentY_, currentYawInRads};
  //   this->coordinates.push_back(coords);
  // }

  rclcpp::Publisher<turtle_project_interfaces::msg::SpawnedTurtle>::SharedPtr
      spawned_turtle_info_publisher_;
  rclcpp::Client<turtlesim::srv::Spawn>::SharedPtr turtle_spawn_client_;
  rclcpp::Service<turtle_project_interfaces::srv::TurtleCoords>::SharedPtr
      turtle_coordinates_service_;
  rclcpp::TimerBase::SharedPtr timer_;
  double currentX_, currentY_, currentYawInRads;
  // std::vector<std::vector<double>> coordinates;
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<TurtleSpawnner>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
