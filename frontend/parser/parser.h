#ifndef PARSER_PARSER_H_
#define PARSER_PARSER_H_

#include "abstract_syntax/abstract_syntax.h"
#include "frontend/tokenizer/token.h"

#include <sstream>
#include <iostream>
#include <stack>

#include "utility/assert.h"

using namespace cs160::frontend;
using cs160::abstract_syntax::frontend::AstVisitor;
using cs160::abstract_syntax::frontend::IntegerExpr;
using cs160::abstract_syntax::frontend::AddExpr;
using cs160::abstract_syntax::frontend::SubtractExpr;
using cs160::abstract_syntax::frontend::MultiplyExpr;
using cs160::abstract_syntax::frontend::DivideExpr;
using cs160::abstract_syntax::frontend::BinaryOperatorExpr;
//using cs160::abstract_syntax::frontend::InterpreterVisitor;
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


  /** Grammar stepthrough **/
  void Recognizer() {
    E();
    Expect(Token::Type::END);
  }

  auto mkNode(Token op, auto first_leaf, auto second_leaf) {
    ASSERT(op.isOperator(), Error());
    switch(op.type()) {
      case Token::Type::ADD_OP: {
        return make_unique<AddExpr>(first_leaf, second_leaf);
      }
      case Token::Type::SUB_OP: {
        return make_unique<SubtractExpr>(first_leaf, second_leaf);
      }
      case Token::Type::MUL_OP: {
        return make_unique<MultiplyExpr>(first_leaf, second_leaf);
      }
      case Token::Type::DIV_OP: {
        return make_unique<DivideExpr>(first_leaf, second_leaf);
      }
    }
  }

  auto mkLeaf(Token num) {
    ASSERT(num.isNumber(), Error());
    auto result = make_unique<IntegerExpr>(num.val());
    return result;
  }

  // every 't' is supposed to be a Tree
  auto Eparser() {
    auto t = E();
    Expect(Token::Type::END);
    return t;
  }

  auto E() {
    auto t = T();
    while (Next() == Token::Type::ADD_OP || Next() == Token::Type::SUB_OP) {
      const auto op = Next();
      Consume();
      const auto t1 = T();
      t = mkNode(op, t, t1);
    }
    return t;
  }

  auto T() {
    auto t = P();
    while (Next() == Token::Type::MUL_OP || Next() == Token::Type::DIV_OP) {
      const auto op = Next();
      Consume();
      const auto t1 = P();
      t = mkNode(op, t, t1);
    }
    return t;
  }

  auto P() {
    if (Next() == Token::Type::NUM) {
      auto t = mkLeaf(program_.back().val());
      Consume();
      return t;
    } else if (Next() == Token::Type::OPEN_PAREN) {
      Consume();
      auto t = E();
      Expect(Token::Type::CLOSE_PAREN);
      return t;
    } else {
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
