#ifndef SINGLE_CHAR_H_
#define SINGLE_CHAR_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class SingleCharParser : NullParser {
 public:
  virtual ParseStatus parse(std::string inputProgram,
  				int startCharacter,
			    std::string errorType = "");
};

class SingleVarCharParser : NullParser {
 public:
  virtual ParseStatus parse(std::string inputProgram,
			    std::string errorType = "");
};

}  // namespace frontend
}  // namespace cs160

#endif  // SINGLE_CHAR_H_
