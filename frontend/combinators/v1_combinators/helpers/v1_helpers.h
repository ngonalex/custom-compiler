#ifndef V1_HELPERS_H_
#define V1_HELPERS_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

#include <iostream>
#include <map>

namespace cs160 {
namespace frontend {

class CloseParenParser : NullParser {
 public:
   virtual ParseStatus parse(std::string inputProgram, int startCharacter, std::string errorType = ""); 
 private:
  std::map<int, ParseStatus> cache;

};

class OpenParenParser : NullParser {
 public:
   virtual ParseStatus parse(std::string inputProgram, int startCharacter, std::string errorType = ""); 
};

class NegativeParser : NullParser {
 public:
	virtual ParseStatus parse(std::string inputProgram, int startCharacter, std::string errorType = "");
};

class AddSubOpParser : NullParser {
 public:
	virtual ParseStatus parse(std::string inputProgram, int startCharacter, std::string errorType = "");
};

class MulDivOpParser : NullParser {
 public:
	virtual ParseStatus parse(std::string inputProgram, int startCharacter, std::string errorType = "");
};

class SemiColonParser : NullParser {
 public:
	virtual ParseStatus parse(std::string inputProgram, int startCharacter, std::string errorType = "");
};


} // namespace frontend
} // namespace cs160

#endif // V1_HELPERS_H_
