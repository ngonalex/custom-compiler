cc_library(
	name = "token",
	copts = ["-Wall -g"],
	hdrs = ["token.h"],
  srcs = ["token.cc"],
	deps = ["//utility"],
	visibility = ["//visibility:public"],
)

cc_library(
	name = "tokenizer",
	copts = ["-Wall -g"],
	hdrs = ["tokenizer.h"],
  srcs = ["tokenizer.cc"],
	deps = ["//utility", ":token"],
	visibility = ["//visibility:public"],
)

cc_test(
	name = "tokenizer_test",
	deps = ["@gtest//:main", ":token", ":tokenizer"],
	srcs = ["token_test.cc", "tokenizer_test.cc"],
	copts = ["-Iexternal/gtest/googletest-release-1.8.0/googletest/include"],
	timeout = "short",
	size = "small",
)
