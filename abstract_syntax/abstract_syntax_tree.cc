#include "abstract_syntax_tree.h";

namespace cs160{
namespace abstract_syntax{

class AstVisitor{
public:
   ~AstVisitor() {}

  // these should be able to change members of the visitor, thus not const
   void VisitIntegerExpr(const IntegerExpr& exp){
       //push value to stack
       opstack.push(exp.value);
   }

   void VisitBinaryOperatorExpr(const BinaryOperatorExpr& exp){
       switch((exp)){
            case AddExpr:
                VisitAddExpr(exp);
                break;
            case SubtractExpr:
                VisitSubtractExpr(exp);
                break;
            case MultiplyExpr:
                VisitMultiplyExpr(exp);
                break;
            case DivideExpr:
                VisitDivideExpr(exp);
                break;
            case default:
                break;
       }
   }

   void VisitAddExpr(const AddExpr& exp) {
   	//Visit left then right (eventually reaches the base case of Int
	//and that will push two values on the stack)
	exp.lhs().Visit(const_cast<AstVisitor*>(this));
   	exp.rhs().Visit(const_cast<AstVisitor*>(this));

	//Pop the top two (left,right), add them, and push it back on the stack
	opstack.push(opstack.pop()+opstack.pop());
   }

   void VisitSubtractExpr(const SubtractExpr& exp) {
   	//Visit left then right(eventually reaches the base case of Int)
	//and that will push the two values on the stack 
	exp.lhs().Visit(const_cast<AstVisitor*>(this));
	exp.rhs().Visit(const_cast<AstVisitor*>(this));

	//Pop (left,right), subtract them, and push it back onto the stack
	opstack.push(opstack.pop()-opstack.pop());
  
   }

   void VisitMultiplyExpr(const MultiplyExpr& exp) {
   	//Visit lhs,rhs
	exp.lhs().Visit(const_cast<AstVisitor*>(this));
	exp.rhs().Visit(const_cast<AstVisitor*>(this));

	//Pop top two, push result back in
	opstack.push(opstack.pop()*opstack.pop());
   }

   void VisitDivideExpr(const DivideExpr& exp) {
	//Visit left/right
	exp.lhs().Visit(const_cast<AstVisitor*>(this));
	exp.rhs().Visit(const_cast<AstVisitor*>(this));

	//Pop top two, push result back in
	//Should we check for division by zero here?
	opstack.push(opstack.pop()/opstack.pop());
   }


};

}//namespace abstract_syntax
}//namespace cs160
