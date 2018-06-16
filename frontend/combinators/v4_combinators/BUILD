cc_library(
	name = "v4_combinators",
	copts = ["-Wall -g"],

	hdrs = [
	"helpers/function_helpers.h",
	"main/function_decl_parser.h",
	"main/function_call_parser.h"
	],

	srcs = [
	"helpers/function_helpers.cc",
	"main/function_decl_parser.cc",
	"main/function_call_parser.cc"
	],

	deps = [
	"//utility", 
	"//frontend/combinators/basic_combinators",
	"//frontend/combinators/v1_combinators:v1_combinators",
	"//frontend/combinators/v2_combinators:v2_combinators",
	"//frontend/combinators/v3_combinators:v3_combinators",
	"//abstract_syntax:abstract_syntax"
	],

	visibility = ["//visibility:public"],
)

cc_library(
	name = "super_function_decl",
	copts = ["-Wall -g"],

	hdrs = [
	"helpers/function_helpers.h",
	"main/function_decl_parser.h",
	"main/function_call_parser.h"
	],

	srcs = [
	"helpers/function_helpers.cc",
	"main/function_decl_parser.cc",
	"main/function_call_parser.cc"
	],

	deps = [
	"//utility", 
	"//frontend/combinators/basic_combinators",
	"//frontend/combinators/v1_combinators:v1_combinators",
	"//frontend/combinators/v2_combinators:assignment_parser",
	"//frontend/combinators/v3_combinators:statement_parser",
	"//abstract_syntax:abstract_syntax"
	],

	visibility = ["//visibility:public"],
)

cc_test(
	name = "combinators_test",

	deps = [
	"@gtest//:main", 
	":v4_combinators", 
	"//abstract_syntax:abstract_syntax",
	"//abstract_syntax:print_visitor_v4"
	],
	
	srcs = ["v4_test.cc"],
	copts = ["-Iexternal/gtest/googletest-release-1.8.0/googletest/include"],
	timeout = "short",
	size = "small",
)
