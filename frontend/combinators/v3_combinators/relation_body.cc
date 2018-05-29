#include "frontend/combinators/v1_combinators/ae.h"
#include "frontend/combinators/v3_combinators/relation_body.h"
#include "frontend/combinators/v3_combinators/helpers/relational_helpers.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"
#include <string>     // std::string, std::stoi

/*
  RSTART -> !REXPR | REXPR
  REXPR -> AE ROP AE LOGIC
  LOGIC -> BIN RE | λ
  BIN -> && | ||
  ROP -> > | < | <= | >= | ==
*/

#define super NullParser

using namespace cs160::frontend;
using namespace std;

ParseStatus RelationBodyParser::parse(std::string inputProgram, std::string errorType) {
	trim(inputProgram);

  if (inputProgram.size() == 0) {
		return super::parse(inputProgram);
	}

  // First Arithmetic Expression
  ArithExprParser aeParser;
  ParseStatus result = aeParser.Parse(inputProgram) ;
  std::string errorMessage = "Expected Arithmetic Expression";

  // Relational Operator
	if (result.status) {
    errorMessage = "Expected Relation Operator (>, >=, <, <=, ==)";
    // Bunch of crap to parse all the operators
    RelationOperator relOpParser;

    ParseStatus result2 = relOpParser.parse(result.remainingCharacters);

    // Second Arithmetic Expression
    if (result2.status) {
      errorMessage = "Expected an Arithmetic Expression after Relation Operator";
      ArithExprParser aeParser;
      ParseStatus result = aeParser.Parse(result2.remainingCharacters); 
      if (result3.status) {
        result3.ast = std::move((std::move(result.ast), 
          result2.parsedCharacters, std::move(result3.ast)));
        return result3;
      }
       // TODO: Check logic operators
    }
   
    
  } 
  
  // TODO: Recheck this. It's probably wrong
  else {
		// Error type returned to user
		result.errorType = errorMessage;
	}

	return result;
}

// ae rop ae
std::unique_ptr<const ArithmeticExpr> RelationBodyParser::make_node(std::unique_ptr<const ArithmeticExpr> first_ae, 
  std::string rop, std::unique_ptr<const ArithmeticExpr> second_ae) {
  if (rop == "<") {
    return make_unique<const LessThanExpr>(std::move(first_ae), std::move(second_ae));
  } else if (rop == "<=") {
    return make_unique<const LessThanEqualToExpr>(std::move(first_ae), std::move(second_ae));
  } else if (reop == ">") {
    return make_unique<const GreaterThanExpr>(std::move(first_ae), std::move(second_ae));
  } else if (rop == ">=") {
    return make_unique<const GreaterThanEqualToExpr>(std::move(first_ae), std::move(second_ae));
  } else /*(rop == "==")*/ {
    return make_unique<const EqualToExpr>(std::move(first_ae), std::move(second_ae));
  }
}
