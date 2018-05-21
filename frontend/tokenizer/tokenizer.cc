#include "frontend/tokenizer/tokenizer.h"
#include <regex>
#include <sstream>

using namespace cs160::frontend;

Tokenizer::Tokenizer(std::string program) : input_program_(program) {
  char prevChar;
  Token::Type prevType = Token::NONE;
  std::string currString;
    
    //Looping through all characters of program and tokenize
    for (char &c : input_program_) {
        Token::Type separatorType = ExtractSeparator(c);
        if (separatorType != Token::INCOMPLETE) {
            if (currString != "") {
            Token::Type type = ExtractType(currString);
            if (type == Token::NUM) {
                Token newToken(Token::NUM, atoi(currString.c_str()));
                tokens_.push_back(newToken);
            } else if (type == Token::IDENTIFIER) {
                Token newToken(Token::IDENTIFIER, currString);
                tokens_.push_back(newToken);
            } else {
                Token newToken(type);
                tokens_.push_back(newToken);
            }
            printf("Current String: %s\n", currString.c_str());
            }
            if (separatorType != Token::WHITESPACE) {
                Token newToken(separatorType);
                tokens_.push_back(newToken);
            }
            currString = "";
        } else {
            std::stringstream currStringStream;
            currStringStream << currString;
            currStringStream << c;

            currString = currStringStream.str();
        }
    }
    
    if (currString == "") {
        Token endToken(Token::ENDOFFILE);
        tokens_.push_back(endToken);
    }
}

Token::Type Tokenizer::ExtractType(std::string expression) {
    if (expression == "var") {
        return Token::VAR_NAME;
    } else if (expression == "func") {
        
    }
    
    bool is_integer = std::regex_match(expression, std::regex("^(0|[1-9][0-9]*)$"));
    if (is_integer) {
        return Token::NUM;
    } else if (!isdigit(expression[0])) {
        return Token::IDENTIFIER;
    } else {
        return Token::FAILED;
    }
}

Token::Type Tokenizer::ExtractSeparator(const char testChar) {
    switch (testChar) {
        case '(': {
            return Token::OPEN_PAREN;
        }
        case ')': {
            return Token::CLOSE_PAREN;
        }
        case '+': {
            return Token::ADD_OP;
        }
        case '-': {
            return Token::SUB_OP;
        }
        case '*': {
            return Token::MUL_OP;
        }
        case '/': {
            return Token::DIV_OP;
        }
        case '=': {
            return Token::EQUAL_SIGN;
        }
        case ';': {
            return Token::END;
        }
        case ':': {
            return Token::FIELD;
        }
        case ' ': {
            return Token::WHITESPACE;
        }
        default: { return Token::INCOMPLETE; }
    }
}

bool Tokenizer::validVariableName(std::stringstream variableName) {
    return false;
}
