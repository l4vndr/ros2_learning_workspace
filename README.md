# ROS 2 Learning Workspace

This repository contains my ROS 2 learning projects, experiments, and notes as I work through different robotics concepts using ROS 2, Gazebo, RViz2, and C++.

The goal of this workspace is not to build a single large project, but to gradually learn and apply robotics concepts through hands-on implementation.

## Environment

* ROS 2 Jazzy
* C++
* Gazebo
* RViz2
* Docker
* Arch Linux

---

## Learning Topics Covered

Throughout this workspace I explored:

* ROS 2 Nodes
* Publishers and Subscribers
* Services
* Parameters
* Launch Files
* Custom Messages
* Custom Services
* Gazebo Simulation
* RViz2 Visualization
* URDF/Xacro Robot Modeling
* ROS 2 ↔ Gazebo Integration
* Differential Drive Control

---

# Featured Projects

## 1. Autonomous Turtle Pursuit System

A ROS 2 application built using TurtleSim where a master turtle autonomously tracks and catches randomly spawned turtles.

### Architecture

The system consists of three independent nodes:

### Turtle Spawner

* Spawns turtles at random coordinates
* Uses ROS 2 services
* Publishes information about newly spawned turtles

### Target Finder

* Maintains a list of active turtles
* Selects the nearest target
* Removes completed targets from its records

### Master Turtle Commander

* Receives target coordinates
* Calculates heading and distance
* Controls turtle motion
* Requests target removal when reached

### Concepts Used

* Publishers/Subscribers
* Services
* Custom Messages
* Custom Services
* Node Architecture
* Autonomous Navigation Logic

---

## 2. Differential Drive Robot with 2-DOF Arm

A robot simulation project featuring:

* Differential drive locomotion
* 2-DOF robotic arm
* Camera sensor
* Gazebo simulation
* RViz2 visualization

### Development Workflow

1. Robot modeled in Onshape
2. Exported to URDF
3. Refined using Xacro
4. Added collision geometries
5. Integrated Gazebo plugins
6. Connected simulation to ROS 2 using ros_gz_bridge

### Features

* Differential drive control
* Joint control
* Camera simulation
* Joint state publishing
* TF publishing
* Odometry visualization

### Concepts Used

* URDF/Xacro
* Gazebo Plugins
* Robot Modeling
* Differential Drive Systems
* ROS 2 ↔ Gazebo Communication
* RViz2

---

# Other Experiments

This repository also contains smaller projects and exercises created while learning ROS 2 concepts such as:

* Topic communication
* Service communication
* Parameters
* Launch files
* Package creation
* Basic node interactions
* Testing and experimentation

These projects may be less polished but are included to document the learning process.

---

# Repository Structure

```text
src/
├── turtle_*
│   └── Autonomous Turtle Pursuit System

├── differential_drive_bot
│   └── Robot description and simulation

├── differential_drive_bot_bringup
│   └── Launch and bringup files

├── ...
│   └── Additional ROS 2 learning packages
```

---

# Future Plans

* Hardware implementation
* Sensor integration
* Navigation stack exploration
* SLAM experiments
* MoveIt integration
* More advanced robot simulations

---

This repository serves as a record of my robotics learning journey. Feedback, suggestions, and discussions are always welcome.
