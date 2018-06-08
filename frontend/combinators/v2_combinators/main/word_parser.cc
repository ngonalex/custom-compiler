#include "frontend/combinators/v2_combinators/main/word_parser.h"
#include <string>  // std::string, std::stoi
#include "frontend/combinators/basic_combinators/and_combinator.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"
#include "frontend/combinators/basic_combinators/zero_or_more_combinator.h"
#include "frontend/combinators/v1_combinators/single_char.h"
#include "frontend/combinators/v1_combinators/single_digit.h"

#define super NullParser

using namespace cs160::frontend;
using namespace std;

ParseStatus WordParser::parse(std::string inputProgram, int startCharacter) {
	int endCharacter = startCharacter;
  	endCharacter += trim(inputProgram);

  std::string errorMessage =
      "Declare variable names with 'var variable_name : type = expression'";

	if (inputProgram.size() == 0) {
		return super::fail(inputProgram, endCharacter);
	}

  SingleVarCharParser charParser;
  SingleDigitParser digitParser;

  OrCombinator charOrDigit;
  charOrDigit.firstParser = reinterpret_cast<NullParser *>(&charParser);
  charOrDigit.secondParser = reinterpret_cast<NullParser *>(&digitParser);

  ZeroOrMoreCombinator zeroOrMore;
  zeroOrMore.parser = reinterpret_cast<NullParser *>(&charOrDigit);

  AndCombinator firstAnd;
  firstAnd.firstParser = reinterpret_cast<NullParser *>(&charParser);
  firstAnd.secondParser = reinterpret_cast<NullParser *>(&zeroOrMore);

  ParseStatus result = firstAnd.parse(inputProgram, endCharacter);

  if (result.status) {
    result.ast =
        std::move(make_unique<const VariableExpr>(result.parsedCharacters));
  } else {

    if (result.parsedCharacters == "heap" || result.parsedCharacters == "bumpptr" || 
        result.parsedCharacters == "returnobj") {
          
      errorMessage = "heap, bumpptr and returnobj are reserved keywords";
    }
    // Error type returned to user
    result.errorType = errorMessage;
  }

  return result;
}
