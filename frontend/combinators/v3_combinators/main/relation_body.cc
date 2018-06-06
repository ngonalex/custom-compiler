#include "frontend/combinators/v1_combinators/ae.h"
#include "frontend/combinators/basic_combinators/and_combinator.h"
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

ParseStatus RelationBodyParser::parse(std::string inputProgram, int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter);
  }

  // First Arithmetic Expression, ROP
  ArithExprParser firstAeParser;
  RelationOperator relOpParser;
  ArithExprParser secondAeParser;

  auto firstAeResult = firstAeParser.parse(inputProgram, startCharacter);
  std::cout << firstAeResult.parsedCharacters << std::endl;
  if (firstAeResult.status) {
     auto relResult = relOpParser.parse(firstAeResult.remainingCharacters, firstAeResult.endCharacter);
      std::cout << relResult.parsedCharacters << std::endl;
    if (relResult.status) {
       auto secondAeResult = secondAeParser.parse(relResult.remainingCharacters, relResult.endCharacter);
       if (secondAeResult.status) {
         std::cout << secondAeResult.parsedCharacters << std::endl;
          secondAeResult.ast = 
            make_node(unique_cast<const ArithmeticExpr>(std::move(firstAeResult.ast)), 
            relResult.parsedCharacters, 
            unique_cast<const ArithmeticExpr>(std::move(secondAeResult.ast)));
          return secondAeResult; 
       }
    }
  }

  return super::fail(inputProgram, endCharacter);
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
  } else if (rop == "==") {
    return make_unique<const EqualToExpr>(std::move(first_ae), std::move(second_ae));
  }
}
