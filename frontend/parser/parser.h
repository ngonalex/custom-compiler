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
  }
  Parser(){ }

  /** Subroutines **/
  // Returns the type of the token in the 'front' of the program_
  Token::Type Next() { return program_.back().type(); }
  // Removes a token from the program_ vector
  void Consume() { program_.pop_back(); };
  // Prints an error message with information of current token
  void Error() {
    std::cout << "Error" << std::endl;
  }
  // Consume Token if proper type, otherwise error
  void Expect(Token::Type type) { Next() == type ? Consume() : Error(); }

  // AST Expressions
  std::unique_ptr<const AstNode> mkNode(Token::Type op, std::unique_ptr<const AstNode> first_leaf, std::unique_ptr<const AstNode> second_leaf) {
    switch(op) {
      case Token::Type::ADD_OP: {
       return make_unique<AddExpr>(std::move(first_leaf), std::move(second_leaf));
      }
      case Token::Type::SUB_OP: {
       return make_unique<SubtractExpr>(std::move(first_leaf), std::move(second_leaf));
      }
      case Token::Type::MUL_OP: {
       return make_unique<MultiplyExpr>(std::move(first_leaf), std::move(second_leaf));
      }
      case Token::Type::DIV_OP: {
       return make_unique<DivideExpr>(std::move(first_leaf), std::move(second_leaf));
      }
      default: {
        Error();
        return nullptr;
      }
    }
  }

  std::unique_ptr<const AstNode> mkLeaf(Token num) {
    ASSERT(num.isNumber(), "Error creating IntegerExpr");
    return make_unique<IntegerExpr>(num.val());
  }

  // every 't' is supposed to be a Tree
  std::unique_ptr<const AstNode> Eparser() {
    std::unique_ptr<const AstNode> t = E();
    Expect(Token::Type::END);
    return t;
  }

  std::unique_ptr<const AstNode> E() {
    std::unique_ptr<const AstNode> t = T();
    Token::Type op = Next();
    while (op == Token::Type::ADD_OP || op == Token::Type::SUB_OP) {
      Consume();
      std::unique_ptr<const AstNode> t1 = T();
      t = mkNode(op, std::move(t), std::move(t1));
    }
    return t;
  }

  std::unique_ptr<const AstNode> T() {
    std::unique_ptr<const AstNode> t = P();
    Token::Type op = Next();
    while (op == Token::Type::MUL_OP || op == Token::Type::DIV_OP) {
      Consume();
      std::unique_ptr<const AstNode> t1 = P();
      t = mkNode(op, std::move(t), std::move(t1));
    }
    return t;
  }

  std::unique_ptr<const AstNode> P() {
    // Either returns an Int
      Token::Type type = Next();
    if (type == Token::Type::NUM) {
      std::unique_ptr<const AstNode> t = mkLeaf(program_.back());
      Consume();
      return t;
    }
    // An Expression
    else if (type == Token::Type::OPEN_PAREN) {
      Consume();
      std::unique_ptr<const AstNode> t = E();
      Expect(Token::Type::CLOSE_PAREN);
      return t;
    }
    // Or an error
    else {
      Error();
      return NULL;
    }
  }

// Tokens will now be in reverse order!
 private:
  // Output from the lexer
  std::vector<Token> program_;

};


} // namespace frontend
} // namespace cs160

#endif // PARSER_PARSER_H_
