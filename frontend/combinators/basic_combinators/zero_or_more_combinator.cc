#include "frontend/combinators/basic_combinators/zero_or_more_combinator.h"

#define super NullParser

using namespace cs160::frontend;

ParseStatus ZeroOrMoreCombinator::do_parse(std::string inputProgram,
                                           int startCharacter) {
  ParseStatus pStatus;
  pStatus.status = true;
  pStatus.remainingCharacters = inputProgram;
  pStatus.startCharacter = startCharacter;
  pStatus.endCharacter = startCharacter;

  while (pStatus.status) {
    ParseStatus status2 =
        parser->do_parse(pStatus.remainingCharacters, pStatus.endCharacter);
    pStatus.status = status2.status;
    if (status2.status) {
      pStatus.parsedCharacters += status2.parsedCharacters;
      pStatus.parsedCharactersArray.insert(
          std::end(pStatus.parsedCharactersArray),
          std::begin(status2.parsedCharactersArray),
          std::end(status2.parsedCharactersArray));
      pStatus.remainingCharacters = status2.remainingCharacters;
      pStatus.endCharacter = status2.endCharacter;
      if (status2.ast != NULL) {
        pStatus.astNodes.push_back(std::move(status2.ast));
      }
    }
  }

  pStatus.status = true;

  return pStatus;
}
