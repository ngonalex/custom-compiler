#ifndef NULL_PARSER_H
#define NULL_PARSER_H

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/parse_status.h"

namespace cs160 {
namespace frontend {

class NullParser {
 public:
  virtual ParseStatus<const AstNode> parse(std::string inputProgram,
  				int startCharacter,
			    std::string errorType = "");
};

}  // namespace frontend
}  // namespace cs160

#endif  // NULL_PARSER_H
