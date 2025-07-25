cmake_minimum_required(VERSION 3.8)
project(foros)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(foros_msgs REQUIRED)
find_package(leveldb REQUIRED)
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 REQUIRED)

include_directories(src)

set(${PROJECT_NAME}_SRCS
  src/cluster_node.cpp
  src/cluster_node_options.cpp
  src/cluster_node_impl.cpp
  src/command.cpp
  src/common/node_util.cpp
  src/raft/context.cpp
  src/raft/context_store.cpp
  src/raft/other_node.cpp
  src/raft/state.cpp
  src/raft/state_machine.cpp
  src/raft/state/candidate.cpp
  src/raft/state/follower.cpp
  src/raft/state/leader.cpp
  src/raft/state/standby.cpp
  src/raft/inspector.cpp
  src/lifecycle/state.cpp
  src/lifecycle/state/active.cpp
  src/lifecycle/state/inactive.cpp
  src/lifecycle/state/standby.cpp
)

add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_SRCS})

target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

ament_target_dependencies(${PROJECT_NAME}
  rclcpp
  foros_msgs
  leveldb
)

pybind11_add_module(cluster_node_py
  python_bindings/cluster_node_py.cpp
)

target_include_directories(cluster_node_py
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(cluster_node_py
  PRIVATE ${PROJECT_NAME}
)

# why shoud I put it as well?? need to check
target_link_libraries(${PROJECT_NAME} leveldb)

ament_export_include_directories(include)
ament_export_libraries(${PROJECT_NAME})
ament_export_targets(${PROJECT_NAME})
ament_export_dependencies(rclcpp foros_msgs leveldb)

install(
  DIRECTORY include/
  DESTINATION include
)

install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(
  TARGETS cluster_node_py
  LIBRARY DESTINATION lib/${PROJECT_NAME}/python3.${Python3_VERSION_MAJOR}
)

if(BUILD_TESTING)
  find_package(ament_cmake_cpplint REQUIRED)
  ament_cpplint(FILTERS "-build/header_guard,-build/include_order")

  find_package(ament_cmake_cppcheck REQUIRED)
  ament_cppcheck()

  find_package(ament_cmake_lint_cmake REQUIRED)
  ament_lint_cmake()

  find_package(ament_cmake_xmllint REQUIRED)
  ament_xmllint()

  add_subdirectory(test)
endif()

ament_package()
