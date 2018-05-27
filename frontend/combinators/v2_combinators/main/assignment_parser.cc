#include "frontend/combinators/v2_combinators/main/assignment_parser.h"
#include "frontend/combinators/v1_combinators/ae.h"
#include "frontend/combinators/v2_combinators/helpers/var_helper.h"
#include "frontend/combinators/v2_combinators/main/word_parser.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"

#include <string> // std::string, std::stoi
#include <stdio.h>

#define super NullParser

using namespace cs160::frontend;
using namespace std;

ParseStatus AssignmentParser::parse(std::string inputProgram, std::string errorType) {
  trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::parse(inputProgram);
  }

  HelperVariableParser varParser;
  WordParser wordParser;
  OrCombinator orCombinator; // Left of equal can be variable instantiation or variable_name
  orCombinator.firstParser = reinterpret_cast<NullParser *>(&varParser);
  orCombinator.secondParser = reinterpret_cast<NullParser *>(&wordParser);

  EqualSignParser equalSignParser;
  ArithExprParser termExprParser;

  ParseStatus result;

  // Parse the first expression
  ParseStatus varResult = orCombinator.parse(inputProgram);
  
  result.status = varResult.status;

  if(varResult.status) {
    result.parsedCharacters += varResult.parsedCharacters;
    result.remainingCharacters = varResult.remainingCharacters;
    ParseStatus equalSignStatus = equalSignParser.parse(result.remainingCharacters);

    if(equalSignStatus.status) {
      result.parsedCharacters += (" " + equalSignStatus.parsedCharacters);
      result.remainingCharacters = equalSignStatus.remainingCharacters;
      ParseStatus termStatus = termExprParser.parse(result.remainingCharacters);
      if(termStatus.status) {
        result.parsedCharacters += (" " + termStatus.parsedCharacters);
        result.remainingCharacters = termStatus.remainingCharacters;

        result.ast = std::move(make_unique<const Assignment>(
          unique_cast<const VariableExpr>(std::move(varResult.ast)),
          unique_cast<const ArithmeticExpr>(std::move(termStatus.ast))));
      }
      else {
        result.status = termStatus.status;
        result.errorType = termStatus.errorType;
      }
    }
    else {
      result.status = equalSignStatus.status;
      result.errorType = equalSignStatus.errorType;
    }
  }
  else {
    result.errorType = varResult.errorType;
  }

  return result;
}
