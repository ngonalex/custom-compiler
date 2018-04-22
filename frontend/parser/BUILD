cc_library(
	name = "parser",
	copts = ["-Wall -g"],
	hdrs = ["parser.h"],
	deps = ["//utility", "//frontend/tokenizer", "//abstract_syntax:abstract_syntax", 
	"//abstract_syntax:print_visitor_v1"],
	visibility = ["//visibility:public"],
)

cc_test(
	name = "parser_test",
	deps = ["@gtest//:main", ":parser", "//abstract_syntax:abstract_syntax"],
	srcs = ["parser_test.cc"],
	copts = ["-Iexternal/gtest/googletest-release-1.8.0/googletest/include"],
	timeout = "short",
	size = "small",
)
