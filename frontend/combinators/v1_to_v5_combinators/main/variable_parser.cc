#include "frontend/combinators/v1_to_v5_combinators/main/variable_parser.h"
#include "frontend/combinators/basic_combinators/and_combinator.h"
#include "frontend/combinators/v1_to_v5_combinators/helpers/v1_helpers.h"
#include "frontend/combinators/v1_to_v5_combinators/helpers/var_helper.h"
#include "frontend/combinators/v1_to_v5_combinators/main/term_expr.h"
#include "frontend/combinators/v1_to_v5_combinators/main/word_parser.h"

#include <string>  // std::string, std::stoi

#define super NullParser

namespace cs160 {
namespace frontend {

ParseStatus VariableParser::do_parse(std::string inputProgram,
                                     int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter);
  }

  VarKeywordParser varParser;
  WordParser wordParser;
  ColonParser colonParser;
  TypeParser typeParser;
  SemiColonParser semiColonP;

  AndCombinator firstAnd;
  firstAnd.firstParser = reinterpret_cast<NullParser *>(&varParser);
  firstAnd.secondParser = reinterpret_cast<NullParser *>(&wordParser);
  ParseStatus intermediateValue =
      firstAnd.do_parse(inputProgram, endCharacter);  // Will be used in cache
  AndCombinator secondAnd;
  secondAnd.firstParser = reinterpret_cast<NullParser *>(&firstAnd);
  secondAnd.secondParser = reinterpret_cast<NullParser *>(&colonParser);
  AndCombinator thirdAnd;
  thirdAnd.firstParser = reinterpret_cast<NullParser *>(&secondAnd);
  thirdAnd.secondParser = reinterpret_cast<NullParser *>(&typeParser);
  AndCombinator fourthAnd;
  fourthAnd.firstParser = reinterpret_cast<NullParser *>(&thirdAnd);
  fourthAnd.secondParser = reinterpret_cast<NullParser *>(&semiColonP);
  ParseStatus result = fourthAnd.do_parse(inputProgram, endCharacter);

  if (result.status) {
    result.ast = std::move(intermediateValue.second_ast);
  }

  return result;
}

}  // namespace frontend
}  // namespace cs160