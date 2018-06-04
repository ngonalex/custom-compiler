#ifndef WORD_PARSER_H_
#define WORD_PARSER_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class WordParser : NullParser {
 public:
  virtual ParseStatus parse(std::string inputProgram, int startCharacter,
                            std::string errorType = "");
};

}  // namespace frontend
}  // namespace cs160

#endif  // WORD_PARSER_H_
