cmake_minimum_required(VERSION 3.8)
project(my_cpp_pkg)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(example_interfaces REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


add_executable(cpp_node src/first_node.cpp)
add_executable(cpp_node_oop src/first_node_oop.cpp)
add_executable(robo_news src/robo_news.cpp)
add_executable(robo_news_rec src/robo_news_rec.cpp)
add_executable(sum_server src/sum_server.cpp)
add_executable(sum_server_client src/sum_server_client.cpp)
add_executable(sum_server_client_oop src/sum_server_client_oop.cpp)
add_executable(number_publisher src/number_publisher.cpp)
add_executable(number_counter src/number_counter.cpp)
add_executable(number_publisher_client src/number_publisher_client.cpp)


ament_target_dependencies(cpp_node rclcpp)
ament_target_dependencies(cpp_node_oop rclcpp)
ament_target_dependencies(robo_news rclcpp example_interfaces)
ament_target_dependencies(robo_news_rec rclcpp example_interfaces)
ament_target_dependencies(sum_server rclcpp example_interfaces)
ament_target_dependencies(sum_server_client rclcpp example_interfaces)
ament_target_dependencies(sum_server_client_oop rclcpp example_interfaces)
ament_target_dependencies(number_publisher rclcpp example_interfaces)
ament_target_dependencies(number_counter rclcpp example_interfaces)
ament_target_dependencies(number_publisher_client rclcpp example_interfaces)


install(TARGETS
  cpp_node
  cpp_node_oop
  robo_news
  robo_news_rec
  sum_server
  sum_server_client
  sum_server_client_oop
  number_publisher
  number_counter
  number_publisher_client
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
