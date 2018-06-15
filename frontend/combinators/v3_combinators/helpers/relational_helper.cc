#include "frontend/combinators/v3_combinators/helpers/relational_helper.h"
#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/and_combinator.h"
#include "frontend/combinators/basic_combinators/atom_parser.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"

#include <iostream>

#include <string>  // std::string, std::stoi

#define super NullParser

using namespace cs160::frontend;

std::string errorMessage = "Expected relational operator";

ParseStatus EqualToOpParser::do_parse(std::string inputProgram,
                                      int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() <= 1) {
    return super::fail(inputProgram, endCharacter, errorMessage);
  }

  auto equalParser_1 = AtomParser('=');
  auto equalParser_2 = AtomParser('=');
  AndCombinator equals_equals;
  equals_equals.firstParser = &equalParser_1;
  equals_equals.secondParser = &equalParser_2;

  return equals_equals.do_parse(inputProgram, endCharacter);
}

ParseStatus NotEqualToOpParser::do_parse(std::string inputProgram,
                                         int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() <= 1) {
    return super::fail(inputProgram, endCharacter, errorMessage);
  }

  auto equalParser_1 = AtomParser('!');
  auto equalParser_2 = AtomParser('=');
  AndCombinator equals_equals;
  equals_equals.firstParser = &equalParser_1;
  equals_equals.secondParser = &equalParser_2;

  return equals_equals.do_parse(inputProgram, endCharacter);
}

ParseStatus GreaterThanOpParser::do_parse(std::string inputProgram,
                                          int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() <= 0) {
    return super::fail(inputProgram, endCharacter, errorMessage);
  }

  auto parser = AtomParser('>');
  return parser.do_parse(inputProgram, endCharacter);
}

ParseStatus LessThanOpParser::do_parse(std::string inputProgram,
                                       int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() <= 0) {
    return super::fail(inputProgram, endCharacter, errorMessage);
  }

  auto parser = AtomParser('<');
  return parser.do_parse(inputProgram, endCharacter);
}

ParseStatus GreaterThanOrEqualToOpParser::do_parse(std::string inputProgram,
                                                   int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() <= 1) {
    return super::fail(inputProgram, endCharacter, errorMessage);
  }

  auto greaterThanParser = AtomParser('>');
  auto equalsParser = AtomParser('=');
  AndCombinator greater_or_equal;
  greater_or_equal.firstParser = &greaterThanParser;
  greater_or_equal.secondParser = &equalsParser;

  return greater_or_equal.do_parse(inputProgram, endCharacter);
}

ParseStatus LessThanOrEqualToOpParser::do_parse(std::string inputProgram,
                                                int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() <= 1) {
    return super::fail(inputProgram, endCharacter, errorMessage);
  }

  auto lessThanParser = AtomParser('<');
  auto equalsParser = AtomParser('=');
  AndCombinator less_or_equal;
  less_or_equal.firstParser = &lessThanParser;
  less_or_equal.secondParser = &equalsParser;

  return less_or_equal.do_parse(inputProgram, endCharacter);
}

ParseStatus AndOpParser::do_parse(std::string inputProgram,
                                  int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() <= 1) {
    return super::fail(inputProgram, endCharacter, errorMessage);
  }

  auto andParser_1 = AtomParser('&');
  auto andParser_2 = AtomParser('&');
  AndCombinator and_and;
  and_and.firstParser = &andParser_1;
  and_and.secondParser = &andParser_2;

  return and_and.do_parse(inputProgram, endCharacter);
}

ParseStatus OrOpParser::do_parse(std::string inputProgram, int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() <= 1) {
    return super::fail(inputProgram, endCharacter, errorMessage);
  }

  auto orParser_1 = AtomParser('|');
  auto orParser_2 = AtomParser('|');
  AndCombinator or_or;
  or_or.firstParser = &orParser_1;
  or_or.secondParser = &orParser_2;

  return or_or.do_parse(inputProgram, endCharacter);
}

ParseStatus NotOpParser::do_parse(std::string inputProgram,
                                  int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() <= 0) {
    return super::fail(inputProgram, endCharacter, errorMessage);
  }

  auto parser = AtomParser('!');
  return parser.do_parse(inputProgram, endCharacter);
}

// ROP
ParseStatus RelationOperatorParser::do_parse(std::string inputProgram,
                                             int startCharacter) {
  EqualToOpParser equalTo;
  NotEqualToOpParser notEqualTo;

  GreaterThanOrEqualToOpParser greaterOrEqual;
  LessThanOrEqualToOpParser lessOrEqual;

  GreaterThanOpParser greaterThan;
  LessThanOpParser lessThan;
  // > or <
  OrCombinator greaterOrLess;
  greaterOrLess.firstParser = &greaterThan;
  greaterOrLess.secondParser = &lessThan;
  // >= or <=
  OrCombinator orEqualTo;
  orEqualTo.firstParser = &greaterOrEqual;
  orEqualTo.secondParser = &lessOrEqual;
  // > or < or >= or <=
  OrCombinator both;
  both.firstParser = &orEqualTo;
  both.secondParser = &greaterOrLess;
  // == or > or < or >= or <=
  OrCombinator all;
  all.firstParser = &equalTo;
  all.secondParser = &both;

  OrCombinator includingNot;
  includingNot.firstParser = &all;
  includingNot.secondParser = &notEqualTo;

  ParseStatus result = includingNot.parse(inputProgram, startCharacter);
  return result;
}

// LOP
ParseStatus LogicOperatorParser::do_parse(std::string inputProgram,
                                          int startCharacter) {
  AndOpParser andOp;
  OrOpParser orOp;

  // && or ||
  OrCombinator and_or;
  and_or.firstParser = &andOp;
  and_or.secondParser = &orOp;

  return and_or.parse(inputProgram, startCharacter);
}

ParseStatus OpenCurlyBrackets::do_parse(std::string inputProgram,
                                        int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter);
  }

  auto openBracketsParser = AtomParser('{');

  return openBracketsParser.do_parse(inputProgram, endCharacter);
}

ParseStatus CloseCurlyBrackets::do_parse(std::string inputProgram,
                                         int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter);
  }

  auto openBracketsParser = AtomParser('}');

  return openBracketsParser.do_parse(inputProgram, endCharacter);
}

ParseStatus IfKeyword::do_parse(std::string inputProgram, int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter);
  }

  auto iParser = AtomParser('i');
  auto fParser = AtomParser('f');

  AndCombinator andOne;
  andOne.firstParser = &iParser;
  andOne.secondParser = &fParser;

  ParseStatus result = andOne.do_parse(inputProgram, endCharacter);
  if (!result.status) {
    result.errorType = errorMessage;
  }
  return result;
}

ParseStatus ElseKeyword::do_parse(std::string inputProgram,
                                  int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter);
  }

  auto e1Parser = AtomParser('e');
  auto lParser = AtomParser('l');
  auto sParser = AtomParser('s');
  auto e2Parser = AtomParser('e');
  AndCombinator andOne;
  andOne.firstParser = &e1Parser;
  andOne.secondParser = &lParser;

  AndCombinator andTwo;
  andTwo.firstParser = &sParser;
  andTwo.secondParser = &e2Parser;

  AndCombinator andThree;
  andThree.firstParser = &andOne;
  andThree.secondParser = &andTwo;

  ParseStatus result = andThree.do_parse(inputProgram, endCharacter);
  if (!result.status) {
    result.errorType = errorMessage;
  }
  return result;
}

ParseStatus RepeatKeyword::do_parse(std::string inputProgram,
                                    int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter);
  }

  auto rParser = AtomParser('r');
  auto eParser = AtomParser('e');
  auto pParser = AtomParser('p');
  auto aParser = AtomParser('a');
  auto tParser = AtomParser('t');
  AndCombinator andOne;
  andOne.firstParser = &rParser;
  andOne.secondParser = &eParser;
  AndCombinator andTwo;
  andTwo.firstParser = &andOne;
  andTwo.secondParser = &pParser;
  AndCombinator andThree;
  andThree.firstParser = &andTwo;
  andThree.secondParser = &eParser;
  AndCombinator andFour;
  andFour.firstParser = &andThree;
  andFour.secondParser = &aParser;
  AndCombinator andFive;
  andFive.firstParser = &andFour;
  andFive.secondParser = &tParser;

  ParseStatus result = andFive.do_parse(inputProgram, endCharacter);
  if (!result.status) {
    result.errorType = errorMessage;
  }
  return result;
}

ParseStatus WhileKeyword::do_parse(std::string inputProgram,
                                   int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter);
  }

  auto wParser = AtomParser('w');
  auto hParser = AtomParser('h');
  auto iParser = AtomParser('i');
  auto lParser = AtomParser('l');
  auto eParser = AtomParser('e');
  AndCombinator andOne;
  andOne.firstParser = &wParser;
  andOne.secondParser = &hParser;
  AndCombinator andTwo;
  andTwo.firstParser = &andOne;
  andTwo.secondParser = &iParser;
  AndCombinator andThree;
  andThree.firstParser = &andTwo;
  andThree.secondParser = &lParser;
  AndCombinator andFour;
  andFour.firstParser = &andThree;
  andFour.secondParser = &eParser;

  ParseStatus result = andFour.do_parse(inputProgram, endCharacter);
  if (!result.status) {
    result.errorType = errorMessage;
  }
  return result;
}
