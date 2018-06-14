#include "frontend/combinators/v3_combinators/main/relation_body.h"
#include "frontend/combinators/v3_combinators/main/relation_parser.h"
#include "frontend/combinators/basic_combinators/and_combinator.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"
#include "frontend/combinators/v1_combinators/ae.h"
#include "frontend/combinators/v3_combinators/helpers/relational_helper.h"

#include "abstract_syntax/print_visitor_v3.h"

#include <iostream>
#include <string>  // std::string, std::stoi

/*
  rel -> ae rop ae (lop rel)

  RSTART -> !REXPR | REXPR
  REXPR -> AE ROP AE LOGIC
  LOGIC -> BIN RE | λ
  BIN -> && | ||
  ROP -> > | < | <= | >= | ==
*/

#define super NullParser

using namespace cs160::frontend;
using namespace std;

ParseStatus RelationBodyParser::do_parse(std::string inputProgram,
                                      int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter);
  }

  ArithExprParser firstAeParser;
  RelationOperatorParser relOpParser;
  ArithExprParser secondAeParser;

  LogicOperatorParser logOpParser;
  RelationParser relParser;

  // ae rop
  AndCombinator aeAndRop;
  aeAndRop.firstParser = reinterpret_cast<NullParser *>(&firstAeParser);
  aeAndRop.secondParser = reinterpret_cast<NullParser *>(&relOpParser);
  // ae rop ae
  AndCombinator firstAnd;
  firstAnd.firstParser = reinterpret_cast<NullParser *>(&aeAndRop);
  firstAnd.secondParser = reinterpret_cast<NullParser *>(&secondAeParser);
  ParseStatus firstStatus = firstAnd.do_parse(
    inputProgram, endCharacter);
  // Cache the result for later
  if (firstStatus.status) {
    // TODO: Look at this, the parsedCharactersArray is empty
    firstStatus.ast = make_node(
      unique_cast<const ArithmeticExpr>(std::move(firstStatus.ast)),
            firstStatus.parsedCharactersArray[0],
            unique_cast<const ArithmeticExpr>(std::move(firstStatus.second_ast)));
  }

  // Optional logical operator stuff
  AndCombinator lopRel;
  lopRel.firstParser = reinterpret_cast<NullParser *>(&logOpParser);
  lopRel.secondParser = reinterpret_cast<NullParser *>(&relParser);
  ParseStatus secondStatus = firstAnd.do_parse(
    inputProgram, endCharacter);  

  if (secondStatus.status) {
    secondStatus.ast = make_node(unique_cast<const RelationalExpr>(std::move(firstStatus.ast)),
                          secondStatus.parsedCharactersArray[0], 
                          unique_cast<const RelationalExpr>(std::move(secondStatus.ast)));
  } else {
    // If it's a failure, that's fine, the logcical stuff is optional. Just return the relexpr
    return firstStatus;
  }

  return secondStatus;
}

// ae rop ae
std::unique_ptr<const RelationalBinaryOperator> RelationBodyParser::make_node(
    std::unique_ptr<const ArithmeticExpr> first_ae, std::string rop,
    std::unique_ptr<const ArithmeticExpr> second_ae) {
  if (rop == "<") {
    return make_unique<const LessThanExpr>(std::move(first_ae),
                                           std::move(second_ae));
  } else if (rop == "<=") {
    return make_unique<const LessThanEqualToExpr>(std::move(first_ae),
                                                  std::move(second_ae));
  } else if (rop == ">") {
    return make_unique<const GreaterThanExpr>(std::move(first_ae),
                                              std::move(second_ae));
  } else if (rop == ">=") {
    return make_unique<const GreaterThanEqualToExpr>(std::move(first_ae),
                                                     std::move(second_ae));
  } else if (rop == "==") {
    return make_unique<const EqualToExpr>(std::move(first_ae),
                                          std::move(second_ae));
  }
}

std::unique_ptr<const LogicalBinaryOperator> RelationBodyParser::make_node(
    std::unique_ptr<const RelationalExpr> first_re, std::string lop,
    std::unique_ptr<const RelationalExpr> second_re) {
  if (lop == "&&") {
    return make_unique<const LogicalAndExpr>(std::move(first_re),
                                             std::move(second_re));
  } else if (lop == "||") {
    return make_unique<const LogicalOrExpr>(std::move(first_re),
                                            std::move(second_re));
  }
}