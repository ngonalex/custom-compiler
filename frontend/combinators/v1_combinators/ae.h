#ifndef AE_H_
#define AE_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

class AEParser : NullParser {
 public:
   virtual ParseStatus parse(std::string inputProgram);
};

#endif // AE_H_
