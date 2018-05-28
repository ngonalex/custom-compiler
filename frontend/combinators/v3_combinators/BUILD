cc_library(
	name = "v3_combinators",
	copts = ["-Wall -g"],
	hdrs = ["helpers/equal_to_op_parser.h", "helpers/greater_than_or_equal_op_parser.h", "helpers/greater_than_op_parser.h", "helpers/less_than_or_equal_op_parser.h", "helpers/less_than_op_parser.h"],
	srcs = ["helpers/equal_to_op_parser.cc", "helpers/greater_than_or_equal_op_parser.cc", "helpers/greater_than_op_parser.cc", "helpers/less_than_or_equal_op_parser.cc", "helpers/less_than_op_parser.cc"],
	deps = ["//utility", "//frontend/combinators/basic_combinators","//frontend/combinators/v1_combinators", "//abstract_syntax:abstract_syntax",
	"//abstract_syntax:print_visitor_v2"],
	visibility = ["//visibility:public"],
)

cc_test(
	name = "combinators_test",
	deps = ["@gtest//:main", ":v2_combinators", "//abstract_syntax:abstract_syntax","//abstract_syntax:print_visitor_v2"],
	srcs = ["v3_test.cc"],
	copts = ["-Iexternal/gtest/googletest-release-1.8.0/googletest/include"],
	timeout = "short",
	size = "small",
)
