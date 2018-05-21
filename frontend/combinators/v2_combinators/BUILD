cc_library(
	name = "v2_combinators",
	copts = ["-Wall -g"],
	hdrs = ["helpers/word_parser.h", "main/variable_parser.h"],
	srcs = ["helpers/word_parser.cc", "main/variable_parser.cc"],
	deps = ["//utility", "//frontend/combinators/basic_combinators","//frontend/combinators/v1_combinators", "//abstract_syntax:abstract_syntax",
	"//abstract_syntax:print_visitor_v1"],
	visibility = ["//visibility:public"],
)

cc_test(
	name = "combinators_test",
	deps = ["@gtest//:main", ":combinators", "//abstract_syntax:abstract_syntax"],
	srcs = ["v2_test.cc"],
	copts = ["-Iexternal/gtest/googletest-release-1.8.0/googletest/include"],
	timeout = "short",
	size = "small",
)
