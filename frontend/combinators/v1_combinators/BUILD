cc_library(
	name = "v1_combinators",
	copts = ["-Wall -g"],
	hdrs = ["single_char.h", "single_digit.h", "add_sub_op.h", "mul_div_op.h", "ae.h", "close_paren.h", "num_parser.h", "open_paren.h", "add_sub_expr.h", "mul_div_expr.h", "term_expr.h", "negative_sign.h"],
	srcs = ["single_char.cc", "single_digit.cc", "add_sub_op.cc", "mul_div_op.cc", "ae.cc", "close_paren.cc", "num_parser.cc", "open_paren.cc", "add_sub_expr.cc","mul_div_expr.cc", "term_expr.cc", "negative_sign.cc"],
	deps = ["//utility", "//frontend/combinators/basic_combinators", "//abstract_syntax:abstract_syntax",
	"//abstract_syntax:print_visitor_v1"],
	visibility = ["//visibility:public"],
)

cc_test(
	name = "basic_combinators",
	deps = ["@gtest//:main", ":combinators", "//frontend/combinators/basic_combinators", "//abstract_syntax:abstract_syntax"],
	srcs = ["v1_test.cc"],
	copts = ["-Iexternal/gtest/googletest-release-1.8.0/googletest/include"],
	timeout = "short",
	size = "small",
)
