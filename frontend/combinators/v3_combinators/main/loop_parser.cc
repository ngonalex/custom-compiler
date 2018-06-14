#include "frontend/combinators/v3_combinators/main/loop_parser.h"
#include "frontend/combinators/v3_combinators/main/block_parser.h"
#include "frontend/combinators/v3_combinators/main/relation_body.h"
#include "frontend/combinators/v3_combinators/main/relation_parser.h"
#include "frontend/combinators/basic_combinators/and_combinator.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"
#include "frontend/combinators/v1_combinators/ae.h"
#include "frontend/combinators/v1_combinators/helpers/v1_helpers.h"
#include "frontend/combinators/v3_combinators/helpers/relational_helper.h"

#include <iostream>
#include <string>  // std::string, std::stoi

/*
  rel_start -> not rel_expr | rel_expr
  rel_expr -> ae rop ae logic 
  log_expr -> log_term {or log_term}
  log_term -> log_factor { and log_factor }
  log_factor -> constant | not log_factor | ( log_expr )
  constant -> "True" | "False"
  or -> '||'
  and -> '&&'
  not -> '!'
  rop -> ">" | "<" | "<=" | ">=" | "=="
*/

#define super NullParser

using namespace cs160::frontend;
using namespace std;

ParseStatus LoopParser::do_parse(std::string inputProgram,
                                      int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter);
  }

  std::string errorMessage = "Incorrect loop declaration";

  RepeatKeyword repeatP;
  WhileKeyword whileP;
  OpenCurlyBrackets openBracketP;
  CloseCurlyBrackets closeBracketP;
  OpenParenParser openParenP;
  CloseParenParser closeParenP;
  RelationParser relationP;
  BlockParser block;

  AndCombinator doWhile1;
  doWhile1.firstParser = reinterpret_cast<NullParser *>(&repeatP);
  doWhile1.secondParser = reinterpret_cast<NullParser *>(&openBracketP);
  AndCombinator doWhile2;
  doWhile2.firstParser = reinterpret_cast<NullParser *>(&doWhile1);
  doWhile2.secondParser = reinterpret_cast<NullParser *>(&block);
  AndCombinator doWhile3;
  doWhile3.firstParser = reinterpret_cast<NullParser *>(&doWhile2);
  doWhile3.secondParser = reinterpret_cast<NullParser *>(&closeBracketP);
  AndCombinator doWhile4;
  doWhile4.firstParser = reinterpret_cast<NullParser *>(&doWhile3);
  doWhile4.secondParser = reinterpret_cast<NullParser *>(&whileP);
  AndCombinator doWhile5;
  doWhile5.firstParser = reinterpret_cast<NullParser *>(&doWhile4);
  doWhile5.secondParser = reinterpret_cast<NullParser *>(&openParenP);
  AndCombinator doWhile6;
  doWhile6.firstParser = reinterpret_cast<NullParser *>(&doWhile5);
  doWhile6.secondParser = reinterpret_cast<NullParser *>(&relationP);
  AndCombinator doWhile7;
  doWhile7.firstParser = reinterpret_cast<NullParser *>(&doWhile6);
  doWhile7.secondParser = reinterpret_cast<NullParser *>(&closeParenP);
  //ParseStatus doWhileBlock = block.do_parse(doWhile1.do_parse().remainingCharacters, doWhile1.endCharacter);


  AndCombinator while1;
  while1.firstParser = reinterpret_cast<NullParser *>(&whileP);
  while1.secondParser = reinterpret_cast<NullParser *>(&openParenP);
  AndCombinator while2;
  while2.firstParser = reinterpret_cast<NullParser *>(&while1);
  while2.secondParser = reinterpret_cast<NullParser *>(&relationP);
  AndCombinator while3;
  while3.firstParser = reinterpret_cast<NullParser *>(&while2);
  while3.secondParser = reinterpret_cast<NullParser *>(&closeParenP);
  AndCombinator while4;
  while4.firstParser = reinterpret_cast<NullParser *>(&while3);
  while4.secondParser = reinterpret_cast<NullParser *>(&openBracketP);
  AndCombinator while5;
  while5.firstParser = reinterpret_cast<NullParser *>(&while4);
  while5.secondParser = reinterpret_cast<NullParser *>(&block);
  AndCombinator while6;
  while6.firstParser = reinterpret_cast<NullParser *>(&while5);
  while6.secondParser = reinterpret_cast<NullParser *>(&closeBracketP);
  //ParseStatus whileBlock = block.do_parse(while4.remainingCharacters, while4.endCharacter);


  OrCombinator loops;
  loops.firstParser = reinterpret_cast<NullParser *>(&doWhile7);
  loops.secondParser = reinterpret_cast<NullParser *>(&while6);

  ParseStatus result = loops.do_parse(inputProgram, endCharacter);
  if(result.status) {
    /*
     std::unique_ptr<const RelationalExpr> relationAst;
      std::vector<std::unique_ptr<const AstNode>> blockAsts;
      for() 
     if(result.firstOrSecond) {
      tempAst = std::move(while2.secondAst);
     }
     else {
      tempAst = std::move(doWhile6.secondAst);
     }
     result.ast = std::move(make_unique<const Loop>(
        unique_cast<const RelationalExpr>(std::move(relationAst)),
        unique_cast<const Statement::Block>(std::move(while5))));*/
      return result;
  }
  else {
    result.errorType = errorMessage;
    return super::fail(inputProgram, endCharacter);
  }

}
