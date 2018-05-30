#ifndef MUL_DIV_OP_H_
#define MUL_DIV_OP_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"
#include "frontend/combinators/basic_combinators/parsestatus.h"

namespace cs160 {
namespace frontend {

class MulDivOpParser : NullParser {
 public:
  virtual ParseStatus parse(std::string inputProgram,
  				int startCharacter,
			    std::string errorType = "");
};

}  // namespace frontend
}  // namespace cs160

#endif  // MUL_DIV_OP_H_
