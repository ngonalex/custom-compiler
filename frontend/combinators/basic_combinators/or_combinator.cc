#include "frontend/combinators/basic_combinators/or_combinator.h"

#define super NullParser

namespace cs160 {
namespace frontend {

ParseStatus OrCombinator::do_parse(std::string inputProgram,
                                   int startCharacter) {
  ParseStatus firstStatus = firstParser->do_parse(inputProgram, startCharacter);
  if (firstStatus.status) {
    firstStatus.firstOrSecond = true;
    return firstStatus;
  }

  ParseStatus secondStatus =
      secondParser->do_parse(inputProgram, startCharacter);
  if (!secondStatus.status) {
    secondStatus.firstOrSecond = false;
    secondStatus.errorType += " or " + firstStatus.errorType;
  }
  return secondStatus;
}

}  // namespace frontend
}  // namespace cs160