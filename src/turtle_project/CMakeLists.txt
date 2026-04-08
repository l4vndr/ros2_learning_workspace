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


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(turtle_spawner src/turtle_spawner.cpp)

ament_target_dependencies(turtle_spawner rclcpp turtlesim turtle_project_interfaces)

install(TARGETS
  turtle_spawner
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
