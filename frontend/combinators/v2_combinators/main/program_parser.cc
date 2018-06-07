#include "frontend/combinators/v2_combinators/main/program_parser.h"
#include "frontend/combinators/v2_combinators/main/assignment_parser.h"
#include "frontend/combinators/v1_combinators/ae.h"
#include "frontend/combinators/v2_combinators/helpers/var_helper.h"
#include "frontend/combinators/v2_combinators/main/word_parser.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"
#include "frontend/combinators/basic_combinators/and_combinator.h"
#include "frontend/combinators/basic_combinators/zero_or_more_combinator.h"

#include <string> // std::string, std::stoi

#define super NullParser


using namespace cs160::frontend;
using namespace std;

ParseStatus ProgramParser::do_parse(std::string inputProgram, int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);
  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter, "Empty inside of ProgramParser");
  }

  ArithExprParser arithExprParser;
  AssignmentParser assignParser;
  ZeroOrMoreCombinator zeroOrMore; 

  zeroOrMore.parser = reinterpret_cast<NullParser *>(&assignParser);

  ParseStatus intermediateResult = zeroOrMore.do_parse(inputProgram, endCharacter);

  AndCombinator firstAnd;
  firstAnd.firstParser = reinterpret_cast<NullParser *>(&zeroOrMore);
  firstAnd.secondParser = reinterpret_cast<NullParser *>(&arithExprParser);

  ParseStatus result = firstAnd.do_parse(inputProgram, endCharacter);

  if(result.status) {
    std::vector<std::unique_ptr<const Statement>> temporaryAssign; 

    for(auto i = intermediateResult.astNodes.begin(); i != intermediateResult.astNodes.end(); ++i) {
      temporaryAssign.push_back(unique_cast<const Statement>(std::move(*i)));
    }

    result.ast = make_unique<const Program>(std::move(temporaryAssign),
    unique_cast<const ArithmeticExpr>(std::move(result.astNodes[intermediateResult.astNodes.size()])));  
  }
    
    result.parsedCharactersArray.erase(std::begin(result.parsedCharactersArray), std::end(result.parsedCharactersArray));
    result.astNodes.erase(std::begin(result.astNodes), std::end(result.astNodes));

  return result;

}
