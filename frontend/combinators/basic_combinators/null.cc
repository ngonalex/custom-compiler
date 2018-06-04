#include "frontend/combinators/basic_combinators/null.h"
#include "frontend/combinators/basic_combinators/parsestatus.h"

using namespace cs160::frontend;

ParseStatus NullParser::fail(std::string inputProgram, int startCharacter, std::string errorMessage) {
  ParseStatus status;
  status.status = false;
  status.remainingCharacters = inputProgram;
  status.parsedCharacters = "";
  status.startCharacter = startCharacter;
  status.endCharacter = startCharacter;
  status.ast = nullptr;
  // status.second_ast = nullptr;
  if (errorMessage != "") {
    status.errorType = errorMessage;
  }
  return status;
}

ParseStatus NullParser::parse(std::string inputProgram, int startCharacter){
    return fail(inputProgram, startCharacter);
}