cc_library(
	name = "basic_combinators",
	copts = ["-Wall -g"],
	hdrs = ["and_combinator.h", "null.h", "or_combinator.h", "one_or_more_combinator.h", "parsestatus.h", "zero_or_more_combinator.h"],
	srcs = ["and_combinator.cc", "null.cc", "or_combinator.cc", "one_or_more_combinator.cc", "parsestatus.cc", "zero_or_more_combinator.cc"],
	deps = ["//utility", "//abstract_syntax:abstract_syntax", "//abstract_syntax:print_visitor_v1"],
	visibility = ["//visibility:public"],
)
