#include "frontend/combinators/v2_combinators/helpers/var_helper.h"
#include "frontend/combinators/basic_combinators/one_or_more_combinator.h"
#include "frontend/combinators/v1_combinators/single_char.h"
#include "frontend/combinators/v2_combinators/main/word_parser.h"
#include "frontend/combinators/v1_combinators/num_parser.h"
#include "frontend/combinators/v1_combinators/helpers/v1_helpers.h"
#include "frontend/combinators/basic_combinators/and_combinator.h"
#include "frontend/combinators/basic_combinators/atom_parser.h"


#include <string>     // std::string, std::stoi

#define super NullParser

using namespace cs160::frontend;
using namespace std;

ParseStatus VarKeywordParser::parse(std::string inputProgram, int startCharacter) {
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

  ParseStatus result = andTwo.parse(inputProgram, endCharacter);
  result.errorType = errorMessage;
  return result;
}


ParseStatus ColonParser::parse(std::string inputProgram, int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  std::string errorMessage = "Missing colon";

	if (inputProgram.size() == 0) {
		return super::fail(inputProgram, endCharacter, errorMessage);
	}

  auto vParser = AtomParser(':');
  ParseStatus result = vParser.parse(inputProgram, endCharacter);
  result.errorType = errorMessage;

  return result;
}


ParseStatus TypeParser::parse(std::string inputProgram, int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  std::string errorMessage = "Incorrect type in variable declaration";
	if (inputProgram.size() == 0) {
		return super::fail(inputProgram, endCharacter, errorMessage);
	}

  OneOrMoreCombinator oneOrMore;
  SingleCharParser charParser;

	oneOrMore.parser = reinterpret_cast<NullParser *>(&charParser);
  ParseStatus result = oneOrMore.parse(inputProgram, endCharacter);

  if(!result.status) {
    result.errorType = errorMessage;
  }

  return result;
}

ParseStatus EqualSignParser::parse(std::string inputProgram, int startCharacter) {
   int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  std::string errorMessage = "Missing equal sign";

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter, errorMessage);
  }

  auto vParser = AtomParser('=');
  ParseStatus result = vParser.parse(inputProgram, endCharacter);
  result.errorType = errorMessage;

  return result;

}


ParseStatus HelperVariableParser::parse(std::string inputProgram, int startCharacter) {
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
  ParseStatus intermediateValue = firstAnd.parse(inputProgram, endCharacter); // Will be used in cache
  AndCombinator secondAnd;
  secondAnd.firstParser = reinterpret_cast<NullParser *>(&firstAnd);
  secondAnd.secondParser = reinterpret_cast<NullParser *>(&colonParser);
  AndCombinator thirdAnd;
  thirdAnd.firstParser = reinterpret_cast<NullParser *>(&secondAnd);
  thirdAnd.secondParser = reinterpret_cast<NullParser *>(&typeParser);
  ParseStatus result = thirdAnd.parse(inputProgram, endCharacter);

  if(result.status) {
    result.ast = std::move(intermediateValue.second_ast);
  }

  return result;
}
