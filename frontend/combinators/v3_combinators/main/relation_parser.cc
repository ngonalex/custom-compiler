#include "frontend/combinators/v3_combinators/main/relation_parser.h"
#include <iostream>
#include <string>  // std::string, std::stoi
#include "frontend/combinators/v3_combinators/helpers/relational_helper.h"
#include "frontend/combinators/v3_combinators/main/relation_body.h"

/*
  RSTART -> !(REXPR) | REXPR
  REXPR -> AE ROP AE LOGIC
  LOGIC -> BIN RE | λ
  BIN -> && | ||
  ROP -> > | < | <= | >= | ==
*/

#define super NullParser

using namespace cs160::frontend;
using namespace std;

ParseStatus RelationParser::parse(std::string inputProgram,
                                  int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter, "");
  }

  RelationBodyParser re;
  NotOpParser notParser;
  ParseStatus notResult = notParser.parse(inputProgram, startCharacter);

  // Only ! expr will work. !(expr) will not properly parse
  if (notResult.status) {
    // ! REXPR
    ParseStatus reResult =
        re.parse(notResult.remainingCharacters, notResult.endCharacter);
    if (reResult.status) {
      reResult.ast = make_unique<const LogicalNotExpr>(
          unique_cast<const RelationalExpr>(std::move(reResult.ast)));
      return reResult;
    }
  } else {
    // REXPR
    ParseStatus reResult = re.parse(inputProgram, startCharacter);
    if (reResult.status) {
      return reResult;
    }
  }
  return super::fail(inputProgram, endCharacter, "");
}
