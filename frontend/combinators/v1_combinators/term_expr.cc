#include "frontend/combinators/v1_combinators/term_expr.h"
#include "frontend/combinators/basic_combinators/and_combinator.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"
#include "frontend/combinators/v2_combinators/main/word_parser.h"

#include "frontend/combinators/v1_combinators/add_sub_expr.h"
#include "frontend/combinators/v1_combinators/helpers/v1_helpers.h"
#include "frontend/combinators/v1_combinators/num_parser.h"

#define super NullParser

using namespace cs160::frontend;
using namespace std;

ParseStatus TermExprParser::do_parse(std::string inputProgram,
                                     int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter);
  }

  // num
  WordParser wordP;
  NumParser num;
  OrCombinator wordOrNum;
  wordOrNum.firstParser = reinterpret_cast<NullParser *>(&wordP);
  wordOrNum.secondParser = reinterpret_cast<NullParser *>(&num);

  ParseStatus numParseStatus = wordOrNum.do_parse(inputProgram, endCharacter);
  // ParseStatus numParseStatus = num.do_parse(inputProgram);

  if (numParseStatus.status) {
    return numParseStatus;  // Returning Success on Num
  }

  // - ae
  NegativeParser negParser;
  AddSubExprParser ae;
  AndCombinator negAndAe;
  negAndAe.firstParser = reinterpret_cast<NullParser *>(&negParser);
  negAndAe.secondParser = reinterpret_cast<NullParser *>(&ae);
  ParseStatus tempResult = negAndAe.do_parse(inputProgram, endCharacter);

  // ( ae )
  OpenParenParser open_paren;
  CloseParenParser close_paren;

  AndCombinator openParenAndExpr;
  openParenAndExpr.firstParser = reinterpret_cast<NullParser *>(&open_paren);
  openParenAndExpr.secondParser = reinterpret_cast<NullParser *>(&ae);

  AndCombinator firstAndClose;
  firstAndClose.firstParser = reinterpret_cast<NullParser *>(&openParenAndExpr);
  firstAndClose.secondParser = reinterpret_cast<NullParser *>(&close_paren);

  // High Level Parse

  OrCombinator firstOr;
  firstOr.firstParser = reinterpret_cast<NullParser *>(&wordOrNum);
  firstOr.secondParser = reinterpret_cast<NullParser *>(&negAndAe);
  if (tempResult.status) {
    std::unique_ptr<const ArithmeticExpr> zero = make_unique<IntegerExpr>(0);
    tempResult.ast = std::move(make_unique<const SubtractExpr>(
        unique_cast<const ArithmeticExpr>(std::move(zero)),
        unique_cast<const ArithmeticExpr>(std::move(tempResult.second_ast))));
    return tempResult;
  }

  OrCombinator secondOr;
  secondOr.firstParser = reinterpret_cast<NullParser *>(&firstOr);
  secondOr.secondParser = reinterpret_cast<NullParser *>(&firstAndClose);
  ParseStatus aeResult = openParenAndExpr.do_parse(inputProgram, endCharacter);
  ParseStatus final_result = firstAndClose.do_parse(inputProgram, endCharacter);

  if (final_result.status) {
    final_result.ast = std::move(aeResult.second_ast);
  }

  return final_result;
}