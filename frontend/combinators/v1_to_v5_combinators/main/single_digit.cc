#include "frontend/combinators/v1_to_v5_combinators/main/single_digit.h"

#define super NullParser

namespace cs160 {
namespace frontend {

ParseStatus SingleDigitParser::do_parse(std::string inputProgram,
                                        int startCharacter) {
  int endCharacter = startCharacter;
  std::string errorMessage = "Digit should be between 0 and 9";

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter, errorMessage);
  }

  ParseStatus status;
  if ((inputProgram[0] >= '0' && inputProgram[0] <= '9')) {
    status.status = true;
    status.parsedCharacters = inputProgram[0];
    status.remainingCharacters = inputProgram.erase(0, 1);
    status.startCharacter = startCharacter;
    status.endCharacter = endCharacter + 1;
  } else {
    return super::fail(inputProgram, endCharacter, errorMessage);
  }
  return status;
}

}  // namespace frontend
}  // namespace cs160