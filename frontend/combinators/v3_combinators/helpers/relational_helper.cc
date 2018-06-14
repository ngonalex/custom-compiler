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
  equals_equals.firstParser = reinterpret_cast<NullParser *>(&equalParser_1);
  equals_equals.secondParser = reinterpret_cast<NullParser *>(&equalParser_2);

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
  greater_or_equal.firstParser =
      reinterpret_cast<NullParser *>(&greaterThanParser);
  greater_or_equal.secondParser = reinterpret_cast<NullParser *>(&equalsParser);

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
  less_or_equal.firstParser = reinterpret_cast<NullParser *>(&lessThanParser);
  less_or_equal.secondParser = reinterpret_cast<NullParser *>(&equalsParser);

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
  and_and.firstParser = reinterpret_cast<NullParser *>(&andParser_1);
  and_and.secondParser = reinterpret_cast<NullParser *>(&andParser_2);

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
  or_or.firstParser = reinterpret_cast<NullParser *>(&orParser_1);
  or_or.secondParser = reinterpret_cast<NullParser *>(&orParser_2);

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

  GreaterThanOrEqualToOpParser greaterOrEqual;
  LessThanOrEqualToOpParser lessOrEqual;

  GreaterThanOpParser greaterThan;
  LessThanOpParser lessThan;
  // > or <
  OrCombinator greaterOrLess;
  greaterOrLess.firstParser = reinterpret_cast<NullParser *>(&greaterThan);
  greaterOrLess.secondParser = reinterpret_cast<NullParser *>(&lessThan);
  // >= or <=
  OrCombinator orEqualTo;
  orEqualTo.firstParser = reinterpret_cast<NullParser *>(&greaterOrEqual);
  orEqualTo.secondParser = reinterpret_cast<NullParser *>(&lessOrEqual);
  // > or < or >= or <=
  OrCombinator both;
  both.firstParser = reinterpret_cast<NullParser *>(&orEqualTo);
  both.secondParser = reinterpret_cast<NullParser *>(&greaterOrLess);
  // == or > or < or >= or <=
  OrCombinator all;
  all.firstParser = reinterpret_cast<NullParser *>(&equalTo);
  all.secondParser = reinterpret_cast<NullParser *>(&both);
    ParseStatus result = all.parse(inputProgram, startCharacter);
    return result;
}

// LOP
ParseStatus LogicOperatorParser::do_parse(std::string inputProgram,
                                          int startCharacter) {
  AndOpParser andOp;
  OrOpParser orOp;

  // && or ||
  OrCombinator and_or;
  and_or.firstParser = reinterpret_cast<NullParser *>(&andOp);
  and_or.secondParser = reinterpret_cast<NullParser *>(&orOp);

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
  andOne.firstParser = reinterpret_cast<NullParser *>(&iParser);
  andOne.secondParser = reinterpret_cast<NullParser *>(&fParser);

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
  andOne.firstParser = reinterpret_cast<NullParser *>(&e1Parser);
  andOne.secondParser = reinterpret_cast<NullParser *>(&lParser);

  AndCombinator andTwo;
  andTwo.firstParser = reinterpret_cast<NullParser *>(&sParser);
  andTwo.secondParser = reinterpret_cast<NullParser *>(&e2Parser);

  AndCombinator andThree;
  andThree.firstParser = reinterpret_cast<NullParser *>(&andOne);
  andThree.secondParser = reinterpret_cast<NullParser *>(&andTwo);

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
  andOne.firstParser = reinterpret_cast<NullParser *>(&rParser);
  andOne.secondParser = reinterpret_cast<NullParser *>(&eParser);
  AndCombinator andTwo;
  andTwo.firstParser = reinterpret_cast<NullParser *>(&andOne);
  andTwo.secondParser = reinterpret_cast<NullParser *>(&pParser);
  AndCombinator andThree;
  andThree.firstParser = reinterpret_cast<NullParser *>(&andTwo);
  andThree.secondParser = reinterpret_cast<NullParser *>(&eParser);
  AndCombinator andFour;
  andFour.firstParser = reinterpret_cast<NullParser *>(&andThree);
  andFour.secondParser = reinterpret_cast<NullParser *>(&aParser);
  AndCombinator andFive;
  andFive.firstParser = reinterpret_cast<NullParser *>(&andFour);
  andFive.secondParser = reinterpret_cast<NullParser *>(&tParser);

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
  andOne.firstParser = reinterpret_cast<NullParser *>(&wParser);
  andOne.secondParser = reinterpret_cast<NullParser *>(&hParser);
  AndCombinator andTwo;
  andTwo.firstParser = reinterpret_cast<NullParser *>(&andOne);
  andTwo.secondParser = reinterpret_cast<NullParser *>(&iParser);
  AndCombinator andThree;
  andThree.firstParser = reinterpret_cast<NullParser *>(&andTwo);
  andThree.secondParser = reinterpret_cast<NullParser *>(&lParser);
  AndCombinator andFour;
  andFour.firstParser = reinterpret_cast<NullParser *>(&andThree);
  andFour.secondParser = reinterpret_cast<NullParser *>(&eParser);

  ParseStatus result = andFour.do_parse(inputProgram, endCharacter);
  if (!result.status) {
    result.errorType = errorMessage;
  }
  return result;
}
