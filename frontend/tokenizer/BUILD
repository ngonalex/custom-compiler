cc_library(
	name = "tokenizer",
	copts = ["-Wall -g"],
	hdrs = ["tokenizer.h"],
        srcs = ["tokenizer.cc"],
	deps = ["//utility"],
	visibility = ["//visibility:public"],
)

cc_test(
	name = "tokenizer_test",
	srcs = ["tokenizer_test.cc"],
	copts = ["-Iexternal/gtest/googletest-release-1.8.0/googletest/include"],
	deps = [
             "@gtest//:main",
             ":tokenizer"
        ],
	timeout = "short",
	size = "small",
)
