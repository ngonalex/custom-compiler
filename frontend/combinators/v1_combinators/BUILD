cc_library(
	name = "v1_combinators",
	copts = ["-Wall -g"],
	hdrs = ["helpers/v1_helpers.h",
					"single_digit.h",
					"num_parser.h",
					"single_char.h",
					"term_expr.h",
					"add_sub_expr.h",
					"mul_div_expr.h",
					"ae.h"],
	srcs = ["helpers/v1_helpers.cc",
					"single_digit.cc",
					"num_parser.cc",
					"single_char.cc",
					"term_expr.cc",
					"add_sub_expr.cc",
					"mul_div_expr.cc",
					"ae.cc"],
	deps = ["//utility", "//frontend/combinators/v2_combinators:word_parser", "//frontend/combinators/basic_combinators", "//abstract_syntax:abstract_syntax",
	"//abstract_syntax:print_visitor_v2"],
	visibility = ["//visibility:public"],
)

cc_library(
	name = "single_char",
	copts = ["-Wall -g"],
	hdrs = ["single_char.h"],
	srcs = ["single_char.cc"],
	deps = ["//utility", "//frontend/combinators/basic_combinators"],
	visibility = ["//visibility:public"],
)


cc_library(
	name = "single_digit",
	copts = ["-Wall -g"],
	hdrs = ["single_digit.h"],
	srcs = ["single_digit.cc"],
	deps = ["//utility", "//frontend/combinators/basic_combinators"],
	visibility = ["//visibility:public"],
)

cc_test(
	name = "combinators_test",
	deps = ["@gtest//:main", ":v1_combinators", "//frontend/combinators/basic_combinators", "//abstract_syntax:abstract_syntax"],
	srcs = ["v1_test.cc"],
	copts = ["-Iexternal/gtest/googletest-release-1.8.0/googletest/include"],
	timeout = "short",
	size = "small",
)
