cc_library(
	name = "peg_library",
	copts = ["-Wall -g"],
	hdrs = ["atom_parser.h",
					"parse_result.h"],
	deps = ["//utility"],
	visibility = ["//visibility:public"],
)

cc_test(
	name = "lib_test",
	deps = ["@gtest//:main", ":peg_library"],
	srcs = ["lib_test.cc"],
	copts = ["-Iexternal/gtest/googletest-release-1.8.0/googletest/include"],
	timeout = "short",
	size = "small",
)