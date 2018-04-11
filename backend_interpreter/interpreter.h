#include "abstract_syntax_tree/abstract_syntax_tree.h";

namespace cs160 {
namespace interpreter{
class interpreter{
    public:
    interpreter(AstNode root){
        this.root = root;
    }

    int eva(AstNode node, int value){
        //typeof???????????
        if(typeof(node) == IntegerExpr){
            return value;
        }else if(typeof(node) == AddExpr){
            return eva(node)
        }


    }




    private:
    AstNode root;
};

}//namespace interpreter
}//namespace cs160
