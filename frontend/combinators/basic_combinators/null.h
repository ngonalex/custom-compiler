#ifndef NULL_PARSER_H
#define NULL_PARSER_H

#include <map> // std::map
#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/parsestatus.h"

namespace cs160 {
namespace frontend {

class NullParser {
 public:
  ParseStatus fail(std::string inputProgram, int startCharacter,
                   std::string errorMessage = "");

  virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
  ParseStatus parse(std::string inputProgram, int startCharacter,
                    std::string errorMessage = "");

 private:
  std::map<int, ParseStatus> cache;
};

}  // namespace frontend
}  // namespace cs160

#endif  // NULL_PARSER_H
