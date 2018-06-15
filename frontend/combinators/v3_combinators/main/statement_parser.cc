#include "frontend/combinators/basic_combinators/or_combinator.h"  // cs160::frontend::OrCombinator

#include "frontend/combinators/v3_combinators/main/statement_parser.h"  // cs160::frontend::StatementParser

#include "frontend/combinators/v2_combinators/main/assignment_parser.h"  // cs160::frontend::AssignmentParser
#include "frontend/combinators/v3_combinators/main/conditional_parser.h"  // cs160::frontend::ConditionalParser
#include "frontend/combinators/v3_combinators/main/loop_parser.h"  // cs160::frontend::LoopParser

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

ParseStatus StatementParser::do_parse(std::string inputProgram,
                                      int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter, "");
  }

  AssignmentParser assignParser;
  ConditionalParser conditionalParser;
  LoopParser loopParser;

  // assign | cond | loop
  OrCombinator assignOrConditional;
  OrCombinator allStatement;

  assignOrConditional.firstParser =
      reinterpret_cast<NullParser *>(&assignParser);
  assignOrConditional.secondParser =
      reinterpret_cast<NullParser *>(&conditionalParser);
  allStatement.firstParser =
      reinterpret_cast<NullParser *>(&assignOrConditional);
  allStatement.secondParser = reinterpret_cast<NullParser *>(&loopParser);

  ParseStatus result = allStatement.do_parse(inputProgram, startCharacter);
  if (!result.status) {
    result.errorType = "Issue parsing statement";
  }

  // TODO: Form AST here

  return result;
}
