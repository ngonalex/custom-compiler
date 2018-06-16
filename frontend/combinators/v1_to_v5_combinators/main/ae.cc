#include "frontend/combinators/v1_to_v5_combinators/main/ae.h"
#include "frontend/combinators/v1_to_v5_combinators/helpers/v1_helpers.h"

#define super NullParser

namespace cs160 {
namespace frontend {

ParseStatus ArithExprParser::do_parse(std::string inputProgram,
                                      int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter);
  }

  AddSubExprParser ae;
  ParseStatus aeParseResult = ae.do_parse(inputProgram, endCharacter);

  if (!aeParseResult.status) {
    aeParseResult.errorType = "Missing arithmetic expression";
  }

  return aeParseResult;
}

}  // namespace frontend
}  // namespace cs160