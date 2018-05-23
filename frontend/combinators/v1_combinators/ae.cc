#include "frontend/combinators/v1_combinators/ae.h"

#define super NullParser

using namespace cs160::frontend;

ParseStatus ArithExprParser::parse(std::string inputProgram) {
  if (inputProgram.size() == 0) {
    return super::parse(inputProgram);
  }
  AddSubExprParser ae;
  ParseStatus aeParseResult = ae.parse(inputProgram);
  return aeParseResult;
}
