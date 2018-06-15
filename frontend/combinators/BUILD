cc_library(
	name = "preparser",
	copts = ["-Wall -g"],
	srcs = ["preparser.cc"],
	deps = [],
	visibility = ["//visibility:public"],
)

cc_test(
	name = "preparser_test",
	deps = ["@gtest//:main", ":preparser"],
	srcs = ["preparser_test.cc"],
	copts = ["-Iexternal/gtest/googletest-release-1.8.0/googletest/include"],
	timeout = "short",
	size = "small",
)
