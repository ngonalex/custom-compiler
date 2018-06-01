#include "frontend/combinators/v1_combinators/single_char.h"

#define super NullParser

using namespace cs160::frontend;

ParseStatus<const AstNode> SingleCharParser::parse(std::string inputProgram, int startCharacter,
				    std::string errorType) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::parse(inputProgram, endCharacter);
  }

  ParseStatus status;
  if ((inputProgram[0] >= 'a' && inputProgram[0] <= 'z') ||
      (inputProgram[0] >= 'A' && inputProgram[0] <= 'Z')) {
    status.status = true;
    status.parsedCharacters = inputProgram[0];
    status.remainingCharacters = inputProgram.erase(0, 1);
    status.startCharacter = startCharacter;
    status.endCharacter = endCharacter + 1;
  } else {
    return super::parse(inputProgram, endCharacter,
			"Char should only have alphabetical character");
  }
  return status;
}

ParseStatus<const AstNode> SingleVarCharParser::parse(std::string inputProgram,
				       std::string errorType) {
  std::string errorMessage = "Char should be alphabetical or underscore";

  int endCharacter = startCharacter;
  trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::parse(inputProgram, endCharacter, errorMessage);
  }

  ParseStatus status;
  if ((inputProgram[0] >= 'a' && inputProgram[0] <= 'z') ||
      (inputProgram[0] >= 'A' && inputProgram[0] <= 'Z') ||
      inputProgram[0] == '_') {
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
