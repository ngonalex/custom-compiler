#include "frontend/combinators/v1_combinators/close_paren.h"

#define super NullParser

using namespace cs160::frontend;

ParseStatus CloseParenParser::parse(std::string inputProgram) {
  if (inputProgram.size() == 0) {
    return super::parse(inputProgram);
  }

  ParseStatus status;
  if (inputProgram[0] == ')') {
    status.status = true;
    status.remainingCharacters = inputProgram.erase(0, 1);
  } else {
    return super::parse(inputProgram);
  }
  return status;
}
