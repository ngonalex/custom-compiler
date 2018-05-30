#include "frontend/combinators/v1_combinators/close_paren.h"

#define super NullParser

using namespace cs160::frontend;

ParseStatus CloseParenParser::parse(std::string inputProgram, int startCharacter,
				    std::string errorType) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);
  std::string errorMessage = "Expecting close paranthesis";

  if (inputProgram.size() == 0) {
    return super::parse(inputProgram, endCharacter, errorMessage);
  }

  ParseStatus status;
  if (inputProgram[0] == ')') {
    status.status = true;
    status.remainingCharacters = inputProgram.erase(0, 1);
    status.parsedCharacters = ')';
    status.startCharacter = startCharacter;
    status.endCharacter = endCharacter + 1;
  } else {
    return super::parse(inputProgram, endCharacter, errorMessage);
  }
  return status;
}
