
#include "backend/lowerer_visitor.h"

namespace cs160 {
namespace backend {

// Abstract ->h implementations to a ->cc file later
const std::string LowererVisitor::GetOutput() const {
  // Iterate through the vector and print out each basic block
  std::string output = "";
  std::vector<std::string> printhelper = {"load", "+", "-", "*", "/", "<", "<=",
    ">", ">=", "==", "&&", "||", "!"};

  for (unsigned int i = 0; i < blocks_.size(); ++i) {
    // If it's a just a int (Register without a name then access it's value)
    // Otherwise access its name
    if (blocks_[i]->op.opcode() == LOAD) {
      if (blocks_[i]->arg1.optype() == INT) {
        output = output + blocks_[i]->target.name()
          + " <- " + std::to_string(blocks_[i]->arg1.value());
      } else {
        output = output + blocks_[i]->target.name()
          + " <- " + blocks_[i]->arg1.reg().name();
      }
    } else {
        output = output + blocks_[i]->target.name()
          + " <- " + blocks_[i]->arg1.reg().name();
    }

    // If it's an arithmetic expr (not a load) then get the 2nd arg as well
    if (blocks_[i]->op != Opcode(LOAD)) {
      output = output + " " + printhelper[blocks_[i]->op.opcode()]
        + " " + blocks_[i]->arg2.reg().name();
    }

    output = output + "\n";
  }

  return output;
}

void LowererVisitor::VisitLessThanExpr(const LessThanExpr& exp) {
  // Visit left hand side (Last thing should be the target where it stores it)  
  exp.lhs().Visit(const_cast<LowererVisitor*>(this));
  int leftindex = blocks_.size();

  exp.rhs().Visit(const_cast<LowererVisitor*>(this));

  BinaryOperatorHelper(LESSTHAN, leftindex);


}
void LowererVisitor::VisitLessThanEqualToExpr(const LessThanEqualToExpr& exp) {
  // Visit left hand side (Last thing should be the target where it stores it)  
  exp.lhs().Visit(const_cast<LowererVisitor*>(this));
  int leftindex = blocks_.size();

  exp.rhs().Visit(const_cast<LowererVisitor*>(this));

  BinaryOperatorHelper(LESSTHANEQ, leftindex);
}
void LowererVisitor::VisitGreaterThanExpr(const GreaterThanExpr& exp) {
  // Visit left hand side (Last thing should be the target where it stores it)  
  exp.lhs().Visit(const_cast<LowererVisitor*>(this));
  int leftindex = blocks_.size();

  exp.rhs().Visit(const_cast<LowererVisitor*>(this));

  BinaryOperatorHelper(GREATERTHAN, leftindex);
}
void LowererVisitor::VisitGreaterThanEqualToExpr(const GreaterThanEqualToExpr& exp) {
    // Visit left hand side (Last thing should be the target where it stores it)  
  exp.lhs().Visit(const_cast<LowererVisitor*>(this));
  int leftindex = blocks_.size();

  exp.rhs().Visit(const_cast<LowererVisitor*>(this));

  BinaryOperatorHelper(GREATERTHANEQ, leftindex);
}
void LowererVisitor::VisitEqualToExpr(const EqualToExpr& exp) {
  // Visit left hand side (Last thing should be the target where it stores it)  
  exp.lhs().Visit(const_cast<LowererVisitor*>(this));
  int leftindex = blocks_.size();

  exp.rhs().Visit(const_cast<LowererVisitor*>(this));

  BinaryOperatorHelper(EQUAL, leftindex);
}
void LowererVisitor::VisitLogicalAndExpr(const LogicalAndExpr& exp) {
  // Visit left hand side (Last thing should be the target where it stores it)  
  exp.lhs().Visit(const_cast<LowererVisitor*>(this));
  int leftindex = blocks_.size();

  exp.rhs().Visit(const_cast<LowererVisitor*>(this));

  BinaryOperatorHelper(LOGAND, leftindex);
}
void LowererVisitor::VisitLogicalOrExpr(const LogicalOrExpr& exp) {
  // Visit left hand side (Last thing should be the target where it stores it)  
  exp.lhs().Visit(const_cast<LowererVisitor*>(this));
  int leftindex = blocks_.size();

  exp.rhs().Visit(const_cast<LowererVisitor*>(this));

  BinaryOperatorHelper(LOGOR, leftindex);
}
void LowererVisitor::VisitLogicalNotExpr(const LogicalNotExpr& exp) {
  // Visit left hand side (Last thing should be the target where it stores it)  
  exp.operand().Visit(const_cast<LowererVisitor*>(this));

  int size = blocks_.size();

  auto newblock = make_unique<struct ThreeAddressCode>();

  newblock->arg1 = Operand(blocks_[size-1]->target);

  newblock->op = Opcode(LOGNOT);
  // look at this later, just going to do this now to test some things
  newblock->target = Register("t_" + std::to_string(variablecounter_));

  // Push into vector
  blocks_.push_back(std::move(newblock));

  ++variablecounter_;
  
}
void LowererVisitor::VisitConditional(const Conditional& conditional) {

}
void LowererVisitor::VisitLoop(const Loop& loop) {

}

void LowererVisitor::VisitAssignment(const Assignment& assignment) {
  auto newblock = make_unique<struct ThreeAddressCode>();

  // Visit the left which will add its variable name to the stack
  assignment.lhs().Visit(const_cast<LowererVisitor*>(this));
  std::string varname = variablestack_.top();
  variablestack_.pop();

  // assign the right hand side to be equal to the left hand side
  // The latest vector addition is the final register to be set to the
  // string name
  assignment.rhs().Visit(const_cast<LowererVisitor*>(this));

  newblock->target = Register(varname);
  newblock->op = Opcode(LOAD);
  newblock->arg1 = Operand(blocks_[blocks_.size()-1]->target);

  blocks_.push_back(std::move(newblock));
}

void LowererVisitor::VisitProgram(const Program& program) {
  // Do all the Assignments, then eval the AE?

  for (auto& statement : program.statements()) {
      statement->Visit(this);
  }

  program.arithmetic_exp().Visit(this);
}

void LowererVisitor::VisitVariableExpr(const VariableExpr& exp) {
  // Just get the string stored in VariableExpr and push it to
  // the stack
  variablestack_.push(exp.name());
}

// General comment, Lots of reused code, Think about abstracting out to helper
// functions as the only difference between add/sub/mult/div is their "sign"
// (Also some additional error handling for div req)

void LowererVisitor::BinaryOperatorHelper(Type type, int leftindex) {
  // Load value into target (t <- prev->target + prev->prev->target)
  // Last two elements of the vector should be the integers to load in
  int size = blocks_.size();

  auto newblock = make_unique<struct ThreeAddressCode>();

  newblock->arg1 = Operand(blocks_[leftindex-1]->target);
  newblock->arg2 = Operand(blocks_[size-1]->target);

  // if (type == DIV) {check for div zero?}

  newblock->op = Opcode(type);
  // look at this later, just going to do this now to test some things
  newblock->target = Register("t_" + std::to_string(variablecounter_));

  // Push into vector
  blocks_.push_back(std::move(newblock));

  ++variablecounter_;
}

void LowererVisitor::VisitIntegerExpr(const IntegerExpr& exp) {
  // Load value into a target (t <- value)
  auto newblock = make_unique<struct ThreeAddressCode>();

  newblock->arg1 = Operand(exp.value());
  newblock->op = Opcode(LOAD);

  // look at this later,just going to do this now to test some things
  newblock->target = Register("t_" + std::to_string(variablecounter_));

  // Push into vector
  blocks_.push_back(std::move(newblock));

  ++variablecounter_;
}

void LowererVisitor::VisitAddExpr(const AddExpr& exp) {
  exp.lhs().Visit(const_cast<LowererVisitor*>(this));
  int leftindex = blocks_.size();
  exp.rhs().Visit(const_cast<LowererVisitor*>(this));

  BinaryOperatorHelper(ADD, leftindex);
}

void LowererVisitor::VisitSubtractExpr(const SubtractExpr& exp) {
  // Visit left then right(eventually reaches the base case of Int)
  exp.lhs().Visit(const_cast<LowererVisitor*>(this));

  int leftindex = blocks_.size();
  exp.rhs().Visit(const_cast<LowererVisitor*>(this));

  BinaryOperatorHelper(SUB, leftindex);
}

void LowererVisitor::VisitMultiplyExpr(const MultiplyExpr& exp) {
  // Visit lhs,rhs
  exp.lhs().Visit(const_cast<LowererVisitor*>(this));

  int leftindex = blocks_.size();
  exp.rhs().Visit(const_cast<LowererVisitor*>(this));

  BinaryOperatorHelper(MULT, leftindex);
}

void LowererVisitor::VisitDivideExpr(const DivideExpr& exp) {
  // Visit left/right
  exp.lhs().Visit(const_cast<LowererVisitor*>(this));

  int leftindex = blocks_.size();
  exp.rhs().Visit(const_cast<LowererVisitor*>(this));

  BinaryOperatorHelper(DIV, leftindex);
}

}  // namespace backend
}  // namespace cs160
