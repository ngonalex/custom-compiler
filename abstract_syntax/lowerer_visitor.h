#ifndef ABSTRACT_SYNTAX_LOWERER_VISITOR_H_
#define ABSTRACT_SYNTAX_LOWERER_VISITOR_H_

#include <string>
#include <vector>

#include "abstract_syntax_tree.h"

namespace cs160{
namespace abstract_syntax{


//Abstract .h implementations to a .cc file later

//Structure to hold a 3Address, Basically 1 block
//Right now all of them are strings, but in the future I think that target should be from a symbol table
// and arg1/2 are either ints or from the symbol table
struct ThreeAddressCode {
  std::string target;
  std::string op;
  std::string arg1;
  std::string arg2;

  struct ThreeAddressCode* next;
  struct ThreeAddressCode* prev;
};

class LowererVisitor : public AstVisitor{
 public:
  LowererVisitor() {}
  ~LowererVisitor() {}

  std::vector<struct ThreeAddressCode*> blocks_;

  const std::string GetOutput() const {
    //Iterate through the vector and print out each basic block
    std::string output = "";
    for(int i = 0; i < blocks_.size(); ++i) {
      output = output + blocks_[i]->target + " <- " + blocks_[i]->arg1;

      if(blocks_[i]->op != "load") {
        output = output + " " + blocks_[i]->op + " " + blocks_[i]->arg2;
      }
      output = output + "\n";
    }
    return output;
  }

  void VisitIntegerExpr(const IntegerExpr& exp){
    //Load value into a target (t <- value)
    ThreeAddressCode* newblock = new ThreeAddressCode();

    newblock->arg1 = std::to_string(exp.value()); 
    newblock->op = "load";
    
    //This is probably wrong, look at this later, just going to do this now to test some things
    newblock->target = "t_" + std::to_string(blocks_.size());

    //Push into vector
    blocks_.push_back(newblock);

  }

  void VisitBinaryOperatorExpr(const BinaryOperatorExpr& exp){
  }

  void VisitAddExpr(const AddExpr& exp) {
    exp.lhs().Visit(const_cast<LowererVisitor*>(this));
    int leftindex = blocks_.size();
	  exp.rhs().Visit(const_cast<LowererVisitor*>(this));

    //Load value into target (t <- prev->target + prev->prev->target)
    //Last two elements of the vector should be the integers to load in
    int size = blocks_.size();

    ThreeAddressCode* newblock = new ThreeAddressCode();

    newblock->arg1 = blocks_[leftindex-1]->target;
    newblock->arg2 = blocks_[size-1]->target; 
    newblock->op = "+";
    //This is probably wrong, look at this later, just going to do this now to test some things
    newblock->target = "t_" + std::to_string(blocks_.size());

    //Push into vector
    blocks_.push_back(newblock);
  }


  void VisitSubtractExpr(const SubtractExpr& exp) {
   	//Visit left then right(eventually reaches the base case of Int)
	  exp.lhs().Visit(const_cast<LowererVisitor*>(this));

	  int leftindex = blocks_.size();
	  exp.rhs().Visit(const_cast<LowererVisitor*>(this));

    //Load value into target (t <- prev->target + prev->prev->target)
    //Last two elements of the vector should be the integers to load in
    int size = blocks_.size();

    ThreeAddressCode* newblock = new ThreeAddressCode();

    newblock->arg1 = blocks_[leftindex-1]->target;
    newblock->arg2 = blocks_[size-1]->target; 
    newblock->op = "-";
    //This is probably wrong, look at this later, just going to do this now to test some things
    newblock->target = "t_" + std::to_string(blocks_.size());

    //Push into vector
    blocks_.push_back(newblock);

  }

  void VisitMultiplyExpr(const MultiplyExpr& exp) {
   	//Visit lhs,rhs
	  exp.lhs().Visit(const_cast<LowererVisitor*>(this));

	  int leftindex = blocks_.size();
	  exp.rhs().Visit(const_cast<LowererVisitor*>(this));

    //Load value into target (t <- prev->target + prev->prev->target)
    //Last two elements of the vector should be the integers to load in
    int size = blocks_.size();

    ThreeAddressCode* newblock = new ThreeAddressCode();

    newblock->arg1 = blocks_[leftindex-1]->target;
    newblock->arg2 = blocks_[size-1]->target;  
    newblock->op = "*";
    //This is probably wrong, look at this later, just going to do this now to test some things
    newblock->target = "t_" + std::to_string(blocks_.size());

    //Push into vector
    blocks_.push_back(newblock);

  }

  void VisitDivideExpr(const DivideExpr& exp) {
	  //Visit left/right
	  exp.lhs().Visit(const_cast<LowererVisitor*>(this));
	  
    int leftindex = blocks_.size();
	  exp.rhs().Visit(const_cast<LowererVisitor*>(this));

    //Load value into target (t <- prev->target + prev->prev->target)
    //Last two elements of the vector should be the integers to load in
    int size = blocks_.size();

    ThreeAddressCode* newblock = new ThreeAddressCode();

    newblock->arg1 = blocks_[leftindex-1]->target;
    newblock->arg2 = blocks_[size-1]->target; 
    newblock->op = "/";
    //This is probably wrong, look at this later, just going to do this now to test some things
    newblock->target = "t_" + std::to_string(blocks_.size());

    //Push into vector
    blocks_.push_back(newblock);
  }



};

}//namespace abstract_syntax
}//namespace cs160

#endif //ABSTRACT_SYNTAX_LOWERER_VISITOR_H_