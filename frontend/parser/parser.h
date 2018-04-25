#ifndef PARSER_PARSER_H_
#define PARSER_PARSER_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/tokenizer/token.h"

#include <sstream>
#include <iostream>
#include <stack>

#include "utility/assert.h"

using namespace cs160::abstract_syntax::frontend;
using cs160::frontend::Token;
using namespace std;

namespace cs160 {
namespace frontend {

class Parser {
 public:
  // Must pass tokens program into Parser, even it it is just Type::END
  explicit Parser(std::vector<Token> program) : program_(program) {
    ASSERT(program.size() != 0, "Program cannot be empty tokens");
    std::reverse(this->program_.begin(), this->program_.end());
  }

  Parser(){ }

  /** Subroutines **/
  // Returns the type of the token in the 'front' of the program_
  Token::Type Next() { return program_.back().type(); }

  // Removes a token from the program_ vector
  void Consume() { program_.pop_back(); };

  // Prints an error message with information of current token
  void Error() {
    std::cerr << "Error!" << std::endl;
    exit(-1);
  }

  // Consume Token if proper type, otherwise error
  void Expect(Token::Type type) { Next() == type ? Consume() : Error(); }

  // AST Expressions
  std::unique_ptr<const AstNode> mkNode(Token::Type op, 
    std::unique_ptr<const AstNode> first_leaf,
    std::unique_ptr<const AstNode> second_leaf);
    
  std::unique_ptr<const AstNode> mkLeaf(Token num);

  std::unique_ptr<const AstNode> Eparser();

  std::unique_ptr<const AstNode> ParseAddSub();

  std::unique_ptr<const AstNode> ParseMulDiv();

  std::unique_ptr<const AstNode> ParseExpression();

// Tokens will now be in reverse order!
 private:
  // Output from the lexer
  std::vector<Token> program_;

};


} // namespace frontend
} // namespace cs160

#endif // PARSER_PARSER_H_
