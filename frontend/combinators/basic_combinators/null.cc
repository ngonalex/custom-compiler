#include "frontend/combinators/basic_combinators/null.h"
#include "frontend/combinators/basic_combinators/parsestatus.h"

using namespace cs160::frontend;

ParseStatus NullParser::parse(std::string inputProgram) {
  ParseStatus status;
  status.status = false;
  status.remainingCharacters = inputProgram;
  status.parsedCharacters = "";
  status.ast = nullptr;
  status.second_ast = nullptr;
  return status;
}
