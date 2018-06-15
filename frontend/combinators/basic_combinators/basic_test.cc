#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/atom_parser.h"
#include "frontend/combinators/basic_combinators/parsestatus.h"

#include "gtest/gtest.h"

using namespace cs160::frontend;

TEST(AtomParser, AtomParserSucess) {
  auto parser = AtomParser('a');
  auto result = parser.parse("af", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 1);
  EXPECT_EQ(result.remainingCharacters, "f");
  EXPECT_EQ(result.parsedCharacters, "a");
}

TEST(AtomParser, AtomParserParen) {
  auto parser = AtomParser('(');
  auto result = parser.parse("(", 0);

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.startCharacter, 0);
  EXPECT_EQ(result.endCharacter, 1);
  EXPECT_EQ(result.remainingCharacters, "");
  EXPECT_EQ(result.parsedCharacters, "(");
}