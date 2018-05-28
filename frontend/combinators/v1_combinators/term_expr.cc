#include "frontend/combinators/v1_combinators/term_expr.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"
#include "frontend/combinators/v2_combinators/main/word_parser.h"
#include "iostream"

#define super NullParser

using namespace cs160::frontend;
using namespace std;

ParseStatus TermExprParser::parse(std::string inputProgram, std::string errorType) {
  trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::parse(inputProgram);
  }

  // num
  OrCombinator orC;
  WordParser wordP;
  NumParser num;
  orC.firstParser = reinterpret_cast<NullParser *>(&wordP);
  orC.secondParser = reinterpret_cast<NullParser *>(&num);

  ParseStatus numParseStatus = orC.parse(inputProgram);
  //ParseStatus numParseStatus = num.parse(inputProgram);

  if (numParseStatus.status) {
    return numParseStatus;  // Returning Success on Num
  }

  ParseStatus result;
  // - ae
  NegativeParser negativeStatus;
  ParseStatus negStatus = negativeStatus.parse(inputProgram);
  result.status = negStatus.status;
  if (negStatus.status) {
    result.parsedCharacters += negStatus.parsedCharacters;
    result.remainingCharacters = negStatus.remainingCharacters;
    AddSubExprParser ae;
    ParseStatus aeParseStatus = ae.parse(negStatus.remainingCharacters);

    if (aeParseStatus.status) {
      result.parsedCharacters += aeParseStatus.parsedCharacters;
      result.remainingCharacters = aeParseStatus.remainingCharacters;
      std::unique_ptr<const ArithmeticExpr> zero = make_unique<IntegerExpr>(0);
      result.ast = std::move(make_unique<const SubtractExpr>(
        unique_cast<const ArithmeticExpr>(std::move(zero)),
        unique_cast<const ArithmeticExpr>(std::move(aeParseStatus.ast))));
    }
    else {
      result.status = aeParseStatus.status;
    }
    return result;
  }

  // ( ae )
  OpenParenParser open_paren;
  ParseStatus openParseStatus = open_paren.parse(inputProgram);
  result.status = openParseStatus.status;
  if (openParseStatus.status) {
    result.parsedCharacters += openParseStatus.parsedCharacters;
    result.remainingCharacters = openParseStatus.remainingCharacters;
    // ae
    AddSubExprParser ae;
    ParseStatus aeParseStatus = ae.parse(openParseStatus.remainingCharacters);
    if (aeParseStatus.status) {
      result.parsedCharacters += aeParseStatus.parsedCharacters;
      result.remainingCharacters = aeParseStatus.remainingCharacters;

      CloseParenParser close_paren;
      ParseStatus cpParseStatus = close_paren.parse(aeParseStatus.remainingCharacters);
      if (cpParseStatus.status) {
        result.parsedCharacters += cpParseStatus.parsedCharacters;
        result.remainingCharacters = cpParseStatus.remainingCharacters;
        result.ast = std::move(aeParseStatus.ast);
      }
      else {
        result.status = cpParseStatus.status;
        result.errorType = cpParseStatus.errorType;
      }
    }
    else {
        result.status = aeParseStatus.status;
        result.errorType = aeParseStatus.errorType;
    }
  }
  else {
    result.status = openParseStatus.status;
      result.errorType = openParseStatus.errorType;
  }

  return result;
}
