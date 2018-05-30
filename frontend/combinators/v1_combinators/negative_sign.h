#ifndef NEGATIVE_SIGN_H_
#define NEGATIVE_SIGN_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/combinators/basic_combinators/null.h"

namespace cs160 {
namespace frontend {

class NegativeParser : NullParser {
public:
	virtual ParseStatus parse(std::string inputProgram, std::string errorType = "");
};

}  // namespace frontend
}  // namespace cs160

#endif  // NEGATIVE_SIGN_H_
