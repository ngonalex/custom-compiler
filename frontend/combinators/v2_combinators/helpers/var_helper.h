#ifndef VAR_HELPER_H_
#define VAR_HELPER_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class VarKeywordParser : public NullParser {
 public:
   virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

class ColonParser : public NullParser {
 public:
   virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

class TypeParser : public NullParser {
 public:
   virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

class EqualSignParser : public NullParser {
 public:
   virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

class HelperVariableParser : public NullParser {
 public:
   virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

class SemiColonParser : public NullParser {
 public:
   virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

}  // namespace frontend
}  // namespace cs160

#endif  // VAR_HELPER_H_
