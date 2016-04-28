cmake_minimum_required(VERSION 2.8.3)
project(deception_simulation)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

## Find catkin macros and libraries
find_package(catkin REQUIRED COMPONENTS
  roscpp
  tf
  grid_map_core
  grid_map_ros
  grid_map_msgs
  grid_map_visualization
  visualization_msgs
  sensor_msgs
  nav_msgs
)

## System dependencies are found with CMake's conventions
#find_package(Eigen3 REQUIRED)

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if you package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
  INCLUDE_DIRS
    include
#  LIBRARIES ${PROJECT_NAME}
  CATKIN_DEPENDS
    roscpp
    tf
    grid_map_core
    grid_map_ros
    grid_map_msgs
    grid_map_visualization
    visualization_msgs
    sensor_msgs
    nav_msgs
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${EIGEN3_INCLUDE_DIR}
)

## Declare a cpp library
# add_library(${PROJECT_NAME}
#   src/${PROJECT_NAME}/grid_map_visualization_node.cpp
# )

## Declare a cpp executable
add_executable(generate_map_visualization_node
  src/generate_map_visualization.cpp
)
target_link_libraries(generate_map_visualization_node
  ${catkin_LIBRARIES}
)
# add_dependencies(generate_map_visualization_node grid_map_visualization)

add_executable(generate_map_node
  src/generate_map_node.cpp
)
target_link_libraries(
  generate_map_node
  ${catkin_LIBRARIES}
)


#############
## Install ##
#############

# Mark executables and/or libraries for installation
install(
  TARGETS generate_map_visualization_node generate_map_node
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

# Mark other files for installation
install(
  DIRECTORY include
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)



