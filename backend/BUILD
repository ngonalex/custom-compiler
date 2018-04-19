
cc_library(
	name = "backend",
	copts = ["-Wall -g"],
	hdrs = ["lowerer_visitor.h","interpreter_visitor.h"],
	deps = ["//utility:utility",
		"//abstract_syntax:abstract_syntax",
  ],
	visibility = ["//visibility:public"],
)

cc_test(
	name = "backend_test",
	deps = ["@gtest//:main", ":backend", "//abstract_syntax:abstract_syntax"],
	srcs = [
		"lowerer_visitor_test.cc","interpreter_visitor_test.cc"
	],
	copts = ["-Iexternal/gtest/googletest-release-1.8.0/googletest/include"],
	timeout = "short",
	size = "small",
)
