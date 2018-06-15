#include "frontend/combinators/v3_combinators/main/relation_not.h"
#include "frontend/combinators/basic_combinators/and_combinator.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"
#include "frontend/combinators/v3_combinators/helpers/relational_helper.h"
#include "frontend/combinators/v3_combinators/main/relation_body.h"

#define super NullParser

using namespace cs160::frontend;
using namespace std;

ParseStatus NotRelationParser::do_parse(std::string inputProgram,
                                        int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter);
  }

  NotOpParser notOp;
  RelationBodyParser relBodyParser;

  AndCombinator notRelationParser;
  notRelationParser.firstParser = &notOp;
  notRelationParser.secondParser = &relBodyParser;

  OrCombinator notRelationParserFinal;
  notRelationParserFinal.firstParser = &notRelationParser;
  notRelationParserFinal.secondParser = &relBodyParser;

  ParseStatus result = notRelationParserFinal.parse(inputProgram, endCharacter);
  if (result.firstParsedCharacters == "!") {
    result.ast = make_node(
        unique_cast<const RelationalExpr>(std::move(result.second_ast)));
  }
  return result;
}

std::unique_ptr<const LogicalNotExpr> NotRelationParser::make_node(
    std::unique_ptr<const RelationalExpr> first_ae) {
  return make_unique<const LogicalNotExpr>(std::move(first_ae));
}
