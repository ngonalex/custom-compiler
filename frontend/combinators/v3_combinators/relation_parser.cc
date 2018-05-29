#include "frontend/combinators/v3_combinators/relation_parser.h"
#include "frontend/combinators/v3_combinators/helpers/relational_helpers.h"
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

  NotParser notParser;
  ParseStatus notResult = notParse.Parse(inputProgram);
  if (notResult.success) {
    // ! REXPR
    ParseStatus reResult = re.Parser(notResult.remainingCharacters);
    if (reResult.status()) {
      reResult.ast = make_unique<const LogicalNotExpr>(
        unique_cast<const RelationalExpr>(std::move(reResult.ast)));
    }
  } else {
    // REXPR
    ParseStatus reResult = re.Parser(inputProgram);
    if (reResult.status()) {
      return result;
    }
  }

  // TODO: Return something
}
