#include "abstract_syntax/abstract_syntax.h"
#include "abstract_syntax/print_visitor_v2.h"
#include "frontend/combinators/v2_combinators/helpers/word_parser.h"

#include "gtest/gtest.h"

using namespace cs160::frontend;

// Success case for Single Character test :: single_char.cc
TEST(WordParserCombinator, failWordParser1) {
  WordParser wordParser;
  ParseStatus result;
  result.status = false;

  ParseStatus testResult = wordParser.parse("11fab");

  EXPECT_EQ(testResult, result);
}
/*

Fail:
“11fab” -> ParseStatus(successStatus = false, remainingCharacters = “1fab”)
“()qam” -> ParseStatus(successStatus = false, remainingCharacters = “()qam”)
“__qam” -> ParseStatus(successStatus = false, remainingCharacters = “__qam”)
“&&qam” -> ParseStatus(successStatus = false, remainingCharacters = “&&qam”)
“%%qam” -> ParseStatus(successStatus = false, remainingCharacters = “%%qam”)
“123victor” -> ParseStatus(successStatus = false, remainingCharacters = “123victor”)

Success:
“var” -> ParseStatus(succesStatus = true, remainingCharacters = “”, parsedCharacters=“var”)
“var;35345345” -> ParseStatus(successStatus = true, remainingCharacters = “;35345345” parsedCharacters=“var”)
“Var abcd 42+30” -> ParseStatus(successStatus = true, remainingCharacters = “ abcd 42+30”, parsedCharacters = “var”)
“_victor” -> ParseStatus(successStatus = true, remainingCharacters = “”, parsedCharacters = “_victor”)
“var35345345” -> ParseStatus(successStatus = true, remainingCharacters = “” parsedCharacters=“var35345345”)
*/
