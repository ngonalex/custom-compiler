#ifndef PARSER_PARSER_H_
#define PARSER_PARSER_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/tokenizer/token.h"

#include <sstream>
#include <iostream>
#include <stack>

#include "utility/assert.h"

using namespace cs160::frontend;
using cs160::abstract_syntax::frontend::AstNode;
using cs160::abstract_syntax::frontend::IntegerExpr;
using cs160::abstract_syntax::frontend::AddExpr;
using cs160::abstract_syntax::frontend::SubtractExpr;
using cs160::abstract_syntax::frontend::MultiplyExpr;
using cs160::abstract_syntax::frontend::DivideExpr;
using cs160::abstract_syntax::frontend::BinaryOperatorExpr;
using cs160::make_unique;

namespace cs160 {
namespace frontend {

class Parser {
 public:
  // Must pass tokens program into Parser, even it it is just Type::END
  explicit Parser(std::vector<Token> program) : program_(program) {
    ASSERT(program.size() != 0, "Program cannot be empty tokens");
  }

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
      }  
    }
  }

  std::unique_ptr<const AstNode> mkLeaf(Token num) {
    ASSERT(num.isNumber(), "Error creating IntegerExpr");
    auto result = make_unique<IntegerExpr>(num.val());
    return result;
  }

  // every 't' is supposed to be a Tree
  std::unique_ptr<const AstNode> Eparser() {
    auto t = E();
    Expect(Token::Type::END);
    return t;
  }

  std::unique_ptr<const AstNode> E() {
    auto t = T();
    while (Next() == Token::Type::ADD_OP || Next() == Token::Type::SUB_OP) {
      const auto op = Next();
      Consume();
      const auto t1 = T();
      t = mkNode(op, std::move(t), t1);
    }
    return t;
  }

  std::unique_ptr<const AstNode> T() {
    auto t = P();
    while (Next() == Token::Type::MUL_OP || Next() == Token::Type::DIV_OP) {
      const auto op = Next();
      Consume();
      const auto t1 = P();
      t = mkNode(op, std::move(t), t1);
    }
    return t;
  }

  std::unique_ptr<const AstNode> P() {
    // Either returns an Int
    if (Next() == Token::Type::NUM) {
      auto t = mkLeaf(program_.back());
      Consume();
      return t;
    }
    // An Expression 
    else if (Next() == Token::Type::OPEN_PAREN) {
      Consume();
      auto t = E();
      Expect(Token::Type::CLOSE_PAREN);
      return t;
    } 
    // Or an error
    else {
      Error();
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
