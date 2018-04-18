cc_library(
	name = "parser",
	copts = ["-Wall -g"],
	hdrs = ["parser.h"],
  srcs = ["parser.cc"],
	deps = ["//utility", "//frontend/tokenizer"],
	visibility = ["//visibility:public"],
)

cc_test(
	name = "parser_test",
	deps = ["@gtest//:main", "parser"],
	srcs = ["parser_test.cc"],
	copts = ["-Iexternal/gtest/googletest-release-1.8.0/googletest/include"],
	timeout = "short",
	size = "small",
)
