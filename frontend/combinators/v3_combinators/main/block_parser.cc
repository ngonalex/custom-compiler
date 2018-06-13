#include "frontend/combinators/v3_combinators/main/relation_body.h"
#include "frontend/combinators/v3_combinators/main/relation_parser.h"
#include "frontend/combinators/basic_combinators/and_combinator.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"
#include "frontend/combinators/v1_combinators/ae.h"
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
  block_parser.parse(inputProgram, startCharacter);
  // TODO: Figure out the AST stuff like Prabal's

  return super::fail(inputProgram, endCharacter);
}