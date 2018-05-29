cc_library(
	name = "v3_combinators",
	copts = ["-Wall -g"],
	hdrs = ["helpers/relational_helper.h"],
	srcs = ["helpers/relational_helper.cc"],
	deps = ["//utility", "//frontend/combinators/basic_combinators","//frontend/combinators/v1_combinators", "//frontend/combinators/v2_combinators", "//abstract_syntax:abstract_syntax",
	"//abstract_syntax:print_visitor_v2"],
	visibility = ["//visibility:public"],
)

cc_test(
	name = "combinators_test",
	deps = ["@gtest//:main", ":v3_combinators", "//abstract_syntax:abstract_syntax","//abstract_syntax:print_visitor_v2"],
	srcs = ["v3_test.cc"],
	copts = ["-Iexternal/gtest/googletest-release-1.8.0/googletest/include"],
	timeout = "short",
	size = "small",
)
