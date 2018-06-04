#include "frontend/combinators/v1_combinators/ae.h"
#include "frontend/combinators/v3_combinators/main/relation_body.h"
#include "frontend/combinators/v3_combinators/helpers/relational_helper.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"

#include <iostream>
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
  ParseStatus result = aeParser.parse(inputProgram);
  std::string errorMessage = "Expected Arithmetic Expression";
  std::cout << "Succesfully parsed Arithmetic Expression" << std::endl;

	if (result.status) {
    errorMessage = "Expected Relation Operator (>, >=, <, <=, ==)";
    // Parse the relational operators
    RelationOperator relOpParser;
    ParseStatus result2 = relOpParser.parse(result.remainingCharacters);
    if (result2.status) {
      result2.parsedCharacters = result.parsedCharacters + result2.parsedCharacters;
      errorMessage = "Expected an Arithmetic Expression after Relation Operator";
      // Second Arithmetic Expression
      ArithExprParser aeParser;
      ParseStatus result3 = aeParser.parse(result2.remainingCharacters); 
      if (result3.status) {
        result3.parsedCharacters = result2.parsedCharacters + result3.parsedCharacters;
        result3.ast = std::move((std::move(result.ast), 
          result2.parsedCharacters, std::move(result3.ast)));
        return result3;
      }
    }
  } else {
		// Error type returned to user
		result.errorType = errorMessage;
	}
	return result;
}

// ae rop ae
std::unique_ptr<const RelationalBinaryOperator> RelationBodyParser::make_node(
     std::unique_ptr<const ArithmeticExpr> first_ae, 
     std::string rop, std::unique_ptr<const ArithmeticExpr> second_ae) {
  if (rop == "<") {
    return make_unique<const LessThanExpr>(std::move(first_ae), std::move(second_ae));
  } else if (rop == "<=") {
    return make_unique<const LessThanEqualToExpr>(std::move(first_ae), std::move(second_ae));
  } else if (rop == ">") {
    return make_unique<const GreaterThanExpr>(std::move(first_ae), std::move(second_ae));
  } else if (rop == ">=") {
    return make_unique<const GreaterThanEqualToExpr>(std::move(first_ae), std::move(second_ae));
  } else /*(rop == "==")*/ {
    return make_unique<const EqualToExpr>(std::move(first_ae), std::move(second_ae));
  }
}
