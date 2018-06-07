cc_library(
	name = "basic_combinators",
	copts = ["-Wall -g"],
	hdrs = [
	"parsestatus.h",
	"atom_parser.h", 
	"null.h", 
	"zero_or_more_combinator.h",
	"one_or_more_combinator.h", 
	"and_combinator.h",
	"or_combinator.h"
	],
	srcs = [
	"parsestatus.cc",
	"atom_parser.cc", 
	"null.cc",
	"zero_or_more_combinator.cc", 
	"one_or_more_combinator.cc", 
	"and_combinator.cc",
	"or_combinator.cc"
	],
	deps = ["//utility", "//abstract_syntax:abstract_syntax", "//abstract_syntax:print_visitor_v1"],
	visibility = ["//visibility:public"],
)

cc_test(
	name = "combinators_test",
	deps = ["@gtest//:main", ":basic_combinators", "//abstract_syntax:abstract_syntax"],
	srcs = ["basic_test.cc"],
	copts = ["-Iexternal/gtest/googletest-release-1.8.0/googletest/include"],
	timeout = "short",
	size = "small",
)