#include "abstract_syntax_tree.h";

namespace cs160{
namespace abstract_syntax{

class AstVisitor{
public:
   ~AstVisitor() {}

  // these should be able to change members of the visitor, thus not const
   void VisitIntegerExpr(const IntegerExpr& exp){
       return exp.value;
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

   void VisitAddExpr(const AddExpr& exp){
       exp.Visit();
       return 
   }
   void VisitSubtractExpr(const SubtractExpr& exp) = 0;
   void VisitMultiplyExpr(const MultiplyExpr& exp) = 0;
   void VisitDivideExpr(const DivideExpr& exp) = 0;


};

}//namespace abstract_syntax
}//namespace cs160