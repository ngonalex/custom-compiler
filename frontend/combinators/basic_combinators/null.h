#ifndef NULL_PARSER_H
#define NULL_PARSER_H

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/parsestatus.h"

namespace cs160 {
namespace frontend {

class NullParser {
 public:
  virtual ParseStatus parse(std::string inputProgram,
			    std::string errorType = "");
};

}  // namespace frontend
}  // namespace cs160

#endif  // NULL_PARSER_H
