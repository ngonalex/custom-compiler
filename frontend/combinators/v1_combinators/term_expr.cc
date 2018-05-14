#include "frontend/combinators/v1_combinators/term_expr.h"

#define super NullParser

ParseStatus TermExpr::parse(std::string inputProgram) {
  if (inputProgram.size() == 0) {
    return super::parse(inputProgram);
  }
  ParseResult result;
  
  
  // NUM
  NumParser num;
  num.parse(inputProgram);
  
  // If it succesfully parses a num, return the result containing an AST node
  if (num.result.status) {
    return num.result;
  }
  
  // PAREN EXPR
  
  // (
  OpenParenParser open_paren;
  open_paren.parse(inputProgram);
  if (open_paren.result.status) {
    // ae
    AddSubParser ae;
    ae.parse(open_paren.result.remainingChracters);
    // )
    CloseParenParse close_paren;
    close_paren.parse(ae.result.remainingCharacters);
    if (close_paren.result.status) {
      // Make the AST and store in in result
      result.status = true;
    }
    result.status = false;
  }
  
  // FAILURE
  return result;
}