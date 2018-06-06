#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"
#include "frontend/combinators/basic_combinators/and_combinator.h"
#include "frontend/combinators/basic_combinators/atom_parser.h"
#include "frontend/combinators/v3_combinators/helpers/relational_helper.h"

#include <iostream>

#include <string>     // std::string, std::stoi

#define super NullParser

using namespace cs160::frontend;

std::string errorMessage = "Expected relational operator";

ParseStatus EqualToOpParser::parse(std::string inputProgram, int startCharacter) {
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

	return equals_equals.parse(inputProgram, endCharacter);
}

ParseStatus GreaterThanOpParser::parse(std::string inputProgram, int startCharacter) {
	int endCharacter = startCharacter;
	endCharacter += trim(inputProgram);

	if (inputProgram.size() <= 0) {
		return super::fail(inputProgram, endCharacter, errorMessage);
	}

	auto parser = AtomParser('>');
	return parser.parse(inputProgram, endCharacter);
}

ParseStatus LessThanOpParser::parse(std::string inputProgram, int startCharacter) {
	int endCharacter = startCharacter;
	endCharacter += trim(inputProgram);

	if (inputProgram.size() <= 0) {
		return super::fail(inputProgram, endCharacter, errorMessage);
	}

	auto parser = AtomParser('<');
	return parser.parse(inputProgram, endCharacter);
}

ParseStatus GreaterThanOrEqualToOpParser::parse(std::string inputProgram, int startCharacter) {
	int endCharacter = startCharacter;
	endCharacter += trim(inputProgram);
	
	if (inputProgram.size() <= 1) {
		return super::fail(inputProgram, endCharacter, errorMessage);
	}

	auto greaterThanParser = AtomParser('>');
	auto equalsParser = AtomParser('=');
	AndCombinator greater_or_equal;
	greater_or_equal.firstParser = reinterpret_cast<NullParser *>(&greaterThanParser);
	greater_or_equal.secondParser = reinterpret_cast<NullParser *>(&equalsParser);

	return greater_or_equal.parse(inputProgram, endCharacter);
}

ParseStatus LessThanOrEqualToOpParser::parse(std::string inputProgram, int startCharacter) {
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

	return less_or_equal.parse(inputProgram, endCharacter);
}

ParseStatus AndOpParser::parse(std::string inputProgram, int startCharacter) {
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

	return and_and.parse(inputProgram, endCharacter);
}

ParseStatus OrOpParser::parse(std::string inputProgram, int startCharacter) {
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

	return or_or.parse(inputProgram, endCharacter);
}

ParseStatus NotOpParser::parse(std::string inputProgram, int startCharacter) {
	int endCharacter = startCharacter;
	endCharacter += trim(inputProgram);

	if (inputProgram.size() <= 0) {
		return super::fail(inputProgram, endCharacter, errorMessage);
	}

	auto parser = AtomParser('!');
	return parser.parse(inputProgram, endCharacter);
}

ParseStatus RelationOperator::parse(std::string inputProgram, int startCharacter) {
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
	
	return all.parse(inputProgram, startCharacter);
}