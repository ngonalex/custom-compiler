#ifndef FUNCTION_HELPER_H_
#define FUNCTION_HELPER_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class CommaOp : public NullParser {
 public:
  virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};
    
class ReturnTypeOp : public NullParser {
public:
    virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};
    
class FuncKeyword : public NullParser {
public:
    virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

class FunctionVariableParsers : public NullParser {
 public:
  virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

class FunctionArgumentParsers : public NullParser {
 public:
  virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

}  // namespace frontend
}  // namespace cs160
#endif
