#include "frontend/combinators/v1_to_v5_combinators/main/relation_body.h"
#include "frontend/combinators/basic_combinators/and_combinator.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"
#include "frontend/combinators/v1_to_v5_combinators/helpers/relational_helper.h"
#include "frontend/combinators/v1_to_v5_combinators/main/ae.h"
#include "frontend/combinators/v1_to_v5_combinators/main/or_relation_parser.h"

#include "abstract_syntax/print_visitor_v3.h"

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

namespace cs160 {
namespace frontend {

ParseStatus RelationBodyParser::do_parse(std::string inputProgram,
                                         int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter);
  }

  ArithExprParser aeParser;
  RelationOperatorParser relOpParser;
  OrRelationParser relParser;

  // ae rop
  AndCombinator aeAndRop;
  aeAndRop.firstParser = &aeParser;
  aeAndRop.secondParser = &relOpParser;
  // ae rop ae
  AndCombinator firstAnd;
  firstAnd.firstParser = &aeAndRop;
  firstAnd.secondParser = &aeParser;
  ParseStatus grabOperatorResult =
      aeAndRop.do_parse(inputProgram, endCharacter);
  ParseStatus firstStatus = firstAnd.do_parse(inputProgram, endCharacter);

  // Cache the result for later
  if (firstStatus.status) {
    // TODO: Look at this, the parsedCharactersArray is empty
    firstStatus.ast = make_node(
        unique_cast<const ArithmeticExpr>(std::move(firstStatus.ast)),
        grabOperatorResult.secondParsedCharacters,
        unique_cast<const ArithmeticExpr>(std::move(firstStatus.second_ast)));
  }

  return firstStatus;
}

// ae rop ae
std::unique_ptr<const RelationalExpr> RelationBodyParser::make_node(
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
  } else if (rop == "!=") {
    return make_unique<const LogicalNotExpr>(make_unique<const EqualToExpr>(
        std::move(first_ae), std::move(second_ae)));
  } else {
    return nullptr;
  }
}

}  // namespace frontend
}  // namespace cs160
