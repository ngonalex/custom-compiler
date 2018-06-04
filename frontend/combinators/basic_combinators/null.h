#ifndef NULL_PARSER_H
#define NULL_PARSER_H

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/parsestatus.h"

namespace cs160 {
namespace frontend {

class NullParser {
 public:
  ParseStatus parse(std::string inputProgram,
  				int startCharacter,
			    std::string errorType = "");

	ResultStatus fail(msg, pos)
};

// class ForwardParser {
// 	public:
// 	ParseStatus do_parse()

// 	setParser(Parser* p)

// 	unique_ptr<Parser> p_;
// };

// auto mul = new ForwardParser()

// mul.setParser(mulConcrete)

}  // namespace frontend
}  // namespace cs160

#endif  // NULL_PARSER_H
