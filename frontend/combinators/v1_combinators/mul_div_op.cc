#include "frontend/combinators/v1_combinators/mul_div_op.h"

#include <string>

#define super NullParser

using namespace cs160::frontend;

ParseStatus MulDivOpParser::parse(std::string inputProgram, int startCharacter,
				  std::string errorType) {
  int endCharacter = startCharacter;
  trim(inputProgram);
  std::string errorMessage = "Expecting * or /";

  if (inputProgram.size() == 0) {
    return super::parse(inputProgram, endCharacter, errorMessage);
  }

  ParseStatus status;

  if (inputProgram[0] == '*' || inputProgram[0] == '/') {
    status.status = true;
    status.parsedCharacters = inputProgram[0];
    status.remainingCharacters = inputProgram.erase(0, 1);
    status.startCharacter = startCharacter;
    status.endCharacter = endCharacter + 1;
  } else {
    return super::parse(inputProgram, endCharacter, errorMessage);
  }
  return status;
}
