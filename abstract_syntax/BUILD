
cc_library(
	name = "abstract_syntax",
	copts = ["-Wall -g"],
	hdrs = ["abstract_syntax_tree.h", "print_visitor.h"],
	deps = ["//utility:utility"],
	visibility = ["//visibility:public"],
)

cc_test(
	name = "abstract_syntax_test",
	deps = ["@gtest//:main", ":abstract_syntax"],
	srcs = [
		"print_visitor_test.cc",
	],
	copts = ["-Iexternal/gtest/googletest-release-1.8.0/googletest/include"],
	timeout = "short",
	size = "small",
)
