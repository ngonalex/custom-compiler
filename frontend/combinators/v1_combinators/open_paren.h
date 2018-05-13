#ifndef OPEN_PAREN_H
#define OPEN_PAREN_H

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

class OpenParenParser : NullParser {
 public:
   virtual ParseStatus parse(std::string inputProgram); 
};

#endif // OPEN_PAREN_H
