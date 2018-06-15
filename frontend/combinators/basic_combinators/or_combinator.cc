#include "frontend/combinators/basic_combinators/or_combinator.h"

#include <iostream>
#define super NullParser

using namespace cs160::frontend;

ParseStatus OrCombinator::do_parse(std::string inputProgram,
                                   int startCharacter) {
  ParseStatus firstStatus = firstParser->do_parse(inputProgram, startCharacter);
  if (firstStatus.status) {
    return firstStatus;
  }

  ParseStatus secondStatus =
      secondParser->do_parse(inputProgram, startCharacter);
  if (!secondStatus.status) {
    secondStatus.errorType += " or " + firstStatus.errorType;
  }
  return secondStatus;
}
