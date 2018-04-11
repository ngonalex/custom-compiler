
cc_library(
	name = "smart_pointer_primer",
	copts = ["-Wall -g"],
	hdrs = ["simple_example.h"],
	visibility = ["//public:__pkg__"],
)

cc_test(
	name = "smart_pointer_primer_test",
	deps = ["@gtest//:main", "//utility:utility", ":smart_pointer_primer"],
	srcs = [
		"simple_example_test.cc"
	],
	copts = ["-Iexternal/gtest/googletest-release-1.8.0/googletest/include"],
	timeout = "short",
	size = "small",
)
