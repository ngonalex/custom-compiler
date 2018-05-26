#ifndef SINGLE_DIGIT_H_
#define SINGLE_DIGIT_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {
	
class SingleDigitParser : NullParser {
public:
	virtual ParseStatus parse(std::string inputProgram, std::string errorType = "");
};

} // namespace frontend
} // namespace cs160

#endif // SINGLE_DIGIT_H_
