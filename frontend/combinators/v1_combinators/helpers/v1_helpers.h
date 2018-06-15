#ifndef FRONTEND_COMBINATORS_V1_COMBINATORS_HELPERS_V1_HELPERS_H_
#define FRONTEND_COMBINATORS_V1_COMBINATORS_HELPERS_V1_HELPERS_H_

#include <iostream>
#include <map>
#include <string>

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class CloseParenParser : public NullParser {
 public:
  virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

class OpenParenParser : public NullParser {
 public:
  virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

class NegativeParser : public NullParser {
 public:
  virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

class AddSubOpParser : public NullParser {
 public:
  virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

class MulDivOpParser : public NullParser {
 public:
  virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

class SemiColonParser : public NullParser {
 public:
  virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

}  // namespace frontend
}  // namespace cs160

#endif  // FRONTEND_COMBINATORS_V1_COMBINATORS_HELPERS_V1_HELPERS_H_
