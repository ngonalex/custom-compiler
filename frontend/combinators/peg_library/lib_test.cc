#include "frontend/combinators/peg_library/peg_library.h"
#include "gtest/gtest.h"

using namespace cs160::frontend;

// Fail Case WordParser
TEST(AtomParser, atomParserSuccess) {
  AtomParser atomParser('h');
  ParseResult<AstNode> result = atomParser.parse("hello world");

  EXPECT_EQ(result.status, true);
}

TEST(AtomParser, atomParserFailure) {
  AtomParser atomParser('f');
  ParseResult<AstNode> result = atomParser.parse("hello world");

  EXPECT_EQ(result.status, false);
}

TEST(AtomParser, atomParserFailure) {
  AtomParser atomParser('');
  ParseResult<AstNode> result = atomParser.parse("hello world");

  EXPECT_EQ(result.status, true);
}