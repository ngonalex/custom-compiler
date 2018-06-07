#include "frontend/combinators/v1_combinators/helpers/v1_helpers.h"
#include "frontend/combinators/basic_combinators/and_combinator.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"
#include "frontend/combinators/basic_combinators/one_or_more_combinator.h"
#include "frontend/combinators/v1_combinators/add_sub_expr.h"
#include "frontend/combinators/v1_combinators/num_parser.h"
#include "frontend/combinators/v1_combinators/mul_div_expr.h"

#define super NullParser

using namespace cs160::frontend;
using namespace std;

ParseStatus AddSubExprParser::do_parse(std::string inputProgram, int startCharacter) {
    int endCharacter = startCharacter;
    endCharacter += trim(inputProgram);

    if (inputProgram.size() == 0) {
        return super::fail(inputProgram, endCharacter);
    }
    
    MulDivExprParser lhs;
    AddSubOpParser op;
    
    AndCombinator andExpr;
    andExpr.firstParser = &lhs;
    andExpr.secondParser = &op;
    
    OneOrMoreCombinator oneOrMore;
    oneOrMore.parser = &andExpr;
 
    MulDivExprParser rhs;
    
    AndCombinator addSubExpr;
    addSubExpr.firstParser = &oneOrMore;
    addSubExpr.secondParser = &rhs;
    
    OrCombinator addSubExprFinal;
    addSubExprFinal.firstParser = &addSubExpr;
    addSubExprFinal.secondParser = &rhs;
    
    ParseStatus result = addSubExprFinal.do_parse(inputProgram, endCharacter);
    result.ast = 
    
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
