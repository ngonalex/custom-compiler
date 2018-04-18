#include "./parser.h"

namespace cs160 {
namespace frontend {
  
  int main() {
    Token firstToken(Type::NUM, 6);
    Token secondToken(Type::DIV_OP);
    Token thirdToken(Type::NUM, 3);
    std::vector<Token> test_vector;
    test_vector.push_back(firstToken);
    test_vector.push_back(secondToken);
    test_vector.push_back(thirdToken);
    
    Parser parser(test_vector);
    parser.start();
    
    return 0;
  }

} // namespace frontend
} // namespace cs160