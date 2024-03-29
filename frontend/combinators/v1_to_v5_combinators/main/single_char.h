#ifndef SINGLE_CHAR_H_
#define SINGLE_CHAR_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class SingleCharParser : public NullParser {
 public:
  virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

class SingleVarCharParser : public NullParser {
 public:
  virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

}  // namespace frontend
}  // namespace cs160

#endif  // SINGLE_CHAR_H_
