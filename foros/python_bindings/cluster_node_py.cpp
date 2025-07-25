#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>

#include <akit/failover/foros/cluster_node.hpp>
#include <akit/failover/foros/cluster_node_options.hpp>

namespace py = pybind11;
using namespace akit::failover::foros;

#define BIND_METHOD(CLASS, METHOD) \
  .def(#METHOD, &CLASS::METHOD)

  PYBIND11_MODULE(cluster_node_py, m) {
    py::class_<ClusterNodeOptions>(m, "ClusterNodeOptions")
      // only expose zero‑arg ctor
      .def(py::init([]() {
        return ClusterNodeOptions(rcl_get_default_allocator());
      }))
      // getter only: unsigned int ClusterNodeOptions::election_timeout_min() const
      .def_property_readonly(
        "election_timeout_min",
        static_cast<unsigned int (ClusterNodeOptions::*)() const>(
          &ClusterNodeOptions::election_timeout_min
        )
      )
      // same for timeout_max
      .def_property_readonly(
        "election_timeout_max",
        static_cast<unsigned int (ClusterNodeOptions::*)() const>(
          &ClusterNodeOptions::election_timeout_max
        )
      )
      // and for the string-returning temp_directory()
      .def_property_readonly(
        "temp_directory",
        static_cast<std::string (ClusterNodeOptions::*)() const>(
          &ClusterNodeOptions::temp_directory
        )
      );

  py::class_<ClusterNode, std::shared_ptr<ClusterNode>>(m, "ClusterNode")

    .def(py::init<const std::string&, uint32_t, const std::vector<uint32_t>&,
                  const ClusterNodeOptions&>(),
         py::arg("cluster_name"),
         py::arg("node_id"),
         py::arg("cluster_node_ids"),
         py::arg("options") = ClusterNodeOptions())
    .def(py::init<const std::string&, uint32_t, const std::vector<uint32_t>&,
                  const std::string&, const ClusterNodeOptions&>(),
         py::arg("cluster_name"),
         py::arg("node_id"),
         py::arg("cluster_node_ids"),
         py::arg("node_namespace"),
         py::arg("options") = ClusterNodeOptions())

    BIND_METHOD(ClusterNode, get_name)
    BIND_METHOD(ClusterNode, get_namespace)
    BIND_METHOD(ClusterNode, get_fully_qualified_name)
    BIND_METHOD(ClusterNode, get_logger)
    BIND_METHOD(ClusterNode, create_callback_group)
    BIND_METHOD(ClusterNode, for_each_callback_group)

    .def(
      "declare_parameter",
      (const rclcpp::ParameterValue & (ClusterNode::*)(
          const std::string &,
          const rclcpp::ParameterValue &,
          const rcl_interfaces::msg::ParameterDescriptor &,
          bool)) 
        &ClusterNode::declare_parameter,
      py::arg("name"),
      py::arg("default_value"),
      py::arg("descriptor"),
      py::arg("ignore_override")
    )

    BIND_METHOD(ClusterNode, undeclare_parameter)
    BIND_METHOD(ClusterNode, has_parameter)

    BIND_METHOD(ClusterNode, get_node_names)
    BIND_METHOD(ClusterNode, get_topic_names_and_types)
    BIND_METHOD(ClusterNode, get_service_names_and_types)

    BIND_METHOD(ClusterNode, count_publishers)
    BIND_METHOD(ClusterNode, count_subscribers)

    BIND_METHOD(ClusterNode, register_on_activated)
    BIND_METHOD(ClusterNode, register_on_deactivated)
    BIND_METHOD(ClusterNode, register_on_standby)
    ;
}
