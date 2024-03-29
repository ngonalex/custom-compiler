#include "frontend/combinators/v1_to_v5_combinators/helpers/v1_helpers.h"
#include "frontend/combinators/basic_combinators/atom_parser.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"

#include <string>  // std::string, std::stoi

#define super NullParser

namespace cs160 {
namespace frontend {
// )
ParseStatus CloseParenParser::do_parse(std::string inputProgram,
                                       int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);
  // Check in the cache if the character cuont parsestatus already exists,
  // return that parsestatus if it does
  std::string errorMessage = "Expecting close parenthesis";

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter, errorMessage);
  }

  auto atomParser = AtomParser(')');
  auto result = atomParser.do_parse(inputProgram, endCharacter);

  // append to cache startCharacter and ParseStatus
  return result;
}

// (
ParseStatus OpenParenParser::do_parse(std::string inputProgram,
                                      int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);

  std::string errorMessage = "Expecting open parenthesis";

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter, errorMessage);
  }

  auto atomParser = AtomParser('(');
  auto result = atomParser.do_parse(inputProgram, endCharacter);
  return result;
}

// -
ParseStatus NegativeParser::do_parse(std::string inputProgram,
                                     int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);
  std::string errorMessage = "Expecting -";

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter, errorMessage);
  }

  auto atomParser = AtomParser('-');
  auto result = atomParser.do_parse(inputProgram, endCharacter);
  return result;
}

// + -
ParseStatus AddSubOpParser::do_parse(std::string inputProgram,
                                     int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);
  std::string errorMessage = "Expecting + or -";

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter, errorMessage);
  }

  auto plusParser = AtomParser('+');
  auto minusParser = AtomParser('-');
  OrCombinator plusOrMinus;
  plusOrMinus.firstParser = reinterpret_cast<NullParser *>(&plusParser);
  plusOrMinus.secondParser = reinterpret_cast<NullParser *>(&minusParser);

  auto result = plusOrMinus.do_parse(inputProgram, endCharacter);
  return result;
}

// * /
ParseStatus MulDivOpParser::do_parse(std::string inputProgram,
                                     int startCharacter) {
  int endCharacter = startCharacter;
  endCharacter += trim(inputProgram);
  std::string errorMessage = "Expecting * or /";

  if (inputProgram.size() == 0) {
    return super::fail(inputProgram, endCharacter, errorMessage);
  }

  auto mulParser = AtomParser('*');
  auto divParser = AtomParser('/');

  OrCombinator mulOrDiv;
  mulOrDiv.firstParser = reinterpret_cast<NullParser *>(&mulParser);
  mulOrDiv.secondParser = reinterpret_cast<NullParser *>(&divParser);

  auto result = mulOrDiv.do_parse(inputProgram, endCharacter);
  return result;
}

}  // namespace frontend
}  // namespace cs160
