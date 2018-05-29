#ifndef RELATIONAL_HELPER_H_
#define RELATIONAL_HELPER_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class EqualToOpParser : NullParser {
 public:
   virtual ParseStatus parse(std::string inputProgram, std::string errorType = "");
};

class GreaterThanOpParser : NullParser {
 public:
   virtual ParseStatus parse(std::string inputProgram, std::string errorType = "");
};

class LessThanOpParser : NullParser {
 public:
   virtual ParseStatus parse(std::string inputProgram, std::string errorType = "");
};

class GreaterThanOrEqualToOpParser : NullParser {
 public:
   virtual ParseStatus parse(std::string inputProgram, std::string errorType = "");
};

class LessThanOrEqualToOpParser : NullParser {
 public:
   virtual ParseStatus parse(std::string inputProgram, std::string errorType = "");
};

class AndOpParser : NullParser {
 public:
   virtual ParseStatus parse(std::string inputProgram, std::string errorType = "");
};

class OrOpParser : NullParser {
 public:
   virtual ParseStatus parse(std::string inputProgram, std::string errorType = "");
};

class NotOpParser : NullParser {
 public:
   virtual ParseStatus parse(std::string inputProgram, std::string errorType = "");
};

} // namespace frontend
} // namespace cs160

#endif // RELATIONAL_HELPER_H_