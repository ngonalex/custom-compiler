cc_library(
	name = "v2_combinators",

	copts = ["-Wall -g"],

	hdrs = [
	"main/program_parser.h", 
	"main/word_parser.h", 
	"main/variable_parser.h", 
	"helpers/var_helper.h", 
	"main/assignment_parser.h"
	],

	srcs = [
	"main/program_parser.cc", 
	"main/word_parser.cc", 
	"main/variable_parser.cc", 
	"helpers/var_helper.cc", 
	"main/assignment_parser.cc"
	],

	deps = [
	"//utility", 
	"//frontend/combinators/basic_combinators",
	"//frontend/combinators/v1_combinators", 
	"//abstract_syntax:abstract_syntax",
	"//abstract_syntax:print_visitor_v3",
	"//frontend/combinators/v3_combinators:statement_parser"
	],

	visibility = ["//visibility:public"],
)

cc_library(
	name = "word_parser",
	copts = ["-Wall -g"],
	hdrs = ["main/word_parser.h"],
	srcs = ["main/word_parser.cc"],
	deps = ["//utility", "//frontend/combinators/v1_combinators:single_char", "//frontend/combinators/v1_combinators:single_digit", "//frontend/combinators/basic_combinators"],
	visibility = ["//visibility:public"],
)

cc_library(
	name = "assignment_parser",
	copts = ["-Wall -g"],
	hdrs = ["main/assignment_parser.h","helpers/var_helper.h" ],
	srcs = ["main/assignment_parser.cc", "helpers/var_helper.cc"],
	deps = ["//utility", "//frontend/combinators/v1_combinators"],
	visibility = ["//visibility:public"],
)

cc_test(
	name = "combinators_test",

	deps = [
	"@gtest//:main", 
	":v2_combinators", 
	"//abstract_syntax:abstract_syntax",
	"//abstract_syntax:print_visitor_v3"
	],

	srcs = ["v2_test.cc"],

	copts = ["-Iexternal/gtest/googletest-release-1.8.0/googletest/include"],
	
	timeout = "short",
	
	size = "small",
)
