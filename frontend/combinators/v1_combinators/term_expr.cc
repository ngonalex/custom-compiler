#include "frontend/combinators/v1_combinators/term_expr.h"

#define super NullParser

ParseStatus TermExpr::parse(std::string inputProgram) {
  if (inputProgram.size() == 0) {
    return super::parse(inputProgram);
  }
  
  // NUM
  ParseResult numResult;
  NumParser num;
  numResult = num.parse(inputProgram);
  
  // If it succesfully parses a num, return the result containing an AST node
  if (numResult.status) {
    return numResult;
  }
  
  // PAREN EXPR
  
  // (
  ParseResult openResult;
  OpenParenParser open_paren;
  openResult = open_paren.parse(inputProgram);
  
  if (openResult.status) {
    // ae
    ParseResult addSubResult;
    AddSubParser ae;
    addSubResult = ae.parse(openResult.remainingChracters);
    if (addSubResult.status) {
      // )
      CloseParenResult closeResult;
      CloseParenParse close_paren;
      closeResult = close_paren.parse(addSubResult.remainingCharacters);
      
      if (closeResult.status) {
        return addSubResult;  // Returning a successful ( ae )
      }
      return closeResult;  // Return Failure on CloseParen
    }
    return addSubResult;  // Returing Failure on AddSubExpr
  }
  return openResult;  // Returning Failure on OpenParen
}