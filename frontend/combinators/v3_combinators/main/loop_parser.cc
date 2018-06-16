#include "frontend/combinators/v3_combinators/main/loop_parser.h"
#include "frontend/combinators/basic_combinators/and_combinator.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"
#include "frontend/combinators/v1_combinators/ae.h"
#include "frontend/combinators/v1_combinators/helpers/v1_helpers.h"
#include "frontend/combinators/v3_combinators/helpers/relational_helper.h"
#include "frontend/combinators/v3_combinators/main/block_parser.h"
#include "frontend/combinators/v3_combinators/main/or_relation_parser.h"
#include "frontend/combinators/v3_combinators/main/relation_body.h"

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

namespace cs160 {
namespace frontend {

ParseStatus LoopParser::do_parse(std::string inputProgram, int startCharacter) {
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
  OrRelationParser relationP;
  BlockParser block;

  AndCombinator doWhile1;
  doWhile1.firstParser = &repeatP;
  doWhile1.secondParser = &openBracketP;
  AndCombinator doWhile2;
  doWhile2.firstParser = &doWhile1;
  doWhile2.secondParser = &block;
  AndCombinator doWhile3;
  doWhile3.firstParser = &doWhile2;
  doWhile3.secondParser = &closeBracketP;
  AndCombinator doWhile4;
  doWhile4.firstParser = &doWhile3;
  doWhile4.secondParser = &whileP;
  AndCombinator doWhile5;
  doWhile5.firstParser = &doWhile4;
  doWhile5.secondParser = &openParenP;
  AndCombinator doWhile6;
  doWhile6.firstParser = &doWhile5;
  doWhile6.secondParser = &relationP;
  AndCombinator doWhile7;
  doWhile7.firstParser = &doWhile6;
  doWhile7.secondParser = &closeParenP;
  // ParseStatus doWhileBlock =
  // block.do_parse(doWhile1.do_parse().remainingCharacters,
  // doWhile1.endCharacter);

  AndCombinator while1;
  while1.firstParser = &whileP;
  while1.secondParser = &openParenP;
  AndCombinator while2;
  while2.firstParser = &while1;
  while2.secondParser = &relationP;
  AndCombinator while3;
  while3.firstParser = &while2;
  while3.secondParser = &closeParenP;
  AndCombinator while4;
  while4.firstParser = &while3;
  while4.secondParser = &openBracketP;
  AndCombinator while5;
  while5.firstParser = &while4;
  while5.secondParser = &block;
  AndCombinator while6;
  while6.firstParser = &while5;
  while6.secondParser = &closeBracketP;
  // ParseStatus whileBlock = block.do_parse(while4.remainingCharacters,
  // while4.endCharacter);

  OrCombinator loops;
  loops.firstParser = &doWhile7;
  loops.secondParser = &while6;

  ParseStatus result = loops.do_parse(inputProgram, endCharacter);
  if (result.status) {
    Statement::Block block;
    if (result.firstOrSecond) {
      for (int i = 0; i < result.astNodes.size() - 1; i++) {
        block.push_back(
            unique_cast<const Statement>(std::move(result.astNodes[i])));
      }
      std::unique_ptr<const RelationalExpr> guard =
          unique_cast<const RelationalExpr>(
              std::move(result.astNodes[result.astNodes.size() - 1]));

      result.ast = std::move(
          make_unique<const Loop>(std::move(guard), std::move(block)));
    } else {
      for (int i = 1; i < result.astNodes.size(); i++) {
        block.push_back(
            unique_cast<const Statement>(std::move(result.astNodes[i])));
      }
      std::unique_ptr<const RelationalExpr> guard =
          unique_cast<const RelationalExpr>(std::move(result.astNodes[0]));
      result.ast = std::move(
          make_unique<const Loop>(std::move(guard), std::move(block)));
    }

    result.astNodes.erase(std::begin(result.astNodes),
                          std::end(result.astNodes));

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
  } else {
    result.errorType = errorMessage;
    return super::fail(inputProgram, endCharacter);
  }
}

}  // namespace frontend
}  // namespace cs160
