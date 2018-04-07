
cc_library(
	name = "rcc",
	copts = ["-Wall -g"],
	srcs = ["rcc.cc"],
	hdrs = ["rcc.h"],
	visibility = ["//public:__pkg__"],
)

cc_test(
	name = "rcc_test",
	deps = ["@gtest//:main", ":rcc"],
	srcs = ["rcc_test.cc"],
	timeout = "short", 
	size = "small",
)
