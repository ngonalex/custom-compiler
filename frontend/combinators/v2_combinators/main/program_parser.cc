
#include "frontend/combinators/v2_combinators/main/program_parser.h"
#include "frontend/combinators/v2_combinators/main/assignment_parser.h"
#include "frontend/combinators/v1_combinators/ae.h"
#include "frontend/combinators/v2_combinators/helpers/var_helper.h"
#include "frontend/combinators/v2_combinators/main/word_parser.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"
#include "frontend/combinators/basic_combinators/zero_or_more_combinator.h"

#include <string> // std::string, std::stoi

#define super NullParser

using namespace cs160::frontend;
using namespace std;

ParseStatus ProgramParser::parse(std::string inputProgram, std::string errorType) {
  trim(inputProgram);
  
/*
  if (inputProgram.size() == 0) {
    return super::parse(inputProgram);
  }

  AssignmentParser assignParser;

  ZeroOrMoreCombinator zeroOrMore; 
  zeroOrMore.parser = &assignParser;

  ArithExprParser termExprParser;

  // Parse the assignments at the beginning
  ParseStatus assignResult = zeroOrMore.parse(inputProgram);

  result.status = assignResult.status;

  if(result.status) {
    result.parsedCharacters += assignResult.parsedCharacters;
    result.remainingCharacters = assignResult.remainingCharacters;
    ParseStatus equalSignStatus = equalSignParser.parse(result.remainingCharacters);

    if(equalSignStatus.status) {
      result.parsedCharacters += (" " + equalSignStatus.parsedCharacters);
      result.remainingCharacters = equalSignStatus.remainingCharacters;
      ParseStatus termStatus = termExprParser.parse(result.remainingCharacters);
      if(termStatus.status) {
        result.parsedCharacters += (" " + termStatus.parsedCharacters);
        result.remainingCharacters = termStatus.remainingCharacters;

        result.ast = std::move(make_unique<const Assignment>(
          unique_cast<const VariableExpr>(std::move(assignResult.ast)),
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

  return result;*/
}