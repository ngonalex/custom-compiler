#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/parsestatus.h"
#include "frontend/combinators/basic_combinators/atom_parser.h"

#include "gtest/gtest.h"

using namespace cs160::frontend;

// Success case for Single Character test :: single_char.cc
TEST(ParseStatus, NonAstSucess) {
  /*
  auto result = ParseStatus<const AstNode>::success(0, 4, "af", "10");

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.character_start, 0);
  EXPECT_EQ(result.character_end, 4 );
  EXPECT_EQ(result.remaining_chars, "af");
  EXPECT_EQ(result.parsed_chars, "10");
  EXPECT_EQ(result.error_type, "No Error");
  EXPECT_EQ(result.ast_result, nullptr);*/
}

TEST(AtomParser, AtomParserSucess) {
  auto parser = AtomParser('a');
  auto result = parser.parse("af", 0, "No Error");

  EXPECT_EQ(result.status, true);
  EXPECT_EQ(result.character_start, 0);
  EXPECT_EQ(result.character_end, 1);
  EXPECT_EQ(result.remaining_chars, "f");
  EXPECT_EQ(result.parsed_chars, "a");
  EXPECT_EQ(result.error_type, "No Error");
  EXPECT_EQ(result.ast_result, nullptr);
}