#include "frontend/combinators/v3_combinators/main/relation_body.h"
#include "frontend/combinators/v3_combinators/main/relation_parser.h"
#include "frontend/combinators/basic_combinators/and_combinator.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"
#include "frontend/combinators/v1_combinators/ae.h"
#include "frontend/combinators/v3_combinators/helpers/relational_helper.h"

#include <iostream>
#include <string>  // std::string, std::stoi

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

ParseStatus RelationBodyParser::parse(std::string inputProgram,
                                      int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter);
  }

  ArithExprParser firstAeParser;
  RelationOperatorParser relOpParser;
  ArithExprParser secondAeParser;

  LogicOperatorParser logOpParser;
  RelationParser relParser;

  auto firstAeResult = firstAeParser.parse(inputProgram, startCharacter);
  if (firstAeResult.status) {
    auto relResult = relOpParser.parse(firstAeResult.remainingCharacters,
                                       firstAeResult.endCharacter);
    std::cout << relResult.parsedCharacters << std::endl;
    if (relResult.status) {
      auto secondAeResult = secondAeParser.parse(relResult.remainingCharacters,
                                                 relResult.endCharacter);
      if (secondAeResult.status) {
        std::cout << secondAeResult.parsedCharacters << std::endl;
        secondAeResult.ast = make_node(
            unique_cast<const ArithmeticExpr>(std::move(firstAeResult.ast)),
            relResult.parsedCharacters,
            unique_cast<const ArithmeticExpr>(std::move(secondAeResult.ast)));

        // Case where there's more logic bits at the end
        auto logOpResult = logOpParser.parse(secondAeResult.remainingCharacters, secondAeResult.endCharacter);
        if (logOpResult.status) {
          auto relParserResult = relParser.parse(
              logOpResult.remainingCharacters, logOpResult.endCharacter);
          if (relParserResult.status) {
            relParserResult.ast =
                make_node(unique_cast<const RelationalExpr>(
                              std::move(secondAeResult.ast)),
                          logOpResult.parsedCharacters,
                          unique_cast<const RelationalExpr>(
                              std::move(relParserResult.ast)));
            return relParserResult;
          }
        } else {
          return secondAeResult;
        }
      }
    }
  }

  return super::fail(inputProgram, endCharacter);
}

// ae rop ae
std::unique_ptr<const RelationalBinaryOperator> RelationBodyParser::make_node(
    std::unique_ptr<const ArithmeticExpr> first_ae, std::string rop,
    std::unique_ptr<const ArithmeticExpr> second_ae) {
  if (rop == "<") {
    return make_unique<const LessThanExpr>(std::move(first_ae),
                                           std::move(second_ae));
  } else if (rop == "<=") {
    return make_unique<const LessThanEqualToExpr>(std::move(first_ae),
                                                  std::move(second_ae));
  } else if (rop == ">") {
    return make_unique<const GreaterThanExpr>(std::move(first_ae),
                                              std::move(second_ae));
  } else if (rop == ">=") {
    return make_unique<const GreaterThanEqualToExpr>(std::move(first_ae),
                                                     std::move(second_ae));
  } else if (rop == "==") {
    return make_unique<const EqualToExpr>(std::move(first_ae),
                                          std::move(second_ae));
  }
}

std::unique_ptr<const LogicalBinaryOperator> RelationBodyParser::make_node(
    std::unique_ptr<const RelationalExpr> first_re, std::string lop,
    std::unique_ptr<const RelationalExpr> second_re) {
  if (lop == "&&") {
    return make_unique<const LogicalAndExpr>(std::move(first_re),
                                             std::move(second_re));
  } else if (lop == "||") {
    return make_unique<const LogicalOrExpr>(std::move(first_re),
                                            std::move(second_re));
  }
}