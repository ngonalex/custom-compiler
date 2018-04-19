#ifndef BACKEND_LOWERER_VISITOR_H_
#define BACKEND_LOWERER_VISITOR_H_

#include "abstract_syntax/abstract_syntax.h"

#include <string>
#include <vector>

using cs160::abstract_syntax::backend::AstVisitor;
using cs160::abstract_syntax::backend::IntegerExpr;
using cs160::abstract_syntax::backend::AddExpr;
using cs160::abstract_syntax::backend::SubtractExpr;
using cs160::abstract_syntax::backend::MultiplyExpr;
using cs160::abstract_syntax::backend::DivideExpr;
using cs160::abstract_syntax::backend::BinaryOperatorExpr;

namespace cs160 {
namespace backend {

// Abstract .h implementations to a .cc file later

// Structure to hold a 3Address, Basically 1 block
// Right now all of them are strings,
// but in the future I think that target should be from a symbol table
// and arg1/2 are either ints or from the symbol table
struct ThreeAddressCode {
  std::string target;
  std::string op;
  std::string arg1;
  std::string arg2;

  struct ThreeAddressCode* next;
  struct ThreeAddressCode* prev;
};

// For ThreeAddressCodes, arg1/arg2 can be a VirtualRegister or a Int
class VirtualRegister {
  explicit VirtualRegister(std::string name) : name_(name) {}

 private:
  std::string name_;
};

class Operand {
 explicit Operand(std::string op) : opcode_(op) {}

 private:
  std::string opcode_;
};

class LowererVisitor : public AstVisitor {
 public:
  LowererVisitor() {}
  ~LowererVisitor() {}

  std::vector<struct ThreeAddressCode*> blocks_;

  const std::string GetOutput() const {
    // Iterate through the vector and print out each basic block
    std::string output = "";

    for (unsigned int i = 0; i < blocks_.size(); ++i) {
      output = output + blocks_[i]->target + " <- " + blocks_[i]->arg1;
      
      if (blocks_[i]->op != "load") {
        output = output + " " + blocks_[i]->op + " " + blocks_[i]->arg2;
      }

      output = output + "\n";
    }
    
    return output;
  }

  void VisitIntegerExpr(const IntegerExpr& exp) {
    // Load value into a target (t <- value)
    ThreeAddressCode* newblock = new ThreeAddressCode();

    newblock->arg1 = std::to_string(exp.value());
    newblock->op = "load";

    // look at this later,just going to do this now to test some things
    newblock->target = "t_" + std::to_string(blocks_.size());

    // Push into vector
    blocks_.push_back(newblock);
  }

  void VisitBinaryOperatorExpr(const BinaryOperatorExpr& exp) {
  }

  void VisitAddExpr(const AddExpr& exp) {
    exp.lhs().Visit(const_cast<LowererVisitor*>(this));
    int leftindex = blocks_.size();
    exp.rhs().Visit(const_cast<LowererVisitor*>(this));

    // Load value into target (t <- prev->target + prev->prev->target)
    // Last two elements of the vector should be the integers to load in
    int size = blocks_.size();

    ThreeAddressCode* newblock = new ThreeAddressCode();

    newblock->arg1 = blocks_[leftindex-1]->target;
    newblock->arg2 = blocks_[size-1]->target;

    newblock->op = "+";
    // look at this later, just going to do this now to test some things
    newblock->target = "t_" + std::to_string(blocks_.size());

    // Push into vector
    blocks_.push_back(newblock);
  }


  void VisitSubtractExpr(const SubtractExpr& exp) {
    // Visit left then right(eventually reaches the base case of Int)
    exp.lhs().Visit(const_cast<LowererVisitor*>(this));

    int leftindex = blocks_.size();
    exp.rhs().Visit(const_cast<LowererVisitor*>(this));

    // Load value into target (t <- prev->target + prev->prev->target)
    // Last two elements of the vector should be the integers to load in
    int size = blocks_.size();

    ThreeAddressCode* newblock = new ThreeAddressCode();

    newblock->arg1 = blocks_[leftindex-1]->target;
    newblock->arg2 = blocks_[size-1]->target;
    newblock->op = "-";
    // look at this later, just going to do this now to test some things
    newblock->target = "t_" + std::to_string(blocks_.size());

    // Push into vector
    blocks_.push_back(newblock);
  }

  void VisitMultiplyExpr(const MultiplyExpr& exp) {
    // Visit lhs,rhs
    exp.lhs().Visit(const_cast<LowererVisitor*>(this));

    int leftindex = blocks_.size();
    exp.rhs().Visit(const_cast<LowererVisitor*>(this));

    // Load value into target (t <- prev->target + prev->prev->target)
    // Last two elements of the vector should be the integers to load in
    int size = blocks_.size();

    ThreeAddressCode* newblock = new ThreeAddressCode();

    newblock->arg1 = blocks_[leftindex-1]->target;
    newblock->arg2 = blocks_[size-1]->target;
    newblock->op = "*";
    // look at this later, just going to do this now to test some things
    newblock->target = "t_" + std::to_string(blocks_.size());

    // Push into vector
    blocks_.push_back(newblock);
  }

  void VisitDivideExpr(const DivideExpr& exp) {
    // Visit left/right
    exp.lhs().Visit(const_cast<LowererVisitor*>(this));

    int leftindex = blocks_.size();
    exp.rhs().Visit(const_cast<LowererVisitor*>(this));

    // Load value into target (t <- prev->target + prev->prev->target)
    // Last two elements of the vector should be the integers to load in
    int size = blocks_.size();

    ThreeAddressCode* newblock = new ThreeAddressCode();

    newblock->arg1 = blocks_[leftindex-1]->target;
    newblock->arg2 = blocks_[size-1]->target;
    newblock->op = "/";
    // look at this later, just going to do this now to test some things
    newblock->target = "t_" + std::to_string(blocks_.size());

    // Push into vector
    blocks_.push_back(newblock);
  }
};

}  // namespace backend
}  // namespace cs160

#endif  // BACKEND_LOWERER_VISITOR_H_
