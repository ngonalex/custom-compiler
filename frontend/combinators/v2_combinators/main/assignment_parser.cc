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

  ParseStatus result;

  // Parsers used
  HelperVariableParser varParser;
  WordParser wordParser;
  OrCombinator varOrWord; // Left of equal can be variable instantiation or variable_name
  varOrWord.firstParser = reinterpret_cast<NullParser *>(&varParser);
  varOrWord.secondParser = reinterpret_cast<NullParser *>(&wordParser);

  EqualSignParser equalSignParser;
  ArithExprParser arithExprParser;

  // Grammar declaration
  AndCombinator firstAnd;
  firstAnd.firstParser = reinterpret_cast<NullParser *>(&varOrWord);
  firstAnd.secondParser = reinterpret_cast<NullParser *>(&equalSignParser);

  AndCombinator secondAnd;
  secondAnd.firstParser = reinterpret_cast<NullParser *>(&firstAnd);
  secondAnd.secondParser = reinterpret_cast<NullParser *>(&arithExprParser);


  ParseStatus result::sucess(secondAnd.remainingCharacters, 
    secondAnd.parsedCharacters, std::move(make_unique<const Assignment>(
          unique_cast<const VariableExpr>(std::move(varResult.ast)),
          unique_cast<const ArithmeticExpr>(std::move(termStatus.ast)))), 
    secondAnd.characterStart);
      
  return result;
}
