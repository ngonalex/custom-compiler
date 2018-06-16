#include "frontend/combinators/v1_to_v5_combinators/main/program_parser.h"
#include "frontend/combinators/basic_combinators/and_combinator.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"
#include "frontend/combinators/basic_combinators/zero_or_more_combinator.h"
#include "frontend/combinators/v1_to_v5_combinators/helpers/var_helper.h"
#include "frontend/combinators/v1_to_v5_combinators/main/ae.h"
#include "frontend/combinators/v1_to_v5_combinators/main/assignment_parser.h"
#include "frontend/combinators/v1_to_v5_combinators/main/function_decl_parser.h"
#include "frontend/combinators/v1_to_v5_combinators/main/statement_parser.h"
#include "frontend/combinators/v1_to_v5_combinators/main/word_parser.h"

#include <string>  // std::string, std::stoi

#define super NullParser

using namespace cs160::frontend;
using namespace std;

ParseStatus ProgramParser::do_parse(std::string inputProgram,
                                    int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);
  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter,
                       "Empty inside of ProgramParser");
  }

  ArithExprParser arithExprParser;
  SemiColonParser semiColon;

  AndCombinator aeSemi;
  aeSemi.firstParser = &arithExprParser;
  aeSemi.secondParser = &semiColon;

  FunctionDeclParser functionDeclParser;
  ZeroOrMoreCombinator zeroOrMoreFunctionDecls;
  zeroOrMoreFunctionDecls.parser = &functionDeclParser;

  ParseStatus functionsResult =
      zeroOrMoreFunctionDecls.do_parse(inputProgram, endCharacter);

  StatementParser statementParser;
  ZeroOrMoreCombinator zeroOrMoreStatements;

  zeroOrMoreStatements.parser = &statementParser;

  ParseStatus statementsResult = zeroOrMoreStatements.do_parse(
      functionsResult.remainingCharacters, functionsResult.endCharacter);

  AndCombinator firstAnd;
  firstAnd.firstParser = &zeroOrMoreFunctionDecls;
  firstAnd.secondParser = &zeroOrMoreStatements;

  AndCombinator secondAnd;
  secondAnd.firstParser = &firstAnd;
  secondAnd.secondParser = &aeSemi;

  ParseStatus result = secondAnd.do_parse(inputProgram, endCharacter);

  if (result.status) {
    std::vector<std::unique_ptr<const FunctionDef>> temporaryFunctions;
    std::vector<std::unique_ptr<const Statement>> temporaryStatements;

    for (auto i = functionsResult.astNodes.begin();
         i != functionsResult.astNodes.end(); ++i) {
      temporaryFunctions.push_back(
          unique_cast<const FunctionDef>(std::move(*i)));
    }

    for (auto i = statementsResult.astNodes.begin();
         i != statementsResult.astNodes.end(); ++i) {
      temporaryStatements.push_back(
          unique_cast<const Statement>(std::move(*i)));
    }

    result.ast = make_unique<const Program>(
        std::move(temporaryFunctions), std::move(temporaryStatements),
        unique_cast<const ArithmeticExpr>(
            std::move(result.astNodes[statementsResult.astNodes.size()])));
  }

  result.parsedCharactersArray.erase(std::begin(result.parsedCharactersArray),
                                     std::end(result.parsedCharactersArray));
  result.astNodes.erase(std::begin(result.astNodes), std::end(result.astNodes));

  return result;
}
