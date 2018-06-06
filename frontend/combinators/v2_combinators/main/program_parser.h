#ifndef PROGRAM_PARSER_H_
#define PROGRAM_PARSER_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class ProgramParser : NullParser {
 public:
   virtual ParseStatus parse(std::string inputProgram, int startCharacter);
};

}  // namespace frontend
}  // namespace cs160

#endif  // PROGRAM_PARSER_H_
