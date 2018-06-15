#include "frontend/combinators/basic_combinators/and_combinator.h"

#include "frontend/combinators/v1_combinators/helpers/v1_helpers.h"  // ( )
#include "frontend/combinators/v3_combinators/helpers/relational_helper.h"  // { } if else

#include "frontend/combinators/v3_combinators/main/block_parser.h"  // cs160::frontend::BlockParser
#include "frontend/combinators/v3_combinators/main/conditional_parser.h"  // cs160::frontend::ConditionalParser
#include "frontend/combinators/v3_combinators/main/or_relation_parser.h"  // cs160::frontend::RelationParser

#include <iostream>
#include <string>  // std::string, std::stoi

/*
  stmt ∈ Statement ⟵ assign | cond | loop
  block ∈ Block ⟵ stmt...

  assign ∈ Assignment ⟵ v := ae
  cond ∈ Conditional ⟵ if re block1 block2
  loop ∈ Loop ⟵ while re block

  prog ∈ Program ⟵ block ae
*/

#define super NullParser

using namespace cs160::frontend;
using namespace std;

ParseStatus ConditionalParser::do_parse(std::string inputProgram,
                                        int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter, "");
  }

  IfKeyword ifParser;
  OpenParenParser openParen;
  OrRelationParser rel_expr;
  CloseParenParser closeParen;
  OpenCurlyBrackets openBracket1;
  BlockParser block1;
  CloseCurlyBrackets closeBracket1;
  ElseKeyword elseParser;
  OpenCurlyBrackets openBracket2;
  BlockParser block2;
  CloseCurlyBrackets closeBracket2;

  // if (
  AndCombinator if_open;
  if_open.firstParser = reinterpret_cast<NullParser *>(&ifParser);
  if_open.secondParser = reinterpret_cast<NullParser *>(&openParen);
  // if (rel
  AndCombinator if_open_rel;
  if_open_rel.firstParser = reinterpret_cast<NullParser *>(&if_open);
  if_open_rel.secondParser = reinterpret_cast<NullParser *>(&rel_expr);
  // if (rel)
  AndCombinator if_open_rel_close;
  if_open_rel_close.firstParser = reinterpret_cast<NullParser *>(&if_open_rel);
  if_open_rel_close.secondParser = reinterpret_cast<NullParser *>(&closeParen);
  // if (rel) {
  AndCombinator open_bracket;
  open_bracket.firstParser = reinterpret_cast<NullParser *>(&if_open_rel_close);
  open_bracket.secondParser = reinterpret_cast<NullParser *>(&openBracket1);
  // if (rel) { block1
  AndCombinator block_1;
  block_1.firstParser = reinterpret_cast<NullParser *>(&open_bracket);
  block_1.secondParser = reinterpret_cast<NullParser *>(&block1);
  // if (rel) { block1 }
  AndCombinator block_1_close;
  block_1_close.firstParser = reinterpret_cast<NullParser *>(&block_1);
  block_1_close.secondParser = reinterpret_cast<NullParser *>(&closeBracket1);

  Statement::Block trueBlock;

  ParseStatus trueBlockResult =
      block_1_close.do_parse(inputProgram, startCharacter);
  if (!trueBlockResult.status) {
    trueBlockResult.errorType = "Issue parsing conditional";
    return trueBlockResult;
  }
  std::unique_ptr<const RelationalExpr> guard =
      unique_cast<const RelationalExpr>(std::move(trueBlockResult.astNodes[0]));
  for (int i = 1; i < trueBlockResult.astNodes.size(); i++) {
    trueBlock.push_back(
        unique_cast<const Statement>(std::move(trueBlockResult.astNodes[i])));
  }
  trueBlockResult.astNodes.erase(std::begin(trueBlockResult.astNodes),
                                 std::end(trueBlockResult.astNodes));

  // if (rel) { block1 } else
  AndCombinator close_else;
  close_else.firstParser = reinterpret_cast<NullParser *>(&block_1_close);
  close_else.secondParser = reinterpret_cast<NullParser *>(&elseParser);
  // if (rel) { block1 } else {
  AndCombinator else_bracket;
  else_bracket.firstParser = reinterpret_cast<NullParser *>(&close_else);
  else_bracket.secondParser = reinterpret_cast<NullParser *>(&openBracket2);
  // if (rel) { block1 } else { block2
  AndCombinator block_2;
  block_2.firstParser = reinterpret_cast<NullParser *>(&else_bracket);
  block_2.secondParser = reinterpret_cast<NullParser *>(&block2);
  // if (rel) { block1 } else { block2 }
  AndCombinator block2_close;
  block2_close.firstParser = reinterpret_cast<NullParser *>(&block_2);
  block2_close.secondParser = reinterpret_cast<NullParser *>(&closeBracket2);

  ParseStatus result = block2_close.do_parse(inputProgram, startCharacter);
  if (!result.status) {
    Statement::Block falseBlock;
    result.ast = std::move(make_unique<const Conditional>(
        std::move(guard), std::move(trueBlock), std::move(falseBlock)));
    return trueBlockResult;
  }
  Statement::Block falseBlock;
  for (int i = trueBlock.size() + 1; i < result.astNodes.size(); i++) {
    falseBlock.push_back(
        unique_cast<const Statement>(std::move(result.astNodes[i])));
  }
  result.ast = std::move(make_unique<const Conditional>(
      std::move(guard), std::move(trueBlock), std::move(falseBlock)));
  result.astNodes.erase(std::begin(result.astNodes), std::end(result.astNodes));

  return result;
}
