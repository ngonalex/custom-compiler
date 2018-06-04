#include "frontend/combinators/v3_combinators/main/relation_parser.h"
#include "frontend/combinators/v3_combinators/main/relation_body.h"
#include "frontend/combinators/v3_combinators/helpers/relational_helper.h"
#include <string>     // std::string, std::stoi

/*
  RSTART -> !REXPR | REXPR
  REXPR -> AE ROP AE LOGIC
  LOGIC -> BIN RE | λ
  BIN -> && | ||
  ROP -> > | < | <= | >= | ==
*/

#define super NullParser

using namespace cs160::frontend;
using namespace std;

ParseStatus RelationParser::parse(std::string inputProgram, std::string errorType) {
	trim(inputProgram);

  if (inputProgram.size() == 0) {
		return super::parse(inputProgram);
	}

  RelationBodyParser re;

  NotOpParser notParser;
  ParseStatus notResult = notParser.parse(inputProgram);
  if (notResult.status) {
    // ! REXPR
    ParseStatus reResult = re.parse(notResult.remainingCharacters);
    if (reResult.status) {
      reResult.ast = make_unique<const LogicalNotExpr>(
        unique_cast<const RelationalExpr>(std::move(reResult.ast)));
    }
  } else {
    // REXPR
    ParseStatus reResult = re.parse(inputProgram);
    if (reResult.status) {
      return reResult;
    }
  }
  return notResult;

  // TODO: Return something
}
