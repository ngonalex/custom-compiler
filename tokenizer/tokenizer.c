#include tokenizer.h
using namespace cs160

Tokenizer::Tokenizer(std::string program) : program(program) { };


std::vector<Token> Tokenizer::tokenize(){
  std::vector<Token> tokens;
  char curr;
  for(char& c : program) {

    if(curr)
  }
}

bool Tokenizer::helper_is_num(){

}
bool Tokenizer::helper_is_paren();
bool Tokenizer::helper_is_addsub_op();
bool Tokenizer::helper_is_muldiv_op();
