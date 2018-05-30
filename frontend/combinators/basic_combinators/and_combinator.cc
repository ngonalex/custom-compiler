#include "frontend/combinators/basic_combinators/and_combinator.h"

#define super NullParser

using namespace cs160::frontend;

ParseStatus AndCombinator::parse(std::string inputProgram,
				 std::string errorType) {
  ParseStatus firstStatus = firstParser->parse(inputProgram);

  if (!firstStatus.status) {
    return firstStatus;
  }

  ParseStatus secondStatus =
      secondParser->parse(firstStatus.remainingCharacters);

  if (!secondStatus.status) {
    return secondStatus;
  }
/*
  ParseStatus both::andSuccess(secondStatus.remainingCharacters, 
  	firstStatus.parsedCharacters + secondStatus.parsedCharacters, std::move(firstStatus.ast),
  	std::move(secondStatus.ast), firstStatus.characterStart + secondStatus.characterStart);*/
  ParseStatus both;
  both.status = true;
  both.parsedCharacters =
      firstStatus.parsedCharacters + secondStatus.parsedCharacters;
  both.remainingCharacters = secondStatus.remainingCharacters;
  both.ast = std::move(firstStatus.ast);
  both.second_ast = std::move(secondStatus.ast);

  return both;
}

// NUM and Operator
//
