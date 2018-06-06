#include "frontend/combinators/v1_combinators/term_expr.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"
#include "frontend/combinators/v2_combinators/main/word_parser.h"

#include "frontend/combinators/v1_combinators/num_parser.h"
#include "frontend/combinators/v1_combinators/add_sub_expr.h"
#include "frontend/combinators/v1_combinators/helpers/v1_helpers.h"

#define super NullParser

using namespace cs160::frontend;
using namespace std;

ParseStatus TermExprParser::do_parse(std::string inputProgram, int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter);
  }
  // num
  OrCombinator orC;
  WordParser wordP;
  NumParser num;
  orC.firstParser = reinterpret_cast<NullParser *>(&wordP);
  orC.secondParser = reinterpret_cast<NullParser *>(&num);

  ParseStatus numParseStatus = orC.do_parse(inputProgram, endCharacter);
  // ParseStatus numParseStatus = num.do_parse(inputProgram);

  if (numParseStatus.status) {
    return numParseStatus;  // Returning Success on Num
  }

  ParseStatus result;
  // - ae
  NegativeParser negativeStatus;
  ParseStatus negStatus = negativeStatus.do_parse(inputProgram, endCharacter);
  result.status = negStatus.status;
  if (negStatus.status) {
    result.parsedCharacters += negStatus.parsedCharacters;
    result.remainingCharacters = negStatus.remainingCharacters;
    AddSubExprParser ae;
    ParseStatus aeParseStatus = ae.do_parse(negStatus.remainingCharacters, negStatus.endCharacter);

    if (aeParseStatus.status) {
      result.parsedCharacters += aeParseStatus.parsedCharacters;
      result.remainingCharacters = aeParseStatus.remainingCharacters;
      result.startCharacter = startCharacter;
      result.endCharacter = aeParseStatus.endCharacter;
      std::unique_ptr<const ArithmeticExpr> zero = make_unique<IntegerExpr>(0);
      result.ast = std::move(make_unique<const SubtractExpr>(
	      unique_cast<const ArithmeticExpr>(std::move(zero)),
	      unique_cast<const ArithmeticExpr>(std::move(aeParseStatus.ast))));
    } else {
      result.startCharacter = startCharacter;
      result.status = aeParseStatus.status;
    }
    return result;
  }

  // ( ae )
  OpenParenParser open_paren;
  ParseStatus openParseStatus = open_paren.do_parse(inputProgram, endCharacter);
  result.status = openParseStatus.status;
  result.startCharacter = startCharacter;
  if (openParseStatus.status) {
    result.parsedCharacters += openParseStatus.parsedCharacters;
    result.remainingCharacters = openParseStatus.remainingCharacters;
    // ae
    AddSubExprParser ae;
    ParseStatus aeParseStatus = ae.do_parse(openParseStatus.remainingCharacters, openParseStatus.endCharacter);
    if (aeParseStatus.status) {
      result.parsedCharacters += aeParseStatus.parsedCharacters;
      result.remainingCharacters = aeParseStatus.remainingCharacters;
      CloseParenParser close_paren;
      ParseStatus cpParseStatus = close_paren.do_parse(aeParseStatus.remainingCharacters, aeParseStatus.endCharacter);
      if (cpParseStatus.status) {
	      result.parsedCharacters += cpParseStatus.parsedCharacters;
	      result.remainingCharacters = cpParseStatus.remainingCharacters;
        result.endCharacter = cpParseStatus.endCharacter;
	      result.ast = std::move(aeParseStatus.ast);
      } else {
        result.status = cpParseStatus.status;
        result.errorType = cpParseStatus.errorType;
      }
    } else {
      result.status = aeParseStatus.status;
      result.errorType = aeParseStatus.errorType;
    }
  } else {
    result.status = openParseStatus.status;
    result.errorType = openParseStatus.errorType;
  }

  return result;
}
