
cc_library(
	name = "ucsb-rcc-dev",
	copts = ["-Wall -g"],
	srcs = glob(['src/*.cc']),
	hdrs = glob(['include/*.h']),
	visibility = ["//test:__pkg__"],
	includes = ["include/"],
)

cc_binary(
	name = "ucsb-rcc",
	copts = ["-Wall -g"],
	deps = [":ucsb-rcc-dev"],
	srcs = glob(['src/*.cc', 'include/*.h']),
	includes = ["include/"],
)

cc_test(
	name = "ucsb-rcc-test",
	deps = ["@gtest//:main", ":ucsb-rcc-dev"],
	srcs = glob(['tests/*.cc']),
        includes = ["include/"],
	timeout = "short", 
	size = "small",
)
