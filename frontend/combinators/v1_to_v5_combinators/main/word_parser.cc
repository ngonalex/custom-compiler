#include "frontend/combinators/v1_to_v5_combinators/main/word_parser.h"
#include <string>  // std::string, std::stoi
#include "frontend/combinators/basic_combinators/and_combinator.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"
#include "frontend/combinators/basic_combinators/zero_or_more_combinator.h"
#include "frontend/combinators/v1_to_v5_combinators/main/single_char.h"
#include "frontend/combinators/v1_to_v5_combinators/main/single_digit.h"

#define super NullParser

namespace cs160 {
namespace frontend {

ParseStatus WordParser::do_parse(std::string inputProgram, int startCharacter) {
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

  ParseStatus result = firstAnd.do_parse(inputProgram, endCharacter);

  // Can't have these keywords b/c of backend
  // https://github.ucsb.edu/CS160-S18/team-influx/issues/36
  if (result.parsedCharacters == "heap" ||
      result.parsedCharacters == "bumpptr" ||
      result.parsedCharacters == "returnobj") {
    return super::fail(inputProgram, endCharacter);
  }

  if (result.status) {
    result.ast =
        std::move(make_unique<const VariableExpr>(result.parsedCharacters));
  } else {
    result.errorType = errorMessage;
  }

  result.parsedCharactersArray.erase(std::begin(result.parsedCharactersArray),
                                     std::end(result.parsedCharactersArray));

  return result;
}

}  // namespace frontend
}  // namespace cs160
