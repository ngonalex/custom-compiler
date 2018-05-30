#include "frontend/combinators/basic_combinators/one_or_more_combinator.h"
#include "iostream"

#define super NullParser

using namespace cs160::frontend;

ParseStatus OneOrMoreCombinator::parse(std::string inputProgram,
               int startCharacter,
				       std::string errorType) {
  ParseStatus pStatus = parser->parse(inputProgram, startCharacter);

  if (!pStatus.status) {
    return pStatus;
  }

  while (pStatus.status) {
    ParseStatus pStatus2 = parser->parse(pStatus.remainingCharacters, pStatus.endCharacter);
    pStatus.status = pStatus2.status;
    pStatus.parsedCharacters += (pStatus2.parsedCharacters);
    pStatus.remainingCharacters = pStatus2.remainingCharacters;
    pStatus.endCharacter = pStatus2.endCharacter;
    if (pStatus2.status && pStatus2.ast != NULL) {
      pStatus.astNodes.push_back(std::move(pStatus2.ast));
    }
  }

  if (pStatus.parsedCharacters != "") pStatus.status = true;

  return pStatus;
}