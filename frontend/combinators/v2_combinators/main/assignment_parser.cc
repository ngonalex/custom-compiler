#include "frontend/combinators/v2_combinators/main/variable_parser.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"
#include "frontend/combinators/basic_combinators/zero_or_more_combinator.h"
#include "frontend/combinators/v1_combinators/single_char.h"
#include "frontend/combinators/v1_combinators/single_digit.h"
#include "frontend/combinators/v1_combinators/term_expr.h"
#include "frontend/combinators/v2_combinators/helpers/word_parser.h"
#include "frontend/combinators/v2_combinators/helpers/var_helper.h"
#include <string> // std::string, std::stoi

#define super NullParser

using namespace cs160::frontend;
using namespace std;

ParseStatus VariableParser::parse(std::string inputProgram) {
  if (inputProgram.size() == 0) {
    return super::parse(inputProgram);
  }

  trim(inputProgram);

  VarKeywordParser varParser;
  WordParser wordParser;
  ColonParser colonParser;
  TypeParser typeParser;
  EqualSignParser equalSignParser;  // Optional
  TermExprParser termExprParser;   // Optional

  // Parse the first character
  ParseStatus result = varParser.parse(inputProgram);

  if (result.status) {
    ParseStatus wordResult = wordParser.parse(result.remainingCharacters);
    if (wordResult.status) {
      result.parsedCharacters += (" " + wordResult.parsedCharacters);
      result.remainingCharacters = wordResult.remainingCharacters;
      ParseStatus colonStatus = colonParser.parse(result.remainingCharacters);

      if (colonStatus.status) {
        result.parsedCharacters += (" " + colonStatus.parsedCharacters);
        result.remainingCharacters = colonStatus.remainingCharacters;
        ParseStatus typeStatus =
            typeParser.parse(colonStatus.remainingCharacters);
        if (typeStatus.status) {
          result.parsedCharacters += (" " + typeStatus.parsedCharacters);
          result.remainingCharacters = typeStatus.remainingCharacters;

          std::unique_ptr<const ArithmeticExpr> variableName =
              make_unique<VariableExpr>(wordResult.parsedCharacters);
          result.ast = std::move(make_unique<const VariableExpr>(
              unique_cast<const ArithmeticExpr>(std::move(variableName)),
              unique_cast<const ArithmeticExpr>(std::move(result.ast))));
        }
      } else
          return colonStatus;
    } else
      return wordResult;
  } else
    return result;

  return result;
}
