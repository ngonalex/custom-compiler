cc_library(
	name = "v2_combinators",
	copts = ["-Wall -g"],
	hdrs = ["main/word_parser.h", "main/variable_parser.h", "helpers/var_helper.h", "main/assignment_parser.h"],
	srcs = ["main/word_parser.cc", "main/variable_parser.cc", "helpers/var_helper.cc", "main/assignment_parser.cc"],
	deps = ["//utility", "//frontend/combinators/basic_combinators","//frontend/combinators/v1_combinators", "//abstract_syntax:abstract_syntax",
	"//abstract_syntax:print_visitor_v2"],
	visibility = ["//visibility:public"],
)

cc_test(
	name = "combinators_test",
	deps = ["@gtest//:main", ":v2_combinators", "//abstract_syntax:abstract_syntax","//abstract_syntax:print_visitor_v2"],
	srcs = ["v2_test.cc"],
	copts = ["-Iexternal/gtest/googletest-release-1.8.0/googletest/include"],
	timeout = "short",
	size = "small",
)
