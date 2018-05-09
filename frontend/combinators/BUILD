cc_library(
	name = "combinators",
	copts = ["-Wall -g"],
	hdrs = ["null.h", "single_char.h", "single_digit.h"],
	srcs = ["single_char.cc", "single_digit.cc", "null.cc"],
	deps = ["//utility", "//abstract_syntax:abstract_syntax",
	"//abstract_syntax:print_visitor_v1"],
	visibility = ["//visibility:public"],
)

cc_test(
	name = "combinators_test",
	deps = ["@gtest//:main", ":combinators", "//abstract_syntax:abstract_syntax"],
	srcs = ["v1_test.cc"],
	copts = ["-Iexternal/gtest/googletest-release-1.8.0/googletest/include"],
	timeout = "short",
	size = "small",
)
