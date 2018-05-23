#include "gtest/gtest.h"
#include "preparser.h"

TEST(PreParser, WhitespaceTest) {
	std::string program = preparse_Program("( 225 * 335   ) + 12 /  2; 3 + 5 * 6");
	EXPECT_EQ(program, "(225*335)+12/2;3+5*6");
}