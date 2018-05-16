cc_library(
	name = "combinators",
	copts = ["-Wall -g"],
	hdrs = ["word_parser.h"],
	srcs = ["word_parser.cc"],
	deps = ["//utility", "//frontend/combinators/basic_combinators", "//abstract_syntax:abstract_syntax",
	"//abstract_syntax:print_visitor_v1"],
	visibility = ["//visibility:public"],
)

cc_test(
	name = "combinators_test",
	deps = ["@gtest//:main", ":combinators", "//frontend/combinators/basic_combinators", "//abstract_syntax:abstract_syntax"],
	srcs = ["v2_test.cc"],
	copts = ["-Iexternal/gtest/googletest-release-1.8.0/googletest/include"],
	timeout = "short",
	size = "small",
)
