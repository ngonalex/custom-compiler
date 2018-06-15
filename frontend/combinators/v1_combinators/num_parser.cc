#include "frontend/combinators/v1_combinators/num_parser.h"
#include "frontend/combinators/basic_combinators/one_or_more_combinator.h"
#include "frontend/combinators/v1_combinators/single_digit.h"

#include <string>

#define super NullParser

using namespace cs160::frontend;
using namespace std;

ParseStatus NumParser::do_parse(std::string inputProgram, int startCharacter) {
  int endCharacter = startCharacter;
  std::string errorMessage = "Number should only have digits (0-9)";

  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter, errorMessage);
  }

  SingleDigitParser digitParser;
  OneOrMoreCombinator oneOrMore;

  oneOrMore.parser = reinterpret_cast<NullParser *>(&digitParser);  // Feels bad
  // Parse the integer
  ParseStatus result = oneOrMore.do_parse(inputProgram, endCharacter);
  if (result.status) {
    // Make an Integer expression
    result.ast =
        std::move(make_unique<IntegerExpr>(stoi(result.parsedCharacters)));
  } else {
    return super::fail(inputProgram, endCharacter, errorMessage);
  }

  result.parsedCharactersArray.erase(std::begin(result.parsedCharactersArray),
                                     std::end(result.parsedCharactersArray));
  return result;
}
