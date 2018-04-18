#include <iostream>
#include "tokenizer.h"

using namespace cs160::frontend;

int main() {
  Tokenizer lexer("3+3");
  std::vector<Token> tokens;
  tokens.push_back(Token(NUM, 3));
  tokens.push_back(Token(ADD_OP));
  tokens.push_back(Token(NUM, 3));
  int i = 0;
  for(Token a : lexer.tokens()){
    std::cout << a.type() << std::endl;
    if(a.type() == NUM)
      std::cout << a.val() << std::endl;
    i++;
  }
  return 0;
}
