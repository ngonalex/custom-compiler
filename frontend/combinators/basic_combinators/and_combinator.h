#ifndef AND_COMBINATOR_H_
#define AND_COMBINATOR_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {
	
class AndCombinator : NullParser {
 public:
	NullParser *firstParser;
	NullParser *secondParser;

	virtual ParseStatus parse(std::string inputProgram);
};

}	// namespace frontend
}	// namespace cs160

#endif // AND_COMBINATOR_H_
