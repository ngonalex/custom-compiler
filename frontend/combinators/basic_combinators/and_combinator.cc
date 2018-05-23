#include "frontend/combinators/basic_combinators/and_combinator.h"

#define super NullParser

using namespace cs160::frontend;

ParseStatus AndCombinator::parse(std::string inputProgram) {
  ParseStatus firstStatus = firstParser->parse(inputProgram);

  if (!firstStatus.status) {
    return firstStatus;
  }

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