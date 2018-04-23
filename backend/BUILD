
cc_library(
	name = "backend",
	copts = ["-Wall -g"],
	hdrs = ["lowerer_visitor.h","interpreter_visitor.h", "code_gen.h", "ir.h"],
  srcs = ["code_gen.cc","lowerer_visitor.cc"],
	deps = ["//utility:utility",
		"//abstract_syntax:abstract_syntax",
  ],
	visibility = ["//visibility:public"],
)

cc_test(
	name = "backend_test",
	deps = ["@gtest//:main", ":backend"],
	srcs = [
		"lowerer_visitor_test.cc","interpreter_visitor_test.cc"
	],
	copts = ["-Iexternal/gtest/googletest-release-1.8.0/googletest/include"],
	timeout = "short",
	size = "small",
)

cc_binary(
    name = "backend_main",
    copts = ["-Wall -g"],
    deps = [
        ":backend"
    ],
    srcs = ["backend_main.cc"],
)