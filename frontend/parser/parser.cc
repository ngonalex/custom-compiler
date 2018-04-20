#include "./parser.h"

using namespace cs160::frontend;

namespace cs160 {
namespace frontend {
  
  Type Parser::Next() {
    return program.back().type();
  }
  
  void Parser::Consume() {
    program.pop_back();
  }
  
  // Should be more elaborate...
  void Parser::Error() {
    printf("Error");
  }
  
  void Parser::Expect(Type type) {
    if (Next() == type) {
      Consume();
    } else {
      Error();
    }
  }
  
  void Parser::Erecognizer() {
    E();
    expect(Type::End);
  }
  
  void Parser::E() {
    P();
    while (Next() == Type::DIV_OP || Next() == Type::MUL_OP ||
           Next() == Type::ADD_OP || Next() == Type::SUB_OP)) {
             Consume()
             P()
           }
  }
  
  void Parser::P() {
    if (Next() == Type::NUM) {
      Consume()
    } else if (Next() == Type::OPEN_PAREN) {
      Consume();
      E();
      Expect(Type::CLOSE_PAREN);
    } else {
      Error();
    }
  }
  

  
  
} // namespace frontend
} // namespace cs160