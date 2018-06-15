#ifndef PARSESTATUS_H_
#define PARSESTATUS_H_

#include "abstract_syntax/abstract_syntax.h"
#include "utility/memory.h"

#include <string>
#include <vector>

using namespace cs160::abstract_syntax::frontend;

namespace cs160 {
namespace frontend {

class ParseStatus {  // Super class
 public:
  ParseStatus();
  ParseStatus(ParseStatus &&other);
  // ParseStatus operator=(ParseStatus& other);
  bool status;
  std::string remainingCharacters;
  std::string parsedCharacters;

  std::vector<std::string> parsedCharactersArray;

  // Only for the success case
  std::unique_ptr<const AstNode> ast;

  // Only for And Combinator
  std::unique_ptr<const AstNode> second_ast;

  // Only for And Combinator (used for ast construction when grabbing relation
  // operator)
  std::string firstParsedCharacters;
  std::string secondParsedCharacters;

  // Only for Or Combinator True = firstParser passed, False = secondParser
  // passed
  bool firstOrSecond;

  // Only for zero_or_more and one_or_more case when returning multiple ast
  // nodes
  std::vector<std::unique_ptr<const AstNode>>
      astNodes;  // Overlapping first ast and second ast

  // Only for the failed case
  std::string errorType;

  int startCharacter;
  int endCharacter;
  /*
          explicit ParseStatus(bool p_status, std::string p_remainingCharacters,
                  std::string p_parsedCharacters, std::unique_ptr<const AstNode>
     p_ast, std::vector<std::unique_ptr<const AstNode>> p_astNodes, std::string
     p_errorType) : status(p_status),
     remainingCharacters(p_remainingCharacters),
     parsedCharacters(p_parsedCharacters), ast(p_ast), p_astNodes(p_astNodes),
     errorType(p_errorType){}

          ParseStatus(ParseStatus&& g) : ast(std::move(g.ast)) {}

      ParseStatus& operator=(ParseStatus&& g) {
          this->ast = std::move(g.ast);
          return *this;
      }
  */
  // check if two ParseStatuses are equal
  bool operator==(const ParseStatus &b) const;

  bool operator!=(const ParseStatus &b) const;

  /*
    ParseStatus(ParseStatus&& g) : status(g.status),
    remainingCharacters(g.remainingCharacters),
    parsedCharacters(g.parsedCharacters), ast(std::move(g.ast)),
    second_ast(std::move(g.second_ast)), astNodes(std::move(g.astNodes)),
    errorType(g.errorType), startCharacter(g.startCharacter),
    endCharacter(g.endCharacter) {}
  */

  /*
    ParseStatus(&& g) {
          this->status = g.status;
          this->remainingCharacters = g.remainingCharacters;
          this->parsedCharacters = g.parsedCharacters;
          this->ast = std::move(g.ast);
          this->second_ast = std::move(g.second_ast);
          this->astNodes = std::move(g.status);
          this->errorType = g.errorType;
          this->startCharacter = g.startCharacter;
          this->endCharacter = g.endCharacter;
          return *this;
    }*/
  /*
    static ParseStatus failure(std::string errorType, int characterStart) {
      std::vector<std::unique_ptr<const AstNode>> astNodes;
      return ParseStatus( false, "", "", nullptr, nullptr, astNodes, errorType,
   characterStart);
    }

    static ParseStatus success(std::string remainingCharacters,
                               std::string parsedCharacters,
                               std::unique_ptr<const AstNode> result,
                               int characterStart) {
          std::vector<std::unique_ptr<const AstNode>> astNodes;
      return ParseStatus(true, remainingCharacters, parsedCharacters,
   std::move(result), nullptr, astNodes, "", characterStart);
    }

    static ParseStatus andSuccess(std::string remainingCharacters,
                                  std::string parsedCharacters,
                                  std::unique_ptr<const AstNode> result,
                                  std::unique_ptr<const AstNode> secondResult,
                                  int characterStart) {
          std::vector<std::unique_ptr<const AstNode>> astNodes;
      return ParseStatus(true, remainingCharacters, parsedCharacters,
   std::move(result), std::move(secondResult), std::move(astNodes), "",
                         characterStart);
    }

    static ParseStatus vectorSuccess(
        std::string remainingCharacters, std::string parsedCharacters,
        std::vector<std::unique_ptr<const AstNode>> astNodes,
        int characterStart) {
      return ParseStatus(true, remainingCharacters, parsedCharacters, nullptr,
                         nullptr, astNodes, "", characterStart);
    }

   private:
    ParseStatus(bool status, std::string remainingCharacters,
                std::string parsedCharacters, std::unique_ptr<const AstNode>
   ast, std::unique_ptr<const AstNode> second_ast,
                std::vector<std::unique_ptr<const AstNode>> astNodes,
                std::string errorType, int characterStart) {
      this->status = status;
      this->remainingCharacters = remainingCharacters;
      this->parsedCharacters = parsedCharacters;
      this->ast = std::move(ast);
      this->second_ast = std::move(second_ast);
      this->astNodes = std::move(astNodes);
      this->errorType = errorType;
      this->characterStart = characterStart;
    }*/
};
/*
class AndCombParseStatus : public ParseStatus {
 public:
                // Only for the success case
        std::unique_ptr<const AstNode> ast;

        // Only for And Combinator
        std::unique_ptr<const AstNode> second_ast;

        AndCombParseStatus(ParseStatus&& firstStatus, ParseStatus&&
secondStatus){ this->ast = firstStatus.ast; this->second_ast = secondStatus.ast;
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

}  // namespace frontend
}  // namespace cs160

#endif  // PARSESTATUS_H_
