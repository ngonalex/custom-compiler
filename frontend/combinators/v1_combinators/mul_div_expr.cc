#include "frontend/combinators/v1_combinators/helpers/v1_helpers.h"
#include "frontend/combinators/basic_combinators/and_combinator.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"
#include "frontend/combinators/basic_combinators/one_or_more_combinator.h"
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
    
    TermExprParser lhs;
    MulDivOpParser op;
    
    AndCombinator andExpr;
    andExpr.firstParser = &lhs;
    andExpr.secondParser = &op;
    
    OneOrMoreCombinator oneOrMore;
    oneOrMore.parser = &andExpr;
    
    TermExprParser rhs;
    
    AndCombinator mulDivExpr;
    mulDivExpr.firstParser = &oneOrMore;
    mulDivExpr.secondParser = &rhs;
    
    OrCombinator mulDivExprFinal;
    mulDivExprFinal.firstParser = &mulDivExpr;
    mulDivExprFinal.secondParser = &rhs;
    
    ParseStatus result = mulDivExprFinal.do_parse(inputProgram, endCharacter);
    
    //AST Formation
    int strIndex = 0;
    for (int i = 0; i < result.astNodes.size(); i++){
        if (i == 0){
            result.ast = std::move(result.astNodes[i]);
        } else {
            std::string parsedCharacters = result.parsedCharactersArray[i-1];
            std::string op = parsedCharacters.substr(parsedCharacters.size()-1, 1);
            result.ast = make_node(op, unique_cast<const ArithmeticExpr>(std::move(result.ast)), unique_cast<const ArithmeticExpr>(std::move(result.astNodes[i])));
        }
    }
    
    result.parsedCharactersArray.erase(std::begin(result.parsedCharactersArray), std::end(result.parsedCharactersArray));
    result.astNodes.erase(std::begin(result.astNodes), std::end(result.astNodes));
    
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