cmake_minimum_required(VERSION 2.8.3)
project(mavros_offboard_example)


find_package(catkin REQUIRED COMPONENTS roscpp mavros)

catkin_package(
  CATKIN_DEPENDS roscpp mavros
)
include_directories(include ${catkin_INCLUDE_DIRS})



add_executable(offb_node src/offb_node.cpp)
target_link_libraries(offb_node ${catkin_LIBRARIES})
add_dependencies(offb_node ${${PROJECT_NAME}EXPORTEDTARGETS} ${catkin_EXPORTED_TARGETS})
