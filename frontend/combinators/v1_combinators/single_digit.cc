#include "frontend/combinators/v1_combinators/single_digit.h"

#define super NullParser

using namespace cs160::frontend;

ParseStatus SingleDigitParser::parse(std::string inputProgram,
                                     std::string errorType) {
  trim(inputProgram);
  std::string errorMessage = "Digit should be between 0 and 9";

  if (inputProgram.size() == 0) {
    return super::parse(inputProgram, errorMessage);
  }

  ParseStatus status;
  if ((inputProgram[0] >= '0' && inputProgram[0] <= '9')) {
    status.status = true;
    status.parsedCharacters = inputProgram[0];
    status.remainingCharacters = inputProgram.erase(0, 1);
  } else {
    return super::parse(inputProgram, errorMessage);
  }
  return status;
}
