cc_library(
	name = "abstract_syntax",
	copts = ["-Wall -g"],
	hdrs = ["abstract_syntax.h"],
	deps = [
             ":abstract_syntax_tree_v1",
             ":abstract_syntax_tree_v2",
             ":abstract_syntax_tree_v3",
             ":abstract_syntax_tree_v4",
             ":abstract_syntax_tree_v5",
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
	name = "abstract_syntax_tree_v3",
	copts = ["-Wall -g"],
	hdrs = ["abstract_syntax_tree_v3.h"],
	deps = ["//utility"],
	visibility = ["//visibility:public"],
)

cc_library(
	name = "abstract_syntax_tree_v4",
	copts = ["-Wall -g"],
	hdrs = ["abstract_syntax_tree_v4.h"],
	deps = ["//utility"],
	visibility = ["//visibility:public"],
)

cc_library(
	name = "abstract_syntax_tree_v5",
	copts = ["-Wall -g"],
	hdrs = ["abstract_syntax_tree_v5.h"],
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

cc_library(
        name = "print_visitor_v2",
        copts = ["-Wall -g"],
        hdrs = ["print_visitor_v2.h"],
        deps = [
             ":abstract_syntax_tree_v2",
             "//utility"
        ],
        visibility = ["//visibility:public"],
)

cc_library(
        name = "print_visitor_v3",
        copts = ["-Wall -g"],
        hdrs = ["print_visitor_v3.h"],
        deps = [
             ":abstract_syntax_tree_v3",
             "//utility"
        ],
        visibility = ["//visibility:public"],
)

cc_library(
        name = "print_visitor_v4",
        copts = ["-Wall -g"],
        hdrs = ["print_visitor_v4.h"],
        deps = [
             ":abstract_syntax_tree_v4",
             "//utility"
        ],
        visibility = ["//visibility:public"],
)


cc_test(
        name = "abstract_syntax_tree_v1_test",
        srcs = ["abstract_syntax_tree_v1_test.cc"],
        deps = [
             ":abstract_syntax_tree_v1",
             "//utility",
             "@gtest//:main",
        ],
        timeout = "short",
        size = "small",
)

cc_test(
        name = "abstract_syntax_tree_v2_test",
        srcs = ["abstract_syntax_tree_v2_test.cc"],
        deps = [
             ":abstract_syntax_tree_v2",
             "//utility",
             "@gtest//:main",
        ],
        timeout = "short",
        size = "small",
)

cc_test(
        name = "abstract_syntax_tree_v3_test",
        srcs = ["abstract_syntax_tree_v3_test.cc"],
        deps = [
             ":abstract_syntax_tree_v3",
             "//utility",
             "@gtest//:main",
        ],
        timeout = "short",
        size = "small",
)

cc_test(
        name = "abstract_syntax_tree_v4_test",
        srcs = ["abstract_syntax_tree_v4_test.cc"],
        deps = [
             ":abstract_syntax_tree_v4",
             "//utility",
             "@gtest//:main",
        ],
        timeout = "short",
        size = "small",
)

cc_test(
        name = "abstract_syntax_tree_v5_test",
        srcs = ["abstract_syntax_tree_v5_test.cc"],
        deps = [
             ":abstract_syntax_tree_v5",
             "//utility",
             "@gtest//:main",
        ],
        timeout = "short",
        size = "small",
)

cc_test(
	name = "print_visitor_v1_test",
	srcs = ["print_visitor_v1_test.cc"],
	deps = [
             ":print_visitor_v1",
             "@gtest//:main",
        ],
	timeout = "short",
	size = "small",
)

cc_test(
	name = "print_visitor_v2_test",
	srcs = ["print_visitor_v2_test.cc"],
	deps = [
             ":print_visitor_v2",
             "@gtest//:main",
        ],
	timeout = "short",
	size = "small",
)