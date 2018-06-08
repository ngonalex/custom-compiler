#include "frontend/combinators/basic_combinators/and_combinator.h"

#define super NullParser

using namespace cs160::frontend;

ParseStatus AndCombinator::do_parse(std::string inputProgram, int startCharacter) {
  ParseStatus firstStatus = firstParser->do_parse(inputProgram, startCharacter);

  if (!firstStatus.status) {
    return firstStatus;
  }

  ParseStatus secondStatus =
      secondParser->do_parse(firstStatus.remainingCharacters, firstStatus.endCharacter);

  if (!secondStatus.status) {
    secondStatus.startCharacter = startCharacter;
    secondStatus.endCharacter = startCharacter;
    return secondStatus;
  }

/*
  ParseStatus both::success(secondStatus.remainingCharacters, 
    firstStatus.parsedCharacters + secondStatus.parsedCharacters, std::move(firstStatus.ast),
    std::move(secondStatus.ast), firstStatus.characterStart + secondStatus.characterStart);*/
  ParseStatus both;
  both.status = true;
  both.parsedCharacters =
      firstStatus.parsedCharacters + secondStatus.parsedCharacters;
    int parsedCharacterCount = firstStatus.parsedCharactersArray.size() + secondStatus.parsedCharactersArray.size();
    if (parsedCharacterCount > 0){
        if (firstStatus.parsedCharactersArray.size() > 0)
            both.parsedCharactersArray.insert(std::end(both.parsedCharactersArray), std::begin(firstStatus.parsedCharactersArray), std::end(firstStatus.parsedCharactersArray));
        else if (firstStatus.parsedCharacters.size() > 0)
            both.parsedCharactersArray.push_back(firstStatus.parsedCharacters);
        if (secondStatus.parsedCharactersArray.size() > 0)
            both.parsedCharactersArray.insert(std::end(both.parsedCharactersArray), std::begin(secondStatus.parsedCharactersArray), std::end(secondStatus.parsedCharactersArray));
        else if (secondStatus.parsedCharacters.size() > 0)
            both.parsedCharactersArray.push_back(secondStatus.parsedCharacters);
    }
  both.remainingCharacters = secondStatus.remainingCharacters;
  both.startCharacter = startCharacter;
  both.endCharacter = secondStatus.endCharacter;
/*  if(firstStatus.ast == NULL)
    both.ast = std::move(firstStatus.second_ast);
  else 
    both.ast = std::move(firstStatus.ast);
  
  if(secondStatus.ast == NULL)
    both.ast = std::move(secondStatus.second_ast);
  else 
    both.second_ast = std::move(secondStatus.ast);
*/    
  int nodeCount = firstStatus.astNodes.size() + secondStatus.astNodes.size();
  if (nodeCount > 0){
        if (firstStatus.astNodes.size() > 0){
            for (int i = 0; i < firstStatus.astNodes.size(); i++){
                both.astNodes.push_back(std::move(firstStatus.astNodes[i]));
            }
        } else if (firstStatus.ast != NULL){
            both.astNodes.push_back(std::move(firstStatus.ast));
        }
        if (secondStatus.astNodes.size() > 0){
            for (int i = 0; i < firstStatus.astNodes.size(); i++){
                both.astNodes.push_back(std::move(secondStatus.astNodes[i]));
            }
        } else if (secondStatus.ast != NULL){
            both.astNodes.push_back(std::move(secondStatus.ast));
        }
    } else {
        if (firstStatus.ast != NULL)
            both.ast = std::move(firstStatus.ast);
        if (secondStatus.ast != NULL)
            both.second_ast = std::move(secondStatus.ast);
    }

  return both;
}

// NUM and Operator
//