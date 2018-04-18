#include <iostream>
#include "tokenizer.h"

using namespace cs160::frontend;

int main() {
  Tokenizer lexer("4+2+0");
  /*std::vector<Token> tokens;
  tokens.push_back(Token(NUM, 3));
  tokens.push_back(Token(ADD_OP));
  tokens.push_back(Token(NUM, 3));*/
  int i = 0;
  lexer.print();
  /*
  for(Token a : lexer.tokens()){
<<<<<<< HEAD
    std::cout << a.type() << std::endl;
    if(a.type() == NUM)
      std::cout << a.val() << std::endl;
    i++;
  }*/
=======
      std::cout << "Type: ";
      std::cout << a.type();
      if(a.type() == NUM){
          std::cout << "; Val: ";
          std::cout << a.val();
      }
      std::cout << std::endl;
      i++;
  }
>>>>>>> 8c6eb21d0567b30190055b7767ab1ded8fe40333
  return 0;
}
