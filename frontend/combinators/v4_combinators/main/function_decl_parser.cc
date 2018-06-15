#include "frontend/combinators/v4_combinators/main/function_decl_parser.h"
#include "frontend/combinators/basic_combinators/and_combinator.h"
#include "frontend/combinators/v1_combinators/helpers/v1_helpers.h"
#include "frontend/combinators/v2_combinators/helpers/var_helper.h"
#include "frontend/combinators/v2_combinators/main/word_parser.h"
#include "frontend/combinators/v3_combinators/helpers/relational_helper.h"
#include "frontend/combinators/v3_combinators/main/block_parser.h"
#include "frontend/combinators/v4_combinators/helpers/function_helpers.h"

#define super NullParser

using namespace cs160::frontend;
using namespace std;

ParseStatus FunctionDeclParser::do_parse(std::string inputProgram,
                                         int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter);
  }

  FuncKeyword funcK;
  WordParser funcName;
  OpenParenParser openParenP;
  FunctionVariableParsers funcVariables;
  CloseParenParser closeParenP;

  ReturnTypeOp returnTypeOp;
  TypeParser typeParser;

  OpenCurlyBrackets openBracketP;
  BlockParser block;
  CloseCurlyBrackets closeBracketP;

  AndCombinator func1;
  func1.firstParser = &funcK;
  func1.secondParser = &funcName;

  AndCombinator func2;
  func2.firstParser = &func1;
  func2.secondParser = &openParenP;

  AndCombinator func3;
  func3.firstParser = &func2;
  func3.secondParser = &funcVariables;

  AndCombinator func4;
  func4.firstParser = &func3;
  func4.secondParser = &closeParenP;

  AndCombinator func5;
  func5.firstParser = &func4;
  func5.secondParser = &returnTypeOp;

  AndCombinator func6;
  func6.firstParser = &func5;
  func6.secondParser = &typeParser;

  AndCombinator func7;
  func7.firstParser = &func6;
  func7.secondParser = &openBracketP;

  AndCombinator func8;
  func8.firstParser = &func7;
  func8.secondParser = &block;

  AndCombinator func9;
  func9.firstParser = &func8;
  func9.secondParser = &closeBracketP;

  // TODO: Add AST Generation

  return func9.do_parse(inputProgram, endCharacter);
}
