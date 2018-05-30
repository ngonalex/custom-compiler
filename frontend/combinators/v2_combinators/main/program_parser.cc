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

  if (inputProgram.size() == 0) {
    return super::parse(inputProgram);
  }

  ArithExprParser arithExprParser;
  AssignmentParser assignParser;
  ZeroOrMoreCombinator zeroOrMore; 

  zeroOrMore.parser = reinterpret_cast<NullParser *>(&assignParser);
  ParseStatus result;

  // Parse the assignments at the beginning
  ParseStatus assignResult = zeroOrMore.parse(inputProgram);
  result.status = assignResult.status;
  if(result.status) {
    result.parsedCharacters += assignResult.parsedCharacters;
    result.remainingCharacters = assignResult.remainingCharacters;
  }
  else {
    return assignResult;
  }

  // Parse the arithmetic expression
  ParseStatus arithResult = arithExprParser.parse(result.remainingCharacters);
  if(arithResult.status) {
    result.parsedCharacters += (" " + arithResult.parsedCharacters);
    result.remainingCharacters = arithResult.remainingCharacters;
    
    
    std::vector<std::unique_ptr<const Assignment>> temporaryAssign; 

    for(auto i = assignResult.astNodes.begin(); i != assignResult.astNodes.end(); ++i) {
      temporaryAssign.push_back(unique_cast<const Assignment>(std::move(*i)));
    }

    result.ast = make_unique<const Program>(std::move(temporaryAssign),
    unique_cast<const ArithmeticExpr>(std::move(arithResult.ast)));
    return result;
  }
  else {
    // Cannot parse any arithmetic expressions
    assignResult.status = arithResult.status;
    assignResult.errorType = arithResult.errorType;
    return assignResult;
  }
}

