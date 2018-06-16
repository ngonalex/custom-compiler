#include "frontend/combinators/v1Tov4Combinators/main/block_parser.h"  // cs160::frontend::StatementParser
#include "frontend/combinators/basic_combinators/one_or_more_combinator.h"
#include "frontend/combinators/v1Tov4Combinators/main/statement_parser.h"  // cs160::frontend::StatementParser

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
  block_parser.parser = &statement_parser;

  ParseStatus result = block_parser.parse(inputProgram, startCharacter);
  if (!result.status) {
    result.errorType = "Issue parsing block";
  }

  return result;
}
