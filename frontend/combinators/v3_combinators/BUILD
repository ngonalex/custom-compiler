cc_library(
	name = "v3_combinators",
	copts = ["-Wall -g"],

	hdrs = [
	"helpers/relational_helper.h", 
	"main/relation_body.h",
	"main/relation_not.h",
	"main/or_relation_parser.h",
	"main/and_relation_parser.h",
	"main/block_parser.h",
	"main/conditional_parser.h",
	"main/loop_parser.h",
	"main/statement_parser.h",
	],

	srcs = [
	"helpers/relational_helper.cc",
	"main/relation_body.cc",
	"main/relation_not.cc",
	"main/or_relation_parser.cc",
	"main/and_relation_parser.cc",
	"main/block_parser.cc",
	"main/conditional_parser.cc",
	"main/loop_parser.cc",
	"main/statement_parser.cc",
	],

	deps = [
	"//utility", 
	"//frontend/combinators/basic_combinators",
	"//frontend/combinators/v1_combinators:v1_combinators",
	"//frontend/combinators/v2_combinators:v2_combinators",
	"//abstract_syntax:abstract_syntax",
	"//abstract_syntax:print_visitor_v3"
	],

	visibility = ["//visibility:public"],
)

cc_test(
	name = "combinators_test",

	deps = [
	"@gtest//:main", 
	":v3_combinators", 
	"//abstract_syntax:abstract_syntax",
	"//abstract_syntax:print_visitor_v2"
	],
	
	srcs = ["v3_test.cc"],
	copts = ["-Iexternal/gtest/googletest-release-1.8.0/googletest/include"],
	timeout = "short",
	size = "small",
)
