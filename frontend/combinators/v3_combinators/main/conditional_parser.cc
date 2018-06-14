#include "frontend/combinators/basic_combinators/and_combinator.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"

#include "frontend/combinators/v1_combinators/helpers/v1_helpers.h"
#include "frontend/combinators/v3_combinators/helpers/relational_helper.h"
#include "frontend/combinators/v3_combinators/main/statement_parser.h"

#include "frontend/combinators/v3_combinators/main/conditional_parser.h" // cs160::frontend::LoopParser

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

ParseStatus LoopParser::do_parse(std::string inputProgram,
                                  int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter, "");
  }

  IfParser ifParser;
  ElseParser elseParser;
  OpenParenParser openParen;
  CloseParenParser closeParen;
  OpenCurlyBrackets openBracket;
  CloseCurlyBrackets closeBracket;
  RelationalExpr rel_expr;
  BlockParser block1;
  BlockParser block2;

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
	open_bracket.secondParser = reinterpret_cast<NullParser *>(&openBrackets);
 // if (rel) { block1
  AndCombinator block_1;
  block_1.firstParser = reinterpret_cast<NullParser *>(&open_bracket);
	block_1.secondParser = reinterpret_cast<NullParser *>(&block1);
  // if (rel) { block1 }
  AndCombinator open_bracket;
  if_open_rel_close.firstParser = reinterpret_cast<NullParser *>(&block_1);
	if_open_rel_close.secondParser = reinterpret_cast<NullParser *>(&closeBracket);
  // if (rel) { block1 } else
  AndCombinator close_else;
  close_else.firstParser = reinterpret_cast<NullParser *>(&open_bracket);
	close_else.secondParser = reinterpret_cast<NullParser *>(&elseParser);
  // if (rel) { block1 } else {
  AndCombinator else_bracket;
  else_bracket.firstParser = reinterpret_cast<NullParser *>(&close_else);
	else_bracket.secondParser = reinterpret_cast<NullParser *>(&openBracket);
  // if (rel) { block1 } else { block2
  AndCombinator block_2;
  block_2.firstParser = reinterpret_cast<NullParser *>(&else_bracket);
	block_2.secondParser = reinterpret_cast<NullParser *>(&block2);
  // if (rel) { block1 } else { block2 }
  AndCombinator close_else;
  close_else.firstParser = reinterpret_cast<NullParser *>(&block_2);
	close_else.secondParser = reinterpret_cast<NullParser *>(&closeBracket);

  closeBracket.parse(inputProgram, startCharacter);

  return super::fail(inputProgram, endCharacter, "");
}