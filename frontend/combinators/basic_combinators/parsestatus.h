#ifndef PARSESTATUS_H_
#define PARSESTATUS_H_

#include "abstract_syntax/abstract_syntax.h"
#include "utility/memory.h"

#include <vector>
#include <string>

using namespace cs160::abstract_syntax::frontend;

namespace cs160 {
namespace frontend {

class ParseStatus { // Super class
 public:
	bool status;
	std::string remainingCharacters;
	std::string parsedCharacters;

	// Only for the success case
	std::unique_ptr<const AstNode> ast;

	// Only for And Combinator
	std::unique_ptr<const AstNode> second_ast;

	// Only for zero_or_more and one_or_more case when returning multiple ast nodes
	std::vector<std::unique_ptr<const AstNode>> astNodes;

	// Only for the failed case
	std::string errorType;

	// check if two ParseStatuses are equal
	bool operator==(const ParseStatus &b) const;

	bool operator!=(const ParseStatus &b) const;
};

} // namespace frontend
} // namespace cs160

#endif // PARSESTATUS_H_
