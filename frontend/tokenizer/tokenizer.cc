#include "./tokenizer.h"
#include <sstream>

using namespace cs160::frontend;

Type extractType (const char testChar) {
  if (isdigit(testChar))
    return NUM;
  switch (testChar) {
    case '(': { return OPEN_PAREN; }
    case ')': { return CLOSE_PAREN; }
    case '+': { return ADD_OP; }
    case '-': { return SUB_OP; }
    case '*': { return MUL_OP; }
    case '/': { return DIV_OP; }
    default: { return FAILED; }
  }
}

void Tokenizer::print() {
  switch(this->type()) {
    case NUM:
      printf("Type: NUM\n");
      printf("\tValue: %i\n", this->val());
      break;
    case FAILED:
      printf("Type: FAILED\n");
      break;
    default:
      printf("Type: %d\n", this->type());
      break;
  }
}

Tokenizer::Tokenizer(std::string program) : program_(program) {
    char prevChar;
    Type prevType = NONE;
    std::string currString;

    for (char &c : program_) {
    Type type = extractType(c);
    if (type != FAILED) {
        std::stringstream currStringStream;
        currStringStream << currString;
        currStringStream << c;

        currString = currStringStream.str();
        if (type != prevType) {
            if (type == NUM){
                Token newToken(type, atoi(currString.c_str()));
                tokens_.push_back(newToken);
            } else {
                Token newToken(type);
                tokens_.push_back(newToken);
            }
            currString = "";
        }
        prevChar = c;
        prevType = type;
    } else {
            Token newToken(FAILED);
            tokens_[0] = (newToken);
            break;
            // Return just one token that is null
    }
}
};
