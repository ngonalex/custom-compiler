#include "frontend/combinators/basic_combinators/and_combinator.h"

#define super NullParser

namespace cs160 {
namespace frontend {

ParseStatus AndCombinator::do_parse(std::string inputProgram,
                                    int startCharacter) {
  ParseStatus firstStatus = firstParser->do_parse(inputProgram, startCharacter);

  if (!firstStatus.status) {
    return firstStatus;
  }

  ParseStatus secondStatus = secondParser->do_parse(
      firstStatus.remainingCharacters, firstStatus.endCharacter);

  if (!secondStatus.status) {
    secondStatus.startCharacter = startCharacter;
    secondStatus.endCharacter = startCharacter;
    return secondStatus;
  }

  ParseStatus both;
  both.status = true;
  both.parsedCharacters =
      firstStatus.parsedCharacters + secondStatus.parsedCharacters;
  size_t parsedCharacterCount = firstStatus.parsedCharactersArray.size() +
                                secondStatus.parsedCharactersArray.size();
  if (parsedCharacterCount > 0) {
    if (firstStatus.parsedCharactersArray.size() > 0)
      both.parsedCharactersArray.insert(
          std::end(both.parsedCharactersArray),
          std::begin(firstStatus.parsedCharactersArray),
          std::end(firstStatus.parsedCharactersArray));
    else if (firstStatus.parsedCharacters.size() > 0)
      both.parsedCharactersArray.push_back(firstStatus.parsedCharacters);
    if (secondStatus.parsedCharactersArray.size() > 0)
      both.parsedCharactersArray.insert(
          std::end(both.parsedCharactersArray),
          std::begin(secondStatus.parsedCharactersArray),
          std::end(secondStatus.parsedCharactersArray));
    else if (secondStatus.parsedCharacters.size() > 0)
      both.parsedCharactersArray.push_back(secondStatus.parsedCharacters);
  } else {
    both.firstParsedCharacters = firstStatus.parsedCharacters;
    both.secondParsedCharacters = secondStatus.parsedCharacters;
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
  size_t nodeCount = firstStatus.astNodes.size() + secondStatus.astNodes.size();
  if (noBackwardsCompat) {
    size_t nodeCount_noArr = 0;
    if (firstStatus.ast != NULL) nodeCount_noArr++;
    if (firstStatus.second_ast != NULL) nodeCount_noArr++;
    if (secondStatus.ast != NULL) nodeCount_noArr++;
    if (secondStatus.second_ast != NULL) nodeCount_noArr++;
    if (nodeCount_noArr > 2 && nodeCount == 0) nodeCount = nodeCount_noArr;
  }

  if (nodeCount > 0) {
    if (firstStatus.astNodes.size() > 0) {
      for (int i = 0; i < firstStatus.astNodes.size(); i++) {
        both.astNodes.push_back(std::move(firstStatus.astNodes[i]));
      }
    } else {
      if (firstStatus.ast != NULL) {
        both.astNodes.push_back(std::move(firstStatus.ast));
      }
      if (firstStatus.second_ast != NULL) {
        both.astNodes.push_back(std::move(firstStatus.second_ast));
      }
    }
    if (secondStatus.astNodes.size() > 0) {
      for (int i = 0; i < secondStatus.astNodes.size(); i++) {
        both.astNodes.push_back(std::move(secondStatus.astNodes[i]));
      }
    } else {
      if (secondStatus.ast != NULL) {
        both.astNodes.push_back(std::move(secondStatus.ast));
      }
      if (secondStatus.second_ast != NULL) {
        both.astNodes.push_back(std::move(secondStatus.second_ast));
      }
    }
  } else {
    if (noBackwardsCompat) {
      std::vector<std::unique_ptr<const AstNode>> asts;
      if (firstStatus.ast != NULL) asts.push_back(std::move(firstStatus.ast));
      if (firstStatus.second_ast != NULL)
        asts.push_back(std::move(firstStatus.second_ast));
      if (secondStatus.ast != NULL) asts.push_back(std::move(secondStatus.ast));
      if (secondStatus.second_ast != NULL)
        asts.push_back(std::move(secondStatus.second_ast));
      if (asts.size() > 0) both.ast = std::move(asts[0]);
      if (asts.size() > 1) both.second_ast = std::move(asts[1]);
    } else {
      if (firstStatus.ast != NULL)
        both.ast = std::move(firstStatus.ast);
      else if (firstStatus.second_ast != NULL)
        both.ast = std::move(firstStatus.second_ast);
      if (secondStatus.ast != NULL)
        both.second_ast = std::move(secondStatus.ast);
      else if (secondStatus.second_ast != NULL)
        both.ast = std::move(secondStatus.second_ast);
    }
  }

  return both;
}

}  // namespace frontend
}  // namespace cs160
