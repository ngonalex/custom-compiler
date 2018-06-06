#include "frontend/combinators/v1_combinators/add_sub_expr.h"
#include "frontend/combinators/v1_combinators/helpers/v1_helpers.h"
#include "frontend/combinators/v1_combinators/mul_div_expr.h"
#include "frontend/combinators/v1_combinators/num_parser.h"

#define super NullParser

using namespace cs160::frontend;
using namespace std;

ParseStatus AddSubExprParser::parse(std::string inputProgram, int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter);
  }

  // ae
  MulDivExprParser lhs;
  ParseStatus result = lhs.parse(inputProgram, endCharacter);
  if (result.status) {
    // op
    AddSubOpParser op;
    ParseStatus asParseStatus =
        op.parse(result.remainingCharacters, result.endCharacter);
    while (asParseStatus.status) {
      result.parsedCharacters += (asParseStatus.parsedCharacters);
      MulDivExprParser rhs;
      ParseStatus rhsParseStatus = rhs.parse(asParseStatus.remainingCharacters,
                                             asParseStatus.endCharacter);
      result.ast = std::move(make_node(
          (asParseStatus.parsedCharacters),
          unique_cast<const ArithmeticExpr>(std::move(result.ast)),
          unique_cast<const ArithmeticExpr>(std::move(rhsParseStatus.ast))));
      asParseStatus = op.parse(rhsParseStatus.remainingCharacters,
                               rhsParseStatus.endCharacter);
      result.parsedCharacters += (rhsParseStatus.parsedCharacters);
      result.startCharacter = startCharacter;
    }
    result.remainingCharacters = asParseStatus.remainingCharacters;
    result.endCharacter = asParseStatus.endCharacter;
  }
  return result;  // Returning Success/Failure on MulDivExpr
}

// Creating the AST Node
std::unique_ptr<const ArithmeticExpr> AddSubExprParser::make_node(
    std::string op, std::unique_ptr<const ArithmeticExpr> first_leaf,
    std::unique_ptr<const ArithmeticExpr> second_leaf) {
  if (op == "+") {
    return make_unique<AddExpr>(std::move(first_leaf), std::move(second_leaf));
  } else if (op == "-") {
    return make_unique<SubtractExpr>(std::move(first_leaf),
                                     std::move(second_leaf));

  } else {
    return nullptr;
  }
}
