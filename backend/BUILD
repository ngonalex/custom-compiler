
cc_library(
  name = "backend",
  copts = ["-Wall -g"],
  hdrs = ["lowerer_visitor.h", "code_gen.h", "ir.h",
          "helper_struct.h", "control_flow_graph.h"],
  srcs = ["code_gen.cc","lowerer_visitor.cc",
          "control_flow_graph.cc"],
  deps = ["//utility:utility",
	  "//abstract_syntax:abstract_syntax"],
  visibility = ["//visibility:public"],
)

cc_test(
  name = "backend_test",
  deps = ["@gtest//:main", ":backend"],
  srcs = [ "lowerer_visitor_test_v1.cc", "lowerer_visitor_test_v2.cc", 
           "lowerer_visitor_test_v3.cc", "lowerer_visitor_test_v4.cc", 
	   "lowerer_visitor_test_v5.cc", "codegen_test_v1.cc", "codegen_test_v2.cc",
	   "codegen_test_v3.cc", "codegen_test_v4.cc", "codegen_test_v5.cc",
           "control_flow_graph_test.cc", "optimizer_test.cc"
	 ],
  copts = ["-Iexternal/gtest/googletest-release-1.8.0/googletest/include"],
  timeout = "short",
  size = "small",
)

cc_test(
  name = "lowerer_test",
  deps = ["@gtest//:main", ":backend"],
  srcs = [ "lowerer_visitor_test_v1.cc", "lowerer_visitor_test_v2.cc", 
           "lowerer_visitor_test_v3.cc", "lowerer_visitor_test_v4.cc",
	   "lowerer_visitor_test_v5.cc"
	 ],
  copts = ["-Iexternal/gtest/googletest-release-1.8.0/googletest/include"],
  timeout = "short",
  size = "small",
)

cc_test(
  name = "control_flow_graph_test",
  deps = ["@gtest//:main", ":backend"],
  srcs = ["control_flow_graph_test.cc"],
  copts = ["-Iexternal/gtest/googletest-release-1.8.0/googletest/include"],
    timeout = "short",
	size = "small",
)

cc_test(
  name = "optimizer_test",
  deps = ["@gtest//:main", ":backend"],
  srcs = ["optimizer_test.cc"],
  copts = ["-Iexternal/gtest/googletest-release-1.8.0/googletest/include"],
    timeout = "short",
	size = "small",
)

cc_test(
  name = "codegen_test",
  deps = ["@gtest//:main", ":backend"],
  srcs = ["codegen_test_v1.cc", "codegen_test_v2.cc", "codegen_test_v3.cc", 
          "codegen_test_v4.cc", "codegen_test_v5.cc"
	 ],
  copts = ["-Iexternal/gtest/googletest-release-1.8.0/googletest/include"],
  timeout = "short",
  size = "small",
)

