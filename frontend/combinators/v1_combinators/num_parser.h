#ifndef NUM_PARSER_H_
#define NUM_PARSER_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class NumParser : NullParser {
 public:
  virtual ParseStatus parse(std::string inputProgram,
  				int startCharacter);
};

}  // namespace frontend
}  // namespace cs160

#endif  // NUM_PARSER_H_
