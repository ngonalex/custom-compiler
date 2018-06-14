#include "frontend/combinators/v3_combinators/main/block_parser.h" // cs160::frontend::StatementParser
#include "frontend/combinators/basic_combinators/one_or_more_combinator.h"
#include "frontend/combinators/v3_combinators/main/statement_parser.h" // cs160::frontend::StatementParser

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

ParseStatus BlockParser::do_parse(std::string inputProgram,
                                      int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter);
  }

  StatementParser statement_parser;
  OneOrMoreCombinator block_parser;
  block_parser.parser = reinterpret_cast<NullParser *>(&statement_parser);
  block_parser.parse(inputProgram, startCharacter);

  auto result = block_parser.do_parse(inputProgram, startCharacter);
  if (!result.status) {
    result.errorType = "Issue parsing block";
  }

  // TODO: Form AST here

  return result;
}