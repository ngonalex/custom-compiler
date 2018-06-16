#include "frontend/combinators/v1_to_v5_combinators/main/dereference_parser.h"
#include "frontend/combinators/basic_combinators/and_combinator.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"
#include "frontend/combinators/v1_to_v5_combinators/helpers/tuple_helpers.h"
#include "frontend/combinators/v1_to_v5_combinators/helpers/var_helper.h"
#include "frontend/combinators/v1_to_v5_combinators/main/ae.h"
#include "frontend/combinators/v1_to_v5_combinators/main/word_parser.h"

#include <stdio.h>
#include <string>  // std::string, std::stoi

#define super NullParser

using namespace cs160::frontend;
using namespace std;

ParseStatus DereferenceParser::do_parse(std::string inputProgram,
                                        int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter);
  }

  // Parsers used
  WordParser wordParser;

  LeftBracketOp leftBracketOp;

  ArithExprParser arithExprParser;

  RightBracketOp rightBracketOp;

  // Grammar declaration
  AndCombinator firstAnd;
  firstAnd.firstParser = &wordParser;
  firstAnd.secondParser = &leftBracketOp;

  ParseStatus firstResult = firstAnd.do_parse(
      inputProgram, endCharacter);  // Will be used as cache result for second

  AndCombinator secondAnd;
  secondAnd.firstParser = &firstAnd;
  secondAnd.secondParser = &arithExprParser;

  ParseStatus secondResult = secondAnd.do_parse(
      inputProgram, endCharacter);  // Will be used as cache result for third

  AndCombinator thirdAnd;
  thirdAnd.firstParser = &secondAnd;
  thirdAnd.secondParser = &rightBracketOp;

  ParseStatus result = thirdAnd.do_parse(inputProgram, endCharacter);

  if (result.status) {
    result.ast = std::move(make_unique<const Dereference>(
        unique_cast<const VariableExpr>(std::move(firstResult.ast)),
        unique_cast<const ArithmeticExpr>(std::move(secondResult.second_ast))));
  }

  return result;
}
