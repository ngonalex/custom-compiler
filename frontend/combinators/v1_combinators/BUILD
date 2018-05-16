cc_library(
	name = "combinators",
	copts = ["-Wall -g"],
	hdrs = ["single_digit.h", 
			"num_parser.h",
			"single_char.h",
			"add_sub_op.h"],
	srcs = ["single_digit.cc", 
			"num_parser.cc",
			"single_char.cc",
			"add_sub_op.cc"],
	deps = ["//utility", "//frontend/combinators/basic_combinators", "//abstract_syntax:abstract_syntax",
	"//abstract_syntax:print_visitor_v1"],
	visibility = ["//visibility:public"],
)

cc_test(
	name = "combinators_test",
	deps = ["@gtest//:main", ":combinators", "//frontend/combinators/basic_combinators", "//abstract_syntax:abstract_syntax"],
	srcs = ["v1_test.cc"],
	copts = ["-Iexternal/gtest/googletest-release-1.8.0/googletest/include"],
	timeout = "short",
	size = "small",
)
