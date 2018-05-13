#ifndef NUM_PARSER_H_
#define NUM_PARSER_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

class NumParser : NullParser {
 public:
   virtual ParseStatus parse(std::string inputProgram); 
};

#endif // NUM_PARSER_H_
