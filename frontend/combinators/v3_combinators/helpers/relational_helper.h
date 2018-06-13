#ifndef RELATIONAL_HELPER_H_
#define RELATIONAL_HELPER_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class EqualToOpParser : NullParser {
 public:
   virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

class GreaterThanOpParser : NullParser {
 public:
   virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

class LessThanOpParser : NullParser {
 public:
   virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

class GreaterThanOrEqualToOpParser : NullParser {
 public:
   virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

class LessThanOrEqualToOpParser : NullParser {
 public:
   virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

class AndOpParser : NullParser {
 public:
   virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

class OrOpParser : NullParser {
 public:
   virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

class NotOpParser : NullParser {
 public:
   virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

class RelationOperatorParser : NullParser {
 public:
   virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

class LogicOperatorParser : NullParser {
 public:
   virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

class OpenCurlyBrackets : NullParser {
 public:
   virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

class CloseCurlyBrackets : NullParser {
 public:
   virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

class RepeatKeyword : NullParser {
 public:
   virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};

class WhileKeyword : NullParser {
 public:
   virtual ParseStatus do_parse(std::string inputProgram, int startCharacter);
};


} // namespace frontend
} // namespace cs160

#endif // RELATIONAL_HELPER_H_