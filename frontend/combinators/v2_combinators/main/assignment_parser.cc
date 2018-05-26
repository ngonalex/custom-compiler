#include "frontend/combinators/v2_combinators/main/assignment_parser.h"
#include "frontend/combinators/v2_combinators/main/variable_parser.h"
#include "frontend/combinators/v1_combinators/term_expr.h"
#include "frontend/combinators/v2_combinators/helpers/var_helper.h"
#include "frontend/combinators/v2_combinators/helper/word_parser.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"

#include <string> // std::string, std::stoi

#define super NullParser

using namespace cs160::frontend;
using namespace std;

ParseStatus AssignmentParser::parse(std::string inputProgram) {
  if (inputProgram.size() == 0) {
    return super::parse(inputProgram);
  }

  trim(inputProgram);

  VariableParser varParser;
  WordParser wordParser;
  OrCombinator orCombinator; // Left of equal can be variable instantiation or variable_name
  orCombinator->firstParser = varParser;
  orCombinator->secondParser = wordParser;

  EqualSignParser equalSignParser;
  TermExprParser termExprParser;

  // Parse the first expression
  for(int i = 0; i < inputProgram.length; i++) {
    ParseStatus result = orCombinator.parse(inputProgram);
    if(result.status)
      break;
  }

  if(result.status) {
    ParseStatus equalSignStatus = equalSignParser.parse(result.remainingCharacters);
    if(equalSignStatus.status) {
      result.parsedCharacters += (" " + equalSignStatus.parsedCharacters);
      result.remainingCharacters = equalSignStatus.remainingCharacters;
      ParseStatus termStatus = termExprParser.parse(result.remainingCharacters);
      if(termStatus.status) {
        result.parsedCharacters += (" " + termStatus.parsedCharacters);
        result.remainingCharacters = termStatus.remainingCharacters;
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

  return result;
}
