#include "rclcpp/rclcpp.hpp"
#include "turtle_project_interfaces/msg/spawned_turtle.hpp"
#include "turtlesim/srv/spawn.hpp"
#include <chrono>
#include <cstdlib>
#include <memory>
#include <random>
#include <rclcpp/logging.hpp>
#include <rclcpp/parameter.hpp>
#include <string>
#include <string_view>
#include <vector>

class TurtleSpawnner : public rclcpp::Node {
public:
  TurtleSpawnner() : rclcpp::Node("turtle_spawner") {
    this->declare_parameter("spawn_freq", 1.0);
    param_handler_ = this->add_post_set_parameters_callback(
        [this](const std::vector<rclcpp::Parameter> &params) {
          for (auto it : params) {
            if (it.get_name() == "spawn_freq") {
              spawn_time_ = it.as_double();
            }
          }
        });

    spawn_time_ = (1.0 / this->get_parameter("spawn_freq").as_double()) * 1000;

    turtle_spawn_client_ = this->create_client<turtlesim::srv::Spawn>("/spawn");
    spawned_turtle_info_publisher_ =
        this->create_publisher<turtle_project_interfaces::msg::SpawnedTurtle>(
            "turtle_spawn_info", 10);

    this->timer_ =
        this->create_wall_timer(std::chrono::milliseconds(spawn_time_),
                                [this]() { this->timerCallback(); });

    RCLCPP_INFO(get_logger(), "Spawner Started");
  }

private:
  // void getTurtleCoords(
  //     turtle_project_interfaces::srv::TurtleCoords::Response::SharedPtr
  //         response) {
  //   response->success = true;
  //   response->x = currentX_;
  //   response->y = currentY_;
  //   response->yaw_in_rad = currentYawInRads_;
  //   response->name = turtleName_;
  //   response->msg = "Successful Transaction";
  //   return;
  // }

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
    // msg.spawned_turtle = true;

    auto result = future.get();
    currentX_ = request->x;
    currentY_ = request->y;
    currentYawInRads_ = request->theta;
    turtleName_ = result->name;

    msg.turtle_name = turtleName_;
    msg.x = currentX_;
    msg.y = currentY_;
    msg.yaw_in_rad = currentYawInRads_;
    this->spawned_turtle_info_publisher_->publish(msg);

    RCLCPP_INFO(get_logger(), "Turtle Spawned at (%.2f, %.2f)\nName: %s",
                currentX_, currentY_, result->name.c_str());

    // this->updateRecords();
  }

  rclcpp::Publisher<turtle_project_interfaces::msg::SpawnedTurtle>::SharedPtr
      spawned_turtle_info_publisher_;
  rclcpp::Client<turtlesim::srv::Spawn>::SharedPtr turtle_spawn_client_;
  rclcpp::TimerBase::SharedPtr timer_;
  double currentX_, currentY_, currentYawInRads_;
  std::string turtleName_;
  PostSetParametersCallbackHandle::SharedPtr param_handler_;
  int spawn_time_;
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<TurtleSpawnner>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
