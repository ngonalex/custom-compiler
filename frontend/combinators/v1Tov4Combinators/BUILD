cc_library(
	name = "v2to4Combinators",
	copts = ["-Wall -g"],

	hdrs = [
	"helpers/relational_helper.h", 
	"helpers/function_helpers.h", 
	"main/function_call_parser.h",
	"main/function_decl_parser.h", 
	"main/relation_body.h",
	"main/relation_not.h",
	"main/or_relation_parser.h",
	"main/and_relation_parser.h",
	"main/block_parser.h",
	"main/conditional_parser.h",
	"main/loop_parser.h",
	"main/statement_parser.h",
		"main/program_parser.h", 
	"main/word_parser.h", 
	"main/variable_parser.h", 
	"helpers/var_helper.h", 
	"main/assignment_parser.h",
		"helpers/v1_helpers.h",
	"main/single_digit.h",
	"main/num_parser.h",
	"main/single_char.h",
	"main/term_expr.h",
	"main/add_sub_expr.h",
	"main/mul_div_expr.h",
	"main/ae.h"
	],

	srcs = [
	"helpers/relational_helper.cc",
	"helpers/function_helpers.cc",
	"main/function_call_parser.cc",
	"main/function_decl_parser.cc", 
	"main/relation_body.cc",
	"main/relation_not.cc",
	"main/or_relation_parser.cc",
	"main/and_relation_parser.cc",
	"main/block_parser.cc",
	"main/conditional_parser.cc",
	"main/loop_parser.cc",
	"main/statement_parser.cc",
		"main/program_parser.cc", 
	"main/word_parser.cc", 
	"main/variable_parser.cc", 
	"helpers/var_helper.cc", 
	"main/assignment_parser.cc",
		"helpers/v1_helpers.cc",
	"main/single_digit.cc",
	"main/num_parser.cc",
	"main/single_char.cc",
	"main/term_expr.cc",
	"main/add_sub_expr.cc",
	"main/mul_div_expr.cc",
	"main/ae.cc"
	],

	deps = [
	"//utility", 
	"//frontend/combinators/basic_combinators",
	"//abstract_syntax:abstract_syntax",
	"//abstract_syntax:print_visitor_v4"
	],

	visibility = ["//visibility:public"],
)

cc_test(
	name = "v1_tests",

	deps = [
	"@gtest//:main", 
	":v2to4Combinators", 
	"//abstract_syntax:abstract_syntax",
	"//abstract_syntax:print_visitor_v4"
	],

	srcs = ["v1_test.cc"],

	copts = ["-Iexternal/gtest/googletest-release-1.8.0/googletest/include"],
	
	timeout = "short",
	
	size = "small",
)

cc_test(
	name = "v2_tests",

	deps = [
	"@gtest//:main", 
	":v2to4Combinators", 
	"//abstract_syntax:abstract_syntax",
	"//abstract_syntax:print_visitor_v4"
	],

	srcs = ["v2_test.cc"],

	copts = ["-Iexternal/gtest/googletest-release-1.8.0/googletest/include"],
	
	timeout = "short",
	
	size = "small",
)

cc_test(
	name = "v3_tests",

	deps = [
	"@gtest//:main", 
	":v2to4Combinators", 
	"//abstract_syntax:abstract_syntax",
	"//abstract_syntax:print_visitor_v4"
	],
	
	srcs = ["v3_test.cc"],
	copts = ["-Iexternal/gtest/googletest-release-1.8.0/googletest/include"],
	timeout = "short",
	size = "small",
)

cc_test(
	name = "v4_tests",

	deps = [
	"@gtest//:main", 
	":v2to4Combinators", 
	"//abstract_syntax:abstract_syntax",
	"//abstract_syntax:print_visitor_v4"
	],
	
	srcs = ["v4_test.cc"],
	copts = ["-Iexternal/gtest/googletest-release-1.8.0/googletest/include"],
	timeout = "short",
	size = "small",
)
