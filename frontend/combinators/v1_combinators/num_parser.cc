#include "frontend/combinators/v1_combinators/num_parser.h"
#include "frontend/combinators/v1_combinators/single_digit.h"
#include "frontend/combinators/basic_combinators/one_or_more_combinator.h"

#define super NullParser

ParseStatus NumParser::parse(std::string inputProgram) {
	if (inputProgram.size() == 0) {
		return super::parse(inputProgram);
	}
  SingleDigitParser digitParser;
  OneOrMoreCombinator oneOrMore;

  return (oneOrMore.parser);
}
