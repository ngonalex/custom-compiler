#include "frontend/combinators/v1_to_v5_combinators/main/function_call_parser.h"
#include "frontend/combinators/basic_combinators/and_combinator.h"
#include "frontend/combinators/v1_to_v5_combinators/helpers/function_helpers.h"
#include "frontend/combinators/v1_to_v5_combinators/helpers/relational_helper.h"
#include "frontend/combinators/v1_to_v5_combinators/helpers/v1_helpers.h"
#include "frontend/combinators/v1_to_v5_combinators/helpers/var_helper.h"
#include "frontend/combinators/v1_to_v5_combinators/main/block_parser.h"
#include "frontend/combinators/v1_to_v5_combinators/main/word_parser.h"

#define super NullParser

namespace cs160 {
namespace frontend {

ParseStatus FunctionCallParser::do_parse(std::string inputProgram,
                                         int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter);
  }

  WordParser wordParser;
  EqualSignParser equalSignParser;
  WordParser funcName;
  OpenParenParser openParenP;
  FunctionArgumentParsers funcArgs;
  CloseParenParser closeParenP;

  AndCombinator func1;
  func1.firstParser = &wordParser;
  func1.secondParser = &equalSignParser;
  func1.noBackwardsCompat = true;

  AndCombinator func2;
  func2.firstParser = &func1;
  func2.secondParser = &funcName;
  func2.noBackwardsCompat = true;

  AndCombinator func3;
  func3.firstParser = &func2;
  func3.secondParser = &openParenP;
  func3.noBackwardsCompat = true;

  AndCombinator func4;
  func4.firstParser = &func3;
  func4.secondParser = &funcArgs;
  func4.noBackwardsCompat = true;

  AndCombinator func5;
  func5.firstParser = &func4;
  func5.secondParser = &closeParenP;
  func5.noBackwardsCompat = true;

  ParseStatus result = func5.do_parse(inputProgram, endCharacter);

  if (result.status) {
    VariableExpr *funcNameExpr = (VariableExpr *)result.astNodes[1].get();

    const std::string funcNameStr = funcNameExpr->name();
    std::vector<std::unique_ptr<const ArithmeticExpr>> arguments;
    for (int i = 2; i < result.astNodes.size(); i++) {
      if (result.astNodes[i] != NULL)
        arguments.push_back(
            unique_cast<const ArithmeticExpr>(std::move(result.astNodes[i])));
    }
    result.ast = make_unique<const FunctionCall>(
        unique_cast<const VariableExpr>(std::move(result.astNodes[0])),
        funcNameStr, std::move(arguments));
    result.astNodes.erase(std::begin(result.astNodes),
                          std::end(result.astNodes));
  }

  return result;
}

}  // namespace frontend
}  // namespace cs160