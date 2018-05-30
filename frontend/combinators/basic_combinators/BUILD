cc_library(
	name = "basic_combinators",
	copts = ["-Wall -g"],
	hdrs = ["parse_status.h",
					"atom_parser.h"],
	srcs = ["atom_parser.cc"],
	deps = ["//utility", "//abstract_syntax:abstract_syntax", "//abstract_syntax:print_visitor_v1"],
	visibility = ["//visibility:public"],
)

cc_test(
	name = "combinators_test",
	deps = ["@gtest//:main", ":basic_combinators", "//abstract_syntax:abstract_syntax"],
	srcs = ["basic_test.cc"],
	copts = ["-Iexternal/gtest/googletest-release-1.8.0/googletest/include"],
	timeout = "short",
	size = "small",
)