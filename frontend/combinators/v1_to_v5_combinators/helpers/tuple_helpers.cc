#include "frontend/combinators/v1_to_v5_combinators/helpers/tuple_helpers.h"
#include "frontend/combinators/basic_combinators/atom_parser.h"

#define super NullParser

namespace cs160 {
namespace frontend {

ParseStatus LeftBracketOp::do_parse(std::string inputProgram,
                                    int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter, "Expected '['");
  }

  auto opParser = AtomParser('[');

  return opParser.do_parse(inputProgram, endCharacter);
}

ParseStatus RightBracketOp::do_parse(std::string inputProgram,
                                     int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter, "Expected ']'");
  }

  auto opParser = AtomParser(']');

  return opParser.do_parse(inputProgram, endCharacter);
}

}  // namespace frontend
}  // namespace cs160
