cc_library(
	name = "basic_combinators",
	copts = ["-Wall -g"],
	hdrs = ["and_combinator.h", "null.h", "or_combinator.h", "one_or_more_combinator.h", "parsestatus.h", "zero_or_more_combinator.h", "num_parser.h", "single_digit.h"],
	srcs = ["and_combinator.cc", "null.cc", "or_combinator.cc", "one_or_more_combinator.cc", "parsestatus.cc", "zero_or_more_combinator.cc", "num_parser.cc", "single_digit.cc"],
	deps = ["//utility", "//abstract_syntax:abstract_syntax",
	"//abstract_syntax:print_visitor_v1"],
	visibility = ["//visibility:public"],
)

cc_test(
	name = "basic_test",
	deps = ["@gtest//:main", ":basic_combinators"],
	srcs = ["basic_test.cc"],
	copts = ["-Iexternal/gtest/googletest-release-1.8.0/googletest/include"],
	timeout = "short",
	size = "small",
)
