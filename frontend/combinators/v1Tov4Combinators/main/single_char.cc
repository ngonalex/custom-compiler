#include "frontend/combinators/v1Tov4Combinators/main/single_char.h"

#define super NullParser

using namespace cs160::frontend;

ParseStatus SingleCharParser::do_parse(std::string inputProgram,
                                       int startCharacter) {
  int endCharacter = startCharacter;
  std::string errorMessage = "Char should only have alphabetical character";

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter);
  }

  ParseStatus status;
  if ((inputProgram[0] >= 'a' && inputProgram[0] <= 'z') ||
      (inputProgram[0] >= 'A' && inputProgram[0] <= 'Z')) {
    status.status = true;
    status.parsedCharacters = inputProgram[0];
    status.remainingCharacters = inputProgram.erase(0, 1);
    status.startCharacter = endCharacter;
    status.endCharacter = endCharacter + 1;
  } else {
    return super::fail(inputProgram, endCharacter, errorMessage);
  }
  return status;
}

ParseStatus SingleVarCharParser::do_parse(std::string inputProgram,
                                          int startCharacter) {
  std::string errorMessage = "Char should be alphabetical or underscore";

  int endCharacter = startCharacter;
  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter, errorMessage);
  }

  ParseStatus status;
  if ((inputProgram[0] >= 'a' && inputProgram[0] <= 'z') ||
      (inputProgram[0] >= 'A' && inputProgram[0] <= 'Z') ||
      inputProgram[0] == '_') {
    status.status = true;
    status.parsedCharacters = inputProgram[0];
    status.remainingCharacters = inputProgram.erase(0, 1);
    status.startCharacter = endCharacter;
    status.endCharacter = endCharacter + 1;
  } else {
    return super::fail(inputProgram, endCharacter, errorMessage);
  }
  return status;
}
