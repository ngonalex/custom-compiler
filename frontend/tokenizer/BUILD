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
	deps = [
             "@gtest//:main",
             ":tokenizer"
        ],
	timeout = "short",
	size = "small",
)
