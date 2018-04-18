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
  for(Token a : lexer.tokens()){
      std::cout << "Type: ";
      std::cout << a.type();
      if(a.type() == NUM){
          std::cout << "; Val: ";
          std::cout << a.val();
      }
      std::cout << std::endl;
      i++;
  }
  return 0;
}
