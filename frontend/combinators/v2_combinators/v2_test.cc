#include "abstract_syntax/abstract_syntax.h"
#include "abstract_syntax/print_visitor_v2.h"
#include "frontend/combinators/v2_combinators/helpers/word_parser.h"
#include "frontend/combinators/v2_combinators/helpers/var_helper.h"

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

// Success Case trim in utility
TEST(WordParserCombinator, trimTest) {
  std::string result = "    var    ";
  cs160::trim(result);
  EXPECT_EQ(result, "var");
}

// Success Case var_keyword_parser
TEST(WordParserCombinator, successVarKeywordParser1) {
  VarKeywordParser varParser;
  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "";
  result.parsedCharacters = "var";

  ParseStatus testResult = varParser.parse("   var");

  EXPECT_EQ(testResult, result);
}

// Fail Case var_keyword_parser
TEST(WordParserCombinator, failVarKeywordParser1) {
  VarKeywordParser varParser;
  ParseStatus result;
  result.status = false;

  ParseStatus testResult = varParser.parse("victor");

  EXPECT_EQ(testResult, result);
  EXPECT_EQ(testResult.errorType, "Start variable declaration with var");
}

// Success Case ColonParser
TEST(WordParserCombinator, sucessColonParser1) {
  ColonParser parser;
  ParseStatus result;
  result.status = true;
  result.remainingCharacters = "  joj";
  result.parsedCharacters = ":";

  ParseStatus testResult = parser.parse("   :  joj");

  EXPECT_EQ(testResult, result);
}

// Fail Case ColonParser
TEST(WordParserCombinator, failColonParser1) {
  ColonParser parser;
  ParseStatus result;
  result.status = false;

  ParseStatus testResult = parser.parse("first char not colon:");

  EXPECT_EQ(testResult, result);
  EXPECT_EQ(testResult.errorType, "Missing colon");
}
