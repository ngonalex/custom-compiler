#include "frontend/combinators/v1_to_v5_combinators/helpers/var_helper.h"
#include "frontend/combinators/basic_combinators/and_combinator.h"
#include "frontend/combinators/basic_combinators/atom_parser.h"
#include "frontend/combinators/basic_combinators/one_or_more_combinator.h"
#include "frontend/combinators/v1_to_v5_combinators/helpers/v1_helpers.h"
#include "frontend/combinators/v1_to_v5_combinators/main/num_parser.h"
#include "frontend/combinators/v1_to_v5_combinators/main/single_char.h"
#include "frontend/combinators/v1_to_v5_combinators/main/word_parser.h"

#include <string>  // std::string, std::stoi

#define super NullParser

namespace cs160 {
namespace frontend {

ParseStatus VarKeywordParser::do_parse(std::string inputProgram,
                                       int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  std::string errorMessage = "Start variable declaration with var";

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter, errorMessage);
  }

  auto vParser = AtomParser('v');
  auto aParser = AtomParser('a');
  auto rParser = AtomParser('r');
  AndCombinator andOne;
  andOne.firstParser = reinterpret_cast<NullParser *>(&vParser);
  andOne.secondParser = reinterpret_cast<NullParser *>(&aParser);
  AndCombinator andTwo;
  andTwo.firstParser = reinterpret_cast<NullParser *>(&andOne);
  andTwo.secondParser = reinterpret_cast<NullParser *>(&rParser);

  ParseStatus result = andTwo.do_parse(inputProgram, endCharacter);
  result.errorType = errorMessage;
  return result;
}

ParseStatus ColonParser::do_parse(std::string inputProgram,
                                  int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  std::string errorMessage = "Missing colon";

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter, errorMessage);
  }

  auto vParser = AtomParser(':');
  ParseStatus result = vParser.do_parse(inputProgram, endCharacter);
  result.errorType = errorMessage;

  return result;
}

ParseStatus TypeParser::do_parse(std::string inputProgram, int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  std::string errorMessage = "Incorrect type in variable declaration";
  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter, errorMessage);
  }

  OneOrMoreCombinator oneOrMore;
  SingleCharParser charParser;

  oneOrMore.parser = reinterpret_cast<NullParser *>(&charParser);
  ParseStatus result = oneOrMore.do_parse(inputProgram, endCharacter);

  if (!result.status) {
    result.errorType = errorMessage;
  }

  return result;
}

ParseStatus EqualSignParser::do_parse(std::string inputProgram,
                                      int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  std::string errorMessage = "Missing equal sign";

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter, errorMessage);
  }

  auto vParser = AtomParser('=');
  ParseStatus result = vParser.do_parse(inputProgram, endCharacter);
  result.errorType = errorMessage;

  return result;
}

ParseStatus HelperVariableParser::do_parse(std::string inputProgram,
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
  ParseStatus result = thirdAnd.do_parse(inputProgram, endCharacter);

  if (result.status) {
    result.ast = std::move(intermediateValue.second_ast);
  }

  return result;
}

// ;
ParseStatus SemiColonParser::do_parse(std::string inputProgram,
                                      int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);
  std::string errorMessage = "Expecting ;";

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter, errorMessage);
  }

  auto atomParser = AtomParser(';');
  auto result = atomParser.do_parse(inputProgram, endCharacter);
  return result;
}

}  // namespace frontend
}  // namespace cs160