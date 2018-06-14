#include "frontend/combinators/v3_combinators/main/relation_body.h"
#include "frontend/combinators/basic_combinators/and_combinator.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"
#include "frontend/combinators/v1_combinators/ae.h"
#include "frontend/combinators/v3_combinators/helpers/relational_helper.h"
#include "frontend/combinators/v3_combinators/main/relation_parser.h"

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

  ArithExprParser aeParser;
  RelationOperatorParser relOpParser;

  LogicOperatorParser logOpParser;
  RelationParser relParser;

  // ae rop
  AndCombinator aeAndRop;
  aeAndRop.firstParser = reinterpret_cast<NullParser *>(&aeParser);
  aeAndRop.secondParser = reinterpret_cast<NullParser *>(&relOpParser);
  // ae rop ae
  AndCombinator firstAnd;
  firstAnd.firstParser = reinterpret_cast<NullParser *>(&aeAndRop);
  firstAnd.secondParser = reinterpret_cast<NullParser *>(&aeParser);
  ParseStatus grabOperatorResult = aeAndRop.do_parse(inputProgram, endCharacter);
  ParseStatus firstStatus = firstAnd.do_parse(inputProgram, endCharacter);

  // Cache the result for later
  if (firstStatus.status) {
    // TODO: Look at this, the parsedCharactersArray is empty
    firstStatus.ast = make_node(
        unique_cast<const ArithmeticExpr>(std::move(firstStatus.ast)),
        grabOperatorResult.secondParsedCharacters,
        unique_cast<const ArithmeticExpr>(std::move(firstStatus.second_ast)));
  }

  // Optional logical operator stuff
  AndCombinator lopRel;
  lopRel.firstParser = reinterpret_cast<NullParser *>(&logOpParser);
  lopRel.secondParser = reinterpret_cast<NullParser *>(&relParser);
  ParseStatus secondStatus = lopRel.do_parse(firstStatus.remainingCharacters,
                                               firstStatus.endCharacter);

  AndCombinator firstAndLopRel;
  firstAndLopRel.firstParser = reinterpret_cast<NullParser *>(&firstAnd);
  firstAndLopRel.secondParser = reinterpret_cast<NullParser *>(&lopRel);

  if (secondStatus.status) {
    ParseStatus result = firstAndLopRel.do_parse(inputProgram, endCharacter);
    result.ast = make_node(
        unique_cast<const RelationalExpr>(std::move(firstStatus.ast)),
        secondStatus.secondParsedCharacters,
        unique_cast<const RelationalExpr>(std::move(secondStatus.ast)));
    return result;
  } else {
    // If it's a failure, that's fine, the logcical stuff is optional. Just
    // return the relexpr
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
  } else {
    return nullptr;
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
  } else {
    return nullptr;
  }
}
