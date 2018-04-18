cc_library(
	name = "simple_example",
	copts = ["-Wall -g"],
	hdrs = ["simple_example.h"],
        deps = ["//utility"],
	visibility = ["//public:__pkg__"],
)

cc_test(
<<<<<<< HEAD
	name = "smart_pointer_primer_test",
	deps = ["@gtest//:main", "//utility:utility", ":smart_pointer_primer"],
	srcs = [
		"simple_example_test.cc"
	],
	copts = ["-Iexternal/gtest/googletest-release-1.8.0/googletest/include"],
=======
	name = "simple_example_test",
	deps = ["@gtest//:main", "//utility", ":simple_example"],
	srcs = ["simple_example_test.cc"],
>>>>>>> d8da0e0f00b0a5a2f16617deff3eddfafa8b6e55
	timeout = "short",
	size = "small",
)
