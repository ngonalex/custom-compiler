#include "frontend/combinators/basic_combinators/one_or_more_combinator.h"
#include "iostream"

#define super NullParser

using namespace cs160::frontend;

ParseStatus OneOrMoreCombinator::do_parse(std::string inputProgram,
               int startCharacter) {
  ParseStatus pStatus = parser->do_parse(inputProgram, startCharacter);

    if (pStatus.ast != NULL)
        pStatus.astNodes.push_back(std::move(pStatus.ast));
    if (pStatus.second_ast != NULL)
        pStatus.astNodes.push_back(std::move(pStatus.second_ast));
    pStatus.parsedCharactersArray.push_back(pStatus.parsedCharacters);
    
  if (!pStatus.status) {
    return pStatus;
  }

  while (pStatus.status) {
    ParseStatus pStatus2 = parser->do_parse(pStatus.remainingCharacters, pStatus.endCharacter);
    pStatus.status = pStatus2.status;
      if (pStatus2.status){
          pStatus.parsedCharacters += (pStatus2.parsedCharacters);
          pStatus.parsedCharactersArray.push_back(pStatus2.parsedCharacters);
          pStatus.remainingCharacters = pStatus2.remainingCharacters;
          pStatus.endCharacter = pStatus2.endCharacter;
          if (pStatus2.ast != NULL) {
              pStatus.astNodes.push_back(std::move(pStatus2.ast));
          }
          if (pStatus2.second_ast != NULL){
              pStatus2.astNodes.push_back(std::move(pStatus2.second_ast));
          }
      }
  }

  if (pStatus.parsedCharacters != "") pStatus.status = true;

  return pStatus;
}