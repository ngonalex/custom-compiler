#include "frontend/combinators/peg_library/atom_parser.h"
#include "gtest/gtest.h"

using namespace cs160::frontend;

// Fail Case WordParser
TEST(AtomParser, atomParserSuccess) {
  AtomParser atomParser('h');
}