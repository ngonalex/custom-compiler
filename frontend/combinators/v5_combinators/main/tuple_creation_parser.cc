#include "frontend/combinators/v5_combinators/main/tuple_creation_parser.h"
#include "frontend/combinators/basic_combinators/and_combinator.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"
#include "frontend/combinators/v1_combinators/ae.h"
#include "frontend/combinators/v2_combinators/helpers/var_helper.h"
#include "frontend/combinators/v5_combinators/helpers/tuple_helpers.h"

#include <stdio.h>
#include <string>  // std::string, std::stoi

#define super NullParser

using namespace cs160::frontend;
using namespace std;

ParseStatus TupleAssignmentParser::do_parse(std::string inputProgram,
                                            int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter);
  }

  // Parsers used
  HelperVariableParser varParser;

  LeftBracketOp leftBracketOp;

  ArithExprParser arithExprParser;

  RightBracketOp rightBracketOp;

  SemiColonParser semiColon;

  AndCombinator aeSemi;
  aeSemi.firstParser = &arithExprParser;
  aeSemi.secondParser = &semiColon;

  // Grammar declaration
  AndCombinator firstAnd;
  firstAnd.firstParser = &varParser;
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

  AndCombinator fourthAnd;
  fourthAnd.firstParser = &thirdAnd;
  fourthAnd.secondParser = &semiColon;

  ParseStatus result = fourthAnd.do_parse(inputProgram, endCharacter);

  if (result.status) {
    result.ast = std::move(make_unique<const AssignmentFromNewTuple>(
        unique_cast<const VariableExpr>(std::move(firstResult.ast)),
        unique_cast<const ArithmeticExpr>(std::move(secondResult.second_ast))));
  }

  return result;
}
