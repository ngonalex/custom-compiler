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
  both.remainingCharacters = secondStatus.remainingCharacters;
  both.startCharacter = startCharacter;
  both.endCharacter = secondStatus.endCharacter;
  both.ast = std::move(firstStatus.ast);
  both.second_ast = std::move(secondStatus.ast);

  return both;
}

// NUM and Operator
//
