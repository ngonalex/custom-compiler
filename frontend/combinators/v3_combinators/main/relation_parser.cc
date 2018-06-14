#include "frontend/combinators/v3_combinators/main/relation_parser.h"
#include <iostream>
#include <string>  // std::string, std::stoi
#include "frontend/combinators/v3_combinators/helpers/relational_helper.h"
#include "frontend/combinators/v3_combinators/main/relation_body.h"
#include "frontend/combinators/basic_combinators/and_combinator.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"

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

ParseStatus RelationParser::do_parse(std::string inputProgram,
                                  int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  std::string errorMessage = "Error in relation parser";

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter, errorMessage);
  }

  NotOpParser notParser;
  RelationBodyParser re;

  AndCombinator notRe;
  notRe.firstParser = reinterpret_cast<NullParser *>(&notParser);
  notRe.secondParser = reinterpret_cast<NullParser *>(&re);  
  ParseStatus notReResult = notRe.do_parse(inputProgram, startCharacter); // result saved on cache


  OrCombinator notOrRe;
  notOrRe.firstParser = reinterpret_cast<NullParser *>(&notRe);
  notOrRe.secondParser = reinterpret_cast<NullParser *>(&re); 

  ParseStatus notResult = notOrRe.do_parse(inputProgram, startCharacter);

  // Ast Construction and error handling
  if(notResult.status && notResult.firstOrSecond) {
    notResult.ast = make_unique<const LogicalNotExpr>(
      unique_cast<const RelationalExpr>(std::move(notReResult.second_ast)));
  }
  if(!notResult.status) {
    return super::fail(inputProgram, endCharacter, errorMessage);
  }

  return notResult;

  /*
  ParseStatus notResult = notParser.do_parse(inputProgram, startCharacter);

  // Only ! expr will work. !(expr) will not properly parse
  if (notResult.status) {
    // ! REXPR
    ParseStatus reResult =
        re.do_parse(notResult.remainingCharacters, notResult.endCharacter);
    if (reResult.status) {
      reResult.ast = make_unique<const LogicalNotExpr>(
          unique_cast<const RelationalExpr>(std::move(reResult.ast)));
      return reResult;
    }
  } else {
    // REXPR
    ParseStatus reResult = re.do_parse(inputProgram, startCharacter);
    if (reResult.status) {
      return reResult;
    }
  }*/
}
