#include "frontend/combinators/v3_combinators/helpers/function_helpers.h"
#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/and_combinator.h"
#include "frontend/combinators/basic_combinators/atom_parser.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"
#include "frontend/combinators/basic_combinators/zero_or_more_combinator.h"
#include "frontend/combinators/v3_combinators/main/ae.h"
#include "frontend/combinators/v3_combinators/helpers/var_helper.h"
#include "frontend/combinators/v3_combinators/main/word_parser.h"

#define super NullParser

using namespace cs160::frontend;

ParseStatus CommaOp::do_parse(std::string inputProgram, int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter, "Expected ','");
  }

  auto opParser = AtomParser(',');

  return opParser.do_parse(inputProgram, endCharacter);
}

ParseStatus FuncKeyword::do_parse(std::string inputProgram,
                                  int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter, "Expected func operator");
  }

  auto fParser = AtomParser('f');
  auto uParser = AtomParser('u');
  auto nParser = AtomParser('n');
  auto cParser = AtomParser('c');
  AndCombinator andOne;
  andOne.firstParser = &fParser;
  andOne.secondParser = &uParser;
  AndCombinator andTwo;
  andTwo.firstParser = &andOne;
  andTwo.secondParser = &nParser;
  AndCombinator andThree;
  andThree.firstParser = &andTwo;
  andThree.secondParser = &cParser;

  ParseStatus result = andThree.do_parse(inputProgram, endCharacter);
  return result;
}

ParseStatus ReturnTypeOp::do_parse(std::string inputProgram,
                                   int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() <= 1) {
    return super::fail(inputProgram, endCharacter, "Expected '->'");
  }

  auto returnParser_1 = AtomParser('-');
  auto returnParser_2 = AtomParser('>');
  AndCombinator return_and;
  return_and.firstParser = &returnParser_1;
  return_and.secondParser = &returnParser_2;

  return return_and.do_parse(inputProgram, endCharacter);
}

ParseStatus FunctionVariableParsers::do_parse(std::string inputProgram,
                                              int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter,
                       "Expected function variable declaration");
  }

  WordParser wordParser;
  ColonParser colonParser;
  TypeParser typeParser;
  CommaOp commaOp;

  AndCombinator firstAnd;
  firstAnd.firstParser = &wordParser;
  firstAnd.secondParser = &colonParser;

  AndCombinator secondAnd;
  secondAnd.firstParser = &firstAnd;
  secondAnd.secondParser = &typeParser;

  AndCombinator thirdAnd;
  thirdAnd.firstParser = &secondAnd;
  thirdAnd.secondParser = &commaOp;

  ZeroOrMoreCombinator intermediateParser;
  intermediateParser.parser = &thirdAnd;

  ParseStatus intermediateValue =
      intermediateParser.do_parse(inputProgram, endCharacter);

  ParseStatus lastValue = secondAnd.do_parse(
      intermediateValue.remainingCharacters, intermediateValue.endCharacter);

  intermediateValue.status = true;
  intermediateValue.remainingCharacters = lastValue.remainingCharacters;
  intermediateValue.parsedCharacters += lastValue.parsedCharacters;
  intermediateValue.endCharacter = lastValue.endCharacter;
  intermediateValue.astNodes.push_back(std::move(lastValue.ast));

  intermediateValue.parsedCharactersArray.erase(
      std::begin(intermediateValue.parsedCharactersArray),
      std::end(intermediateValue.parsedCharactersArray));

  return intermediateValue;
}

ParseStatus FunctionArgumentParsers::do_parse(std::string inputProgram,
                                              int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter,
                       "Expected function variable declaration");
  }

  ArithExprParser arithExpr;
  CommaOp commaOp;

  AndCombinator argumentParser;
  argumentParser.firstParser = &arithExpr;
  argumentParser.secondParser = &commaOp;

  ZeroOrMoreCombinator intermediateParser;
  intermediateParser.parser = &argumentParser;

  ParseStatus intermediateValue =
      intermediateParser.do_parse(inputProgram, endCharacter);

  ParseStatus lastValue = arithExpr.do_parse(
      intermediateValue.remainingCharacters, intermediateValue.endCharacter);

  intermediateValue.status = true;
  intermediateValue.remainingCharacters = lastValue.remainingCharacters;
  intermediateValue.parsedCharacters += lastValue.parsedCharacters;
  intermediateValue.endCharacter = lastValue.endCharacter;
  intermediateValue.astNodes.push_back(std::move(lastValue.ast));

  intermediateValue.parsedCharactersArray.erase(
      std::begin(intermediateValue.parsedCharactersArray),
      std::end(intermediateValue.parsedCharactersArray));

  return intermediateValue;
}
