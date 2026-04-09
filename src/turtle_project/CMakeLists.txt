cmake_minimum_required(VERSION 3.8)
project(turtle_project)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(turtle_project_interfaces REQUIRED)
find_package(turtle_project_interfaces REQUIRED)
find_package(turtlesim REQUIRED)
find_package(geometry_msgs REQUIRED)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(turtle_spawner src/turtle_spawner.cpp)
add_executable(target_finder src/target_finder.cpp)
add_executable(master_turtle_commander src/master_turtle_commander.cpp)

ament_target_dependencies(turtle_spawner rclcpp turtlesim turtle_project_interfaces)
ament_target_dependencies(target_finder rclcpp turtlesim turtle_project_interfaces)
ament_target_dependencies(master_turtle_commander rclcpp turtlesim turtle_project_interfaces geometry_msgs)

install(TARGETS
  turtle_spawner
  target_finder
  master_turtle_commander
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
