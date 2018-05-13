#ifndef CLOSE_PAREN_H
#define CLOSE_PAREN_H

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

class CloseParenParser : NullParser {
 public:
   virtual ParseStatus parse(std::string inputProgram); 
};

#endif // CLOSE_PAREN_H
