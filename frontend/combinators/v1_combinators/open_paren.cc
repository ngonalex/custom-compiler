#include "frontend/combinators/v1_combinators/open_paren.h"

#define super NullParser

using namespace cs160::frontend;

ParseStatus OpenParenParser::parse(std::string inputProgram,
				   std::string errorType) {
  trim(inputProgram);

  std::string errorMessage = "Expecting open paranthesis";

  if (inputProgram.size() == 0) {
    return super::parse(inputProgram, errorMessage);
  }

  ParseStatus status;
  if (inputProgram[0] == '(') {
    status.status = true;
    status.remainingCharacters = inputProgram.erase(0, 1);
    status.parsedCharacters = '(';
  } else {
    return super::parse(inputProgram, errorMessage);
  }
  return status;
}
