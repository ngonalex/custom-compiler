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
    
    ParseStatus oneOrMoreResult = oneOrMore.do_parse(inputProgram, endCharacter);
    
    //AST Formation
    int strIndex = 0;
    for (int i = 0; i < oneOrMoreResult.astNodes.size(); i++){
        if (i == 0){
            oneOrMoreResult.ast = std::move(oneOrMoreResult.astNodes[i]);
        } else {
            std::string parsedCharacters = oneOrMoreResult.parsedCharactersArray[i-1];
            std::string op = parsedCharacters.substr(parsedCharacters.size()-1, 1);
            oneOrMoreResult.ast = make_node(op, unique_cast<const ArithmeticExpr>(std::move(oneOrMoreResult.ast)), unique_cast<const ArithmeticExpr>(std::move(oneOrMoreResult.astNodes[i])));
        }
    }
    
    MulDivExprParser rhs;
    
    AndCombinator addSubExpr;
    addSubExpr.firstParser = &oneOrMore;
    addSubExpr.secondParser = &rhs;
    
    OrCombinator addSubExprFinal;
    addSubExprFinal.firstParser = &addSubExpr;
    addSubExprFinal.secondParser = &rhs;
    
    ParseStatus result = addSubExprFinal.do_parse(inputProgram, endCharacter);
    
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
