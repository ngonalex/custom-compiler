#include "frontend/combinators/v1_combinators/helpers/v1_helpers.h"
#include "frontend/combinators/v1_combinators/mul_div_expr.h"
#include "frontend/combinators/v1_combinators/num_parser.h"
#include "frontend/combinators/v1_combinators/term_expr.h"

#define super NullParser

using namespace cs160::frontend;
using namespace std;

ParseStatus MulDivExprParser::do_parse(std::string inputProgram, int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter);
  }

  // ae
  TermExprParser lhs;
  ParseStatus result = lhs.do_parse(inputProgram, endCharacter);
  if (result.status) {
    // op
    MulDivOpParser op;
    ParseStatus mdParseStatus = op.do_parse(result.remainingCharacters, result.endCharacter);
    while (mdParseStatus.status) {
      result.parsedCharacters += mdParseStatus.parsedCharacters;
      TermExprParser rhs;
      ParseStatus rhsParseStatus = rhs.do_parse(mdParseStatus.remainingCharacters, mdParseStatus.endCharacter);
      result.ast = std::move(make_node(
	      (mdParseStatus.parsedCharacters),
	        unique_cast<const ArithmeticExpr>(std::move(result.ast)),
	        unique_cast<const ArithmeticExpr>(std::move(rhsParseStatus.ast))));
      mdParseStatus = op.do_parse(rhsParseStatus.remainingCharacters, rhsParseStatus.endCharacter);
      result.parsedCharacters += (rhsParseStatus.parsedCharacters);
      result.startCharacter = startCharacter;
      result.endCharacter = rhsParseStatus.endCharacter;
    }
    result.remainingCharacters = mdParseStatus.remainingCharacters;
  }
  return result;  // Returning Success/Failure on TermExpr
}

// Creating the AST Node
std::unique_ptr<const ArithmeticExpr> MulDivExprParser::make_node(
    std::string op, std::unique_ptr<const ArithmeticExpr> first_leaf,
    std::unique_ptr<const ArithmeticExpr> second_leaf) {
  if (op == "*") {
    return make_unique<MultiplyExpr>(std::move(first_leaf),
				     std::move(second_leaf));
  } else if (op == "/") {
    return make_unique<DivideExpr>(std::move(first_leaf),
				   std::move(second_leaf));
  } else {
    return nullptr;
  }
}
