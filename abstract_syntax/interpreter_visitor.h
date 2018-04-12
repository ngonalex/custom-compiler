#include "abstract_syntax_tree.h";

namespace cs160{
namespace abstract_syntax{

class InterpreterVisitor : AstVisitor{
public:
   InterpreterVisitor() {}
   ~InterpreterVisitor() {}

  // these should be able to change members of the visitor, thus not const
   void VisitIntegerExpr(const IntegerExpr& exp){
       //push value to stack
       opstack.push(exp.value);
   }

   void VisitBinaryOperatorExpr(const BinaryOperatorExpr& exp){
   }

   void VisitAddExpr(const AddExpr& exp) {
   	//Visit left then right (eventually reaches the base case of Int
	//and that will push two values on the stack)
	exp.lhs().Visit(const_cast<InterpreterVisitor*>(this));
   	exp.rhs().Visit(const_cast<InterpreterVisitor*>(this));

	//Pop the top two (left,right), add them, and push it back on the stack
    int l = opstack.top();
    opstack.pop();
    int r = opstack.top();
    opstack.pop();
	opstack.push(l+r);
   }

   void VisitSubtractExpr(const SubtractExpr& exp) {
   	//Visit left then right(eventually reaches the base case of Int)
	//and that will push the two values on the stack 
	exp.lhs().Visit(const_cast<InterpreterVisitor*>(this));
	exp.rhs().Visit(const_cast<InterpreterVisitor*>(this));

	//Pop (left,right), subtract them, and push it back onto the stack
	int l = opstack.top();
    opstack.pop();
    int r = opstack.top();
    opstack.pop();
	opstack.push(l-r);
  
   }

   void VisitMultiplyExpr(const MultiplyExpr& exp) {
   	//Visit lhs,rhs
	exp.lhs().Visit(const_cast<InterpreterVisitor*>(this));
	exp.rhs().Visit(const_cast<InterpreterVisitor*>(this));

	//Pop top two, push result back in
	int l = opstack.top();
    opstack.pop();
    int r = opstack.top();
    opstack.pop();
	opstack.push(l*r);
   }

   void VisitDivideExpr(const DivideExpr& exp) {
	//Visit left/right
	exp.lhs().Visit(const_cast<InterpreterVisitor*>(this));
	exp.rhs().Visit(const_cast<InterpreterVisitor*>(this));

	//Pop top two, push result back in
	//Should we check for division by zero here?
	int l = opstack.top();
    opstack.pop();
    int r = opstack.top();
    opstack.pop();
	opstack.push(l/r);
   }

    //Not very general, this is probably a bad idea for future ASTS
private:
    std::stack<int> opstack;


};

}//namespace abstract_syntax
}//namespace cs160
