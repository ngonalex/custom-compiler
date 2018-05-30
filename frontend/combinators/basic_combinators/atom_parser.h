#ifndef ATOM_PARSER_H_
#define ATOM_PARSER_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class AtomParser : NullParser {
 public:
  virtual ParseStatus parse(std::string inputProgram,
  				int startCharacter,
			    std::string errorType = "");

  explicit AtomParser(char char_to_parse) { char_to_parse_ = char_to_parse; }

 private:
  char char_to_parse_:
};

}  // namespace frontend
}  // namespace cs160

#endif  // ATOM_PARSER_H_
