#include "frontend/combinators/basic_combinators/and_combinator.h"

#define super NullParser

using namespace cs160::frontend;

ParseStatus AtomParser::parse(std::string inputProgram, std::string errorType) {
  if (input.size() == 0 || input[0] != char_to_parse_) {
    ParseStatus result;
    result.errorType = "Failed to parse";
    result.return ParseStatus::("Failed to parse");
  }
  return ParseResult<AstNode>::success();

  ParseStatus secondStatus =
      secondParser->parse(firstStatus.remainingCharacters);

  if (!secondStatus.status) {
    return secondStatus;
  }

  ParseStatus both;
  both.status = true;
  both.parsedCharacters =
      firstStatus.parsedCharacters + secondStatus.parsedCharacters;
  both.remainingCharacters = secondStatus.remainingCharacters;
  both.ast = std::move(firstStatus.ast);
  both.second_ast = std::move(secondStatus.ast);

  return both;
}

// NUM and Operator
//
