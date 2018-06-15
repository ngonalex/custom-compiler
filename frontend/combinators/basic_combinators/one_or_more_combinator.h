#ifndef ONE_OR_MORE_COMBINATOR_H_
#define ONE_OR_MORE_COMBINATOR_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class OneOrMoreCombinator : NullParser {
 public:
	NullParser *parser;
	virtual ParseStatus parse(std::string inputProgram, std::string errorType = "");
};

}  // namespace frontend
}  // namespace cs160

#endif  // ONE_OR_MORE_COMBINATOR_H_
