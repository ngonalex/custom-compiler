#include "frontend/combinators/v1_to_v5_combinators/main/and_relation_parser.h"
#include "frontend/combinators/basic_combinators/and_combinator.h"
#include "frontend/combinators/basic_combinators/one_or_more_combinator.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"
#include "frontend/combinators/v1_to_v5_combinators/helpers/relational_helper.h"
#include "frontend/combinators/v1_to_v5_combinators/main/relation_not.h"

#define super NullParser

namespace cs160 {
namespace frontend {

ParseStatus AndRelationParser::do_parse(std::string inputProgram,
                                        int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter);
  }

  NotRelationParser lhs;
  AndOpParser op;

  AndCombinator andExpr;
  andExpr.firstParser = &lhs;
  andExpr.secondParser = &op;

  OneOrMoreCombinator oneOrMore;
  oneOrMore.parser = &andExpr;

  NotRelationParser rhs;

  AndCombinator andRelationParser;
  andRelationParser.firstParser = &oneOrMore;
  andRelationParser.secondParser = &rhs;

  OrCombinator andRelationParserFinal;
  andRelationParserFinal.firstParser = &andRelationParser;
  andRelationParserFinal.secondParser = &rhs;

  ParseStatus result =
      andRelationParserFinal.do_parse(inputProgram, endCharacter);

  // AST Formation
  int strIndex = 0;
  for (int i = 0; i < result.astNodes.size(); i++) {
    if (i == 0) {
      result.ast = std::move(result.astNodes[i]);
    } else {
      result.ast = make_node(
          unique_cast<const RelationalExpr>(std::move(result.ast)),
          unique_cast<const RelationalExpr>(std::move(result.astNodes[i])));
    }
  }

  result.parsedCharactersArray.erase(std::begin(result.parsedCharactersArray),
                                     std::end(result.parsedCharactersArray));
  result.astNodes.erase(std::begin(result.astNodes), std::end(result.astNodes));

  return result;  // Returning Success/Failure on MulDivExpr
}

// Creating the AST Node
std::unique_ptr<const LogicalAndExpr> AndRelationParser::make_node(
    std::unique_ptr<const RelationalExpr> first_leaf,
    std::unique_ptr<const RelationalExpr> second_leaf) {
  return make_unique<LogicalAndExpr>(std::move(first_leaf),
                                     std::move(second_leaf));
}

}  // namespace frontend
}  // namespace cs160