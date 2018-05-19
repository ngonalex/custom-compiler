#include "frontend/combinators/v1_combinators/term_expr.h"

#define super NullParser

using namespace cs160::frontend;
using namespace std;

ParseStatus TermExprParser::parse(std::string inputProgram) {
  if (inputProgram.size() == 0) {
    return super::parse(inputProgram);
  }

  // num
  NumParser num;
  ParseStatus numParseStatus = num.parse(inputProgram);
  if (numParseStatus.status) {
    return numParseStatus;  // Returning Success on Num
  }

  // - ae
  NegativeParser negativeStatus;
  ParseStatus negStatus = negativeStatus.parse(inputProgram);
  if (negStatus.status) {
    AddSubExprParser ae;
    ParseStatus aeParseStatus = ae.parse(negStatus.remainingCharacters);
    if (aeParseStatus.status) {
        std::unique_ptr<const AstNode> zero = make_unique<IntegerExpr>(0);
        aeParseStatus.ast = std::move(make_unique<const SubtractExpr>(std::move(zero), std::move(aeParseStatus.ast)));
      return aeParseStatus; // Returning Success on -AE
    }
    return aeParseStatus; // Returning Failure on -AE
  }

  // ( ae )
  OpenParenParser open_paren;
  ParseStatus openParseStatus = open_paren.parse(inputProgram);
  if (openParseStatus.status) {
    // ae
    AddSubExprParser ae;
    ParseStatus aeParseStatus = ae.parse(openParseStatus.remainingCharacters);
    if (aeParseStatus.status) {
      // )
      CloseParenParser close_paren;
      ParseStatus cpParseStatus = close_paren.parse(aeParseStatus.remainingCharacters);
      if (cpParseStatus.status) {
        aeParseStatus.remainingCharacters = cpParseStatus.remainingCharacters;
        return aeParseStatus; // Returning Success on AE
      }
      return cpParseStatus;  // Returing Failure on AddSubExpr
    }
    return aeParseStatus; // Returning Failure on CloseParen
  }
  return openParseStatus;  // Returning Failure on OpenParen
}
