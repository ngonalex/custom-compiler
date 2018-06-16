#include "frontend/combinators/v1_to_v5_combinators/main/assignment_parser.h"
#include "frontend/combinators/basic_combinators/and_combinator.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"
#include "frontend/combinators/v1_to_v5_combinators/helpers/var_helper.h"
#include "frontend/combinators/v1_to_v5_combinators/main/ae.h"
#include "frontend/combinators/v1_to_v5_combinators/main/dereference_parser.h"
#include "frontend/combinators/v1_to_v5_combinators/main/word_parser.h"

#include <stdio.h>
#include <string>  // std::string, std::stoi

#define super NullParser

namespace cs160 {
namespace frontend {

ParseStatus AssignmentParser::do_parse(std::string inputProgram,
                                       int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter);
  }

  // Parsers used
  HelperVariableParser varParser;
  DereferenceParser dereferenceParser;
  WordParser wordParser;
  OrCombinator varOrTuple;  // Left of equal can be variable instantiation or
                            // variable_name
  varOrTuple.firstParser = reinterpret_cast<NullParser *>(&varParser);
  varOrTuple.secondParser = reinterpret_cast<NullParser *>(&dereferenceParser);

  OrCombinator varOrTupleOrWord;
  varOrTupleOrWord.firstParser = &varOrTuple;
  varOrTupleOrWord.secondParser = &wordParser;

  EqualSignParser equalSignParser;

  ArithExprParser arithExprParser;
  SemiColonParser semiColon;

  AndCombinator aeSemi;
  aeSemi.firstParser = reinterpret_cast<NullParser *>(&arithExprParser);
  aeSemi.secondParser = reinterpret_cast<NullParser *>(&semiColon);

  // Grammar declaration
  AndCombinator firstAnd;
  firstAnd.firstParser = reinterpret_cast<NullParser *>(&varOrTupleOrWord);
  firstAnd.secondParser = reinterpret_cast<NullParser *>(&equalSignParser);

  ParseStatus firstResult = firstAnd.do_parse(
      inputProgram, endCharacter);  // Will be used as cache result for second

  AndCombinator secondAnd;
  secondAnd.firstParser = reinterpret_cast<NullParser *>(&firstAnd);
  secondAnd.secondParser = reinterpret_cast<NullParser *>(&aeSemi);

  ParseStatus result = secondAnd.do_parse(inputProgram, endCharacter);

  if (result.status) {
    /*
    if (firstResult.firstOrSecond) {
        std::string temp = firstResult.parsedCharacters.erase(0,3);
         std::unique_ptr<const AstNode> temp_ast =
          std::move(make_unique<const VariableExpr>(temp));

      result.ast = std::move(make_unique<const Assignment>(
        unique_cast<const VariableExpr>(std::move(temp_ast)),
        unique_cast<const ArithmeticExpr>(std::move(result.second_ast))));
    }
    else {*/
    result.ast = std::move(make_unique<const AssignmentFromArithExp>(
        unique_cast<const VariableExpr>(std::move(firstResult.ast)),
        unique_cast<const ArithmeticExpr>(std::move(result.second_ast))));
    //}
  }

  return result;
}

}  // namespace frontend
}  // namespace cs160
