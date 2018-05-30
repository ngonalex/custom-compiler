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
/*
	explicit ParseStatus(bool p_status, std::string p_remainingCharacters,
		std::string p_parsedCharacters, std::unique_ptr<const AstNode> p_ast, 
		std::vector<std::unique_ptr<const AstNode>> p_astNodes, std::string p_errorType) 
		: status(p_status), remainingCharacters(p_remainingCharacters), parsedCharacters(p_parsedCharacters),
		ast(p_ast), p_astNodes(p_astNodes), errorType(p_errorType){}

	ParseStatus(ParseStatus&& g) : ast(std::move(g.ast)) {}

    ParseStatus& operator=(ParseStatus&& g) {
        this->ast = std::move(g.ast);
        return *this;
    }
*/
	// check if two ParseStatuses are equal
	bool operator==(const ParseStatus &b) const;

	bool operator!=(const ParseStatus &b) const;
};
/*
class AndCombParseStatus : public ParseStatus { 
 public:
 		// Only for the success case
	std::unique_ptr<const AstNode> ast;

	// Only for And Combinator
	std::unique_ptr<const AstNode> second_ast;

	AndCombParseStatus(ParseStatus&& firstStatus, ParseStatus&& secondStatus){
		this->ast = firstStatus.ast;
		this->second_ast = secondStatus.ast;
	} 

    AndCombParseStatus& operator=(AndCombParseStatus&& g) {
        this->ast = std::move(g.ast);
        this->second_ast = std::move(g.second_ast);
        return *this;
    }


	// check if two ParseStatuses are equal
	bool operator==(const ParseStatus &b) const;

	bool operator!=(const ParseStatus &b) const;
};*/

} // namespace frontend
} // namespace cs160

#endif // PARSESTATUS_H_
