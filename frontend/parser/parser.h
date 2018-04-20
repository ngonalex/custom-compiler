#ifndef PARSER_PARSER_H_
#define PARSER_PARSER_H_

#include "frontend/tokenizer/tokenizer.h"

#include <sstream>
#include <iostream>

using namespace cs160::frontend;

namespace cs160 {
namespace frontend {
  
class Parser {
 public:
  Parser(std::vector<Token> program) {
    program_ = program;
  }
  
  /** Subroutines **/
  // Returns the type of the token in the 'front' of the program_ 
  Type Next() { return program_.back().type(); }
  // Removes a token from the program_ vector
  void Consume() { program_.pop_back() }; 
  // Prints an error message with information of current token
  void Error() {
    std::cout << "Error" << std::endl;
  }
  // Consume Token if proper type, otherwise error
  void Expect(Type type) { Next() == type ? Consume() : Error(); }
  
  /** Grammar stepthrough **/
  void Recognizer() {
    E();
    Expect(Type::END);
  }
  
  void E() {
    P();
    while (Next() == Type::DIV_OP || Next() == Type::MUL_OP || 
    Next() == Type::ADD_OP || Next() == Type::SUB_OP)) {
      Consume();
      P();
    }
  }
  
  void P() {
    if (Next() == Type::NUM) {
      Consume();
    } else if (Next() == Type::OPEN_PAREN) {
      Consume();
      E();
      Expect(Type::CLOSE_PAREN);
    } else {
      Error();
    }
  }
  
// Tokens will now be in reverse order!
 private:
  std::vector<Token> program_;
};
  
  
} // namespace frontend
} // namespace cs160

#endif // PARSER_PARSER_H_