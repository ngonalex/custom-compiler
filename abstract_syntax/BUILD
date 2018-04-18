cc_library(
	name = "abstract_syntax",
	copts = ["-Wall -g"],
	hdrs = ["abstract_syntax.h"],
	deps = [
             ":abstract_syntax_tree_v1",
             ":abstract_syntax_tree_v2",
             "//utility"
        ],
	visibility = ["//visibility:public"],
)

cc_library(
	name = "abstract_syntax_tree_v1",
	copts = ["-Wall -g"],
	hdrs = ["abstract_syntax_tree_v1.h"],
	deps = ["//utility"],
	visibility = ["//visibility:public"],
)

cc_library(
	name = "abstract_syntax_tree_v2",
	copts = ["-Wall -g"],
	hdrs = ["abstract_syntax_tree_v2.h"],
	deps = ["//utility"],
	visibility = ["//visibility:public"],
)

cc_library(
        name = "print_visitor_v1",
        copts = ["-Wall -g"],
        hdrs = ["print_visitor_v1.h"],
        deps = [
             ":abstract_syntax_tree_v1",
             "//utility"
        ],
        visibility = ["//visibility:public"],
)

cc_test(
	name = "abstract_syntax_test",
	deps = ["@gtest//:main", ":abstract_syntax", ":print_visitor_v1"],
	srcs = [
		"print_visitor_v1_test.cc",
	],
	copts = ["-Iexternal/gtest/googletest-release-1.8.0/googletest/include"],
	timeout = "short",
	size = "small",
)
