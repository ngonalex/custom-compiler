
cc_library(
	name = "tokenizer",
	copts = ["-Wall -g"],
	hdrs = ["tokenizer.h"],
	visibility = ["//public:__pkg__"],
)

cc_binary(
	name = "compiler",
	copts = ["-Wall -g"],
	deps = ["//rcc:rcc", "//abstract_syntax:abstract_syntax"],
	srcs = ["main.cc"],
)
