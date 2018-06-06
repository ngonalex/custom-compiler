#include "frontend/combinators/basic_combinators/and_combinator.h"
#include "frontend/combinators/basic_combinators/one_or_more_combinator.h"
#include "abstract_syntax/print_visitor_v2.h"

#include "frontend/combinators/v1_combinators/helpers/v1_helpers.h"
#include "frontend/combinators/v1_combinators/add_sub_expr.h"
#include "frontend/combinators/v1_combinators/num_parser.h"
#include "frontend/combinators/v1_combinators/mul_div_expr.h"

#include <iostream>

#define super NullParser

using namespace cs160::frontend;
using namespace std;

/* 
  Parses all characters properly, but next to impossible to do AST generation from this

  // mul_div
  MulDivExprParser lhs;
  ParseStatus result = lhs.parse(inputProgram, endCharacter);
  std::cout << result.parsedCharacters << std::endl;
  
  // (+/- mul_div)*
  AddSubOpParser op;
  MulDivExprParser rhs;
  AndCombinator rhs_combinator;
	rhs_combinator.firstParser = reinterpret_cast<NullParser *>(&op);
	rhs_combinator.secondParser = reinterpret_cast<NullParser *>(&rhs);

  OneOrMoreCombinator right_hand_extras;
  right_hand_extras.parser = reinterpret_cast<NullParser *>(&rhs_combinator);
  ParseStatus optional_result = right_hand_extras.parse(result.remainingCharacters, result.endCharacter);
*/

ParseStatus AddSubExprParser::parse(std::string inputProgram, int startCharacter,
				    std::string errorType) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::parse(inputProgram, endCharacter);
  }
  // ae
  MulDivExprParser lhs;
  ParseStatus result = lhs.parse(inputProgram, endCharacter);
  if (result.status) {
    // op
    AddSubOpParser op;
    ParseStatus asParseStatus = op.parse(result.remainingCharacters, result.endCharacter);
    while (asParseStatus.status) {
      result.parsedCharacters += (asParseStatus.parsedCharacters);
      MulDivExprParser rhs;
      ParseStatus rhsParseStatus = rhs.parse(asParseStatus.remainingCharacters, asParseStatus.endCharacter);
      result.ast = std::move(make_node(
	  (asParseStatus.parsedCharacters),
	  unique_cast<const ArithmeticExpr>(std::move(result.ast)),
	  unique_cast<const ArithmeticExpr>(std::move(rhsParseStatus.ast))));
      asParseStatus = op.parse(rhsParseStatus.remainingCharacters, rhsParseStatus.endCharacter);
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