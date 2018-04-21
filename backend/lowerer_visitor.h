#ifndef BACKEND_LOWERER_VISITOR_H_
#define BACKEND_LOWERER_VISITOR_H_

#include <string>
#include <vector>

//#include "utility/assert.h"
#include "abstract_syntax/abstract_syntax.h"
#include "backend/ir.h"

using cs160::abstract_syntax::backend::AstVisitor;
using cs160::abstract_syntax::backend::IntegerExpr;
using cs160::abstract_syntax::backend::AddExpr;
using cs160::abstract_syntax::backend::SubtractExpr;
using cs160::abstract_syntax::backend::MultiplyExpr;
using cs160::abstract_syntax::backend::DivideExpr;
using cs160::abstract_syntax::backend::BinaryOperatorExpr;

namespace cs160 {
namespace backend {


// Abstract ->h implementations to a ->cc file later
class LowererVisitor : public AstVisitor {
 public:
  LowererVisitor() {}
  ~LowererVisitor() {}

  const std::string GetOutput() const {
    // Iterate through the vector and print out each basic block
    std::string output = "";
    std::vector<std::string> printhelper = {"load", "+", "-", "*", "/"};

    for (unsigned int i = 0; i < blocks_.size(); ++i) {
      if (blocks_[i]->arg1.reg().name() == "") {
        output = output + blocks_[i]->target.name() + " <- " + std::to_string(blocks_[i]->arg1.value());
      } else {
          output = output + blocks_[i]->target.name() + " <- " + blocks_[i]->arg2.reg().name();
      }

      if (blocks_[i]->op != Opcode(LOAD)) {
        output = output + " " + printhelper[blocks_[i]->op.opcode()] + " " + blocks_[i]->arg2.reg().name();
      }

      output = output + "\n";
    }

  return output;
  }

  void VisitIntegerExpr(const IntegerExpr& exp) {
    // Load value into a target (t <- value)
    auto newblock = make_unique<struct ThreeAddressCode>();

    newblock->arg1 = Operand(exp.value());
    newblock->op = Opcode(LOAD);

    // look at this later,just going to do this now to test some things
    newblock->target = Register("t_" + std::to_string(blocks_.size()));

    // Push into vector
    blocks_.push_back(std::move(newblock));
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

    auto newblock = make_unique<struct ThreeAddressCode>();

    newblock->arg1 = Operand(blocks_[leftindex-1]->target);
    newblock->arg2 = Operand(blocks_[size-1]->target);

    newblock->op = Opcode(ADD);
    // look at this later, just going to do this now to test some things
    newblock->target = Register("t_" + std::to_string(blocks_.size()));

    // Push into vector
    blocks_.push_back(std::move(newblock));
  }

  void VisitSubtractExpr(const SubtractExpr& exp) {
    // Visit left then right(eventually reaches the base case of Int)
    exp.lhs().Visit(const_cast<LowererVisitor*>(this));

    int leftindex = blocks_.size();
    exp.rhs().Visit(const_cast<LowererVisitor*>(this));

    // Load value into target (t <- prev->target + prev->prev->target)
    // Last two elements of the vector should be the integers to load in
    int size = blocks_.size();

    auto newblock = make_unique<struct ThreeAddressCode>();

    newblock->arg1 = Operand(blocks_[leftindex-1]->target);
    newblock->arg2 = Operand(blocks_[size-1]->target);
    newblock->op = Opcode(SUB);
    // look at this later, just going to do this now to test some things
    newblock->target = Register("t_" + std::to_string(blocks_.size()));

    // Push into vector
    blocks_.push_back(std::move(newblock));
  }

  void VisitMultiplyExpr(const MultiplyExpr& exp) {
    // Visit lhs,rhs
    exp.lhs().Visit(const_cast<LowererVisitor*>(this));

    int leftindex = blocks_.size();
    exp.rhs().Visit(const_cast<LowererVisitor*>(this));

    // Load value into target (t <- prev->target + prev->prev->target)
    // Last two elements of the vector should be the integers to load in
    int size = blocks_.size();

    auto newblock = make_unique<struct ThreeAddressCode>();

    newblock->arg1 = Operand(blocks_[leftindex-1]->target);
    newblock->arg2 = Operand(blocks_[size-1]->target);
    newblock->op = Opcode(MULT);
    // look at this later, just going to do this now to test some things
    newblock->target = Register("t_" + std::to_string(blocks_.size()));

    // Push into vector
    blocks_.push_back(std::move(newblock));
  }

  void VisitDivideExpr(const DivideExpr& exp) {
    // Visit left/right
    exp.lhs().Visit(const_cast<LowererVisitor*>(this));

    int leftindex = blocks_.size();
    exp.rhs().Visit(const_cast<LowererVisitor*>(this));

    // Load value into target (t <- prev->target + prev->prev->target)
    // Last two elements of the vector should be the integers to load in
    int size = blocks_.size();

    auto newblock = make_unique<struct ThreeAddressCode>();

    newblock->arg1 = Operand(blocks_[leftindex-1]->target);
    newblock->arg2 = Operand(blocks_[size-1]->target);
    newblock->op = Opcode(DIV);
    // look at this later, just going to do this now to test some things
    newblock->target = Register("t_" + std::to_string(blocks_.size()));

    // Push into vector
    blocks_.push_back(std::move(newblock));
  }

  std::vector<std::unique_ptr<struct ThreeAddressCode>> GetIR() {
    return std::move(blocks_);
  }

 private:
  std::vector<std::unique_ptr<struct ThreeAddressCode>> blocks_;
};

}  // namespace backend
}  // namespace cs160

#endif  // BACKEND_LOWERER_VISITOR_H_
