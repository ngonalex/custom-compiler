#include "abstract_syntax/abstract_syntax.h"
#include "abstract_syntax/print_visitor_v2.h"
#include "frontend/combinators/v2_combinators/helpers/word_parser.h"

#include "gtest/gtest.h"

using namespace cs160::frontend;

// Fail Case WordParser
TEST(WordParserCombinator, failWordParser1) {
  WordParser wordParser;
  ParseStatus result;
  result.status = false;
  result.remainingCharacters = "11fab";

  ParseStatus testResult = wordParser.parse("11fab");

  EXPECT_EQ(testResult, result);
}

// Fail Case WordParser
TEST(WordParserCombinator, failWordParser2) {
  WordParser wordParser;
  ParseStatus result;
  result.status = false;
  result.remainingCharacters = "()qam";

  ParseStatus testResult = wordParser.parse("()qam");

  EXPECT_EQ(testResult, result);
}

// Fail Case WordParser
TEST(WordParserCombinator, failWordParser4) {
  WordParser wordParser;
  ParseStatus result;
  result.status = false;
  result.remainingCharacters = "&&qam";

  ParseStatus testResult = wordParser.parse("&&qam");

  EXPECT_EQ(testResult, result);
}
// Fail Case WordParser
TEST(WordParserCombinator, failWordParser5) {
  WordParser wordParser;
  ParseStatus result;
  result.status = false;
  result.remainingCharacters = "%%qam";

  ParseStatus testResult = wordParser.parse("%%qam");

  EXPECT_EQ(testResult, result);
}

// Fail Case WordParser
TEST(WordParserCombinator, failWordParser6) {
  WordParser wordParser;
  ParseStatus result;
  result.status = false;
  result.remainingCharacters = "123victor";

  ParseStatus testResult = wordParser.parse("123victor");

  EXPECT_EQ(testResult, result);
}

// Success Case WordParser
TEST(WordParserCombinator, successWordParser1) {
  WordParser wordParser;
  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "";
  result.parsedCharacters = "var";

  ParseStatus testResult = wordParser.parse("var");

  EXPECT_EQ(testResult, result);
}

// Success Case WordParser
TEST(WordParserCombinator, successWordParser2) {
  WordParser wordParser;
  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "";
  result.parsedCharacters = "var;35345345";

  ParseStatus testResult = wordParser.parse("var;35345345");

  EXPECT_EQ(testResult.status, result.status);
}

// Success Case WordParser
TEST(WordParserCombinator, successWordParser3) {
  WordParser wordParser;
  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "";
  result.parsedCharacters = "_victor";

  ParseStatus testResult = wordParser.parse("_victor");

  EXPECT_EQ(testResult, result);
}
