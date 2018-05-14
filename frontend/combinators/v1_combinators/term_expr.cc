#include "frontend/combinators/v1_combinators/term_expr.h"

#define super NullParser

ParseStatus TermExpr::parse(std::string inputProgram) {
  if (inputProgram.size() == 0) {
    return super::parse(inputProgram);
  }
  ParseStatus result;


  // NUM
  ParseResult numResult;
  NumParser num;
  ParseStatus numParseStatus = num.parse(inputProgram);

  // If it succesfully parses a num, return the result containing an AST node
  if (numParseStatus.status) {
    return numParseStatus;
  }

  // PAREN EXPR

  // (
  ParseResult openResult;
  OpenParenParser open_paren;
  ParseStatus openParseStatus = open_paren.parse(inputProgram);
  if (openParseStatus.status) {
    // ae
    ParseResult addSubResult;
    AddSubParser ae;
    ParseStatus aeParseStatus = ae.parse(openParseStatus.remainingCharacters);
    // )
    CloseParenParser close_paren;
    ParseStatus cpParseStatus = close_paren.parse(aeParseStatus.remainingCharacters);
    if (cpParseStatus.status) {
      // Make the AST and store in in result
      result.status = true;
    }
    return addSubResult;  // Returing Failure on AddSubExpr
  }

  // FAILURE
  return result;
}
