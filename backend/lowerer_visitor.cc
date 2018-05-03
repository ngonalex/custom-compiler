
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
  Register arg1 = GetArgument(lastchildtype_);
  exp.rhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg2 = GetArgument(lastchildtype_);

  BinaryOperatorHelper(LESSTHAN, arg1, arg2);


}
void LowererVisitor::VisitLessThanEqualToExpr(const LessThanEqualToExpr& exp) {
  // Visit left hand side (Last thing should be the target where it stores it)  
  exp.lhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg1 = GetArgument(lastchildtype_);
  exp.rhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg2 = GetArgument(lastchildtype_);

  BinaryOperatorHelper(LESSTHANEQ, arg1, arg2);
}
void LowererVisitor::VisitGreaterThanExpr(const GreaterThanExpr& exp) {
  // Visit left hand side (Last thing should be the target where it stores it)  
  exp.lhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg1 = GetArgument(lastchildtype_);
  exp.rhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg2 = GetArgument(lastchildtype_);

  BinaryOperatorHelper(GREATERTHAN, arg1, arg2);
}
void LowererVisitor::VisitGreaterThanEqualToExpr(const GreaterThanEqualToExpr& exp) {
    // Visit left hand side (Last thing should be the target where it stores it)  
  exp.lhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg1 = GetArgument(lastchildtype_);
  exp.rhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg2 = GetArgument(lastchildtype_);

  BinaryOperatorHelper(GREATERTHANEQ, arg1, arg2);
}
void LowererVisitor::VisitEqualToExpr(const EqualToExpr& exp) {
  // Visit left hand side (Last thing should be the target where it stores it)  
  exp.lhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg1 = GetArgument(lastchildtype_);
  exp.rhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg2 = GetArgument(lastchildtype_);

  BinaryOperatorHelper(EQUAL, arg1, arg2);
}
void LowererVisitor::VisitLogicalAndExpr(const LogicalAndExpr& exp) {
  // Visit left hand side (Last thing should be the target where it stores it)  
  exp.lhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg1 = GetArgument(lastchildtype_);
  exp.rhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg2 = GetArgument(lastchildtype_);

  BinaryOperatorHelper(LOGAND, arg1, arg2);
}
void LowererVisitor::VisitLogicalOrExpr(const LogicalOrExpr& exp) {
  // Visit left hand side (Last thing should be the target where it stores it)  
  exp.lhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg1 = GetArgument(lastchildtype_);
  exp.rhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg2 = GetArgument(lastchildtype_);

  BinaryOperatorHelper(LOGOR, arg1, arg2);
}
void LowererVisitor::VisitLogicalNotExpr(const LogicalNotExpr& exp) {
  // Visit left hand side (Last thing should be the target where it stores it)  
  exp.operand().Visit(const_cast<LowererVisitor*>(this));
  Register arg1 = GetArgument(lastchildtype_);

  BinaryOperatorHelper(LOGNOT, arg1, Register());
  
}
void LowererVisitor::VisitConditional(const Conditional& conditional) {
  // Make the IR of the guard first
  // We don't "evaluate" the conditional until the boolean
  // value of the guard has been reached
  conditional.guard().Visit(const_cast<LowererVisitor*>(this));
  
  // So at this point the last statement should be something of the form
  // t_a <- t_b re t_c
  // How should we structure the if TAC
  // Ideas: 1) Structure it like the assembly instructions
  // If/Else branches in assembly are done with cmp, j[re] chains
  // Algorithm would be
  // 1) Look at the previous instructions opcode
  // 2) represent the "if" as a condition jump statement
  // Ex. if(x>5) translates to
  // t0 <- 5, t1 <- x > 5, block1 <- t1 jge/jmp (Basically if t1 is true
  // jump to block 1

  // Code to create if TAC here
  // Two notes: I looked at the gcc compiler and it'll reverse the conditional
  // e.x. x == 5 is represented as a jne
  // We need a way to keep track if x has been assigned (map of strings -> bools) maybe?

  // Check the previous statement and reverse it
  blocks_[blocks_.size()-1]->op.ChangeOpCode(
    JumpConditionalHelper(blocks_[blocks_.size()-1]->op.opcode()));

  // Jump conditional to the false branch here or if the false branch is empty
  // then just jump to the continue branch
  // Create a temp jump TAC + note the index return to this later
  auto jumpblock = make_unique<struct ThreeAddressCode>();
  int oldindex = blocks_.size()-1;

  // Do this normally
  for (auto& statement : conditional.true_branch()) {
    statement->Visit(this);
  }

  //Create a jump to the continue


  // This should be in a false_branch label
  // jump instruction
  for (auto& statement : conditional.false_branch()) {
    statement->Visit(this);
  }
  if (oldindex != blocks_.size()) {
    // TAC to jump to continue
  }

  for (auto& statement : conditional.true_branch()) {
    statement->Visit(this);
  }

  // TAC to jump to continue here


}
void LowererVisitor::VisitLoop(const Loop& loop) {

  // Similar to branching (Again flip conditionals + eval variables)
  // Probably have a TAC to create a new label here

  // First evaluate the guard
  loop.guard().Visit(const_cast<LowererVisitor*>(this));

  // if true, run the code 
  // otherwise jump to continue


}

void LowererVisitor::VisitAssignment(const Assignment& assignment) {
  auto newblock = make_unique<struct ThreeAddressCode>();

  // Visit the left which will add its variable name to the stack
  assignment.lhs().Visit(const_cast<LowererVisitor*>(this));
  std::string varname = variablestack_.top();
  variablestack_.pop();

  // Push variablename into the set (Flagging it as being assigned)
  variableset_.insert(varname);

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
  lastchildtype_ = VARCHILD;
}

// General comment, Lots of reused code, Think about abstracting out to helper
// functions as the only difference between add/sub/mult/div is their "sign"
// (Also some additional error handling for div req)

void LowererVisitor::BinaryOperatorHelper(Type type, Register arg1, Register arg2) {
  // Load value into target (t <- prev->target + prev->prev->target)
  // Last two elements of the vector should be the integers to load in
  int size = blocks_.size();

  auto newblock = make_unique<struct ThreeAddressCode>();

  // Check for variable non assignment here?
  newblock->arg1 = Operand(arg1);

  if (arg2.name() != "") {
    newblock->arg2 = Operand(arg2);
  }

  // if (type == DIV) {check for div zero?}

  newblock->op = Opcode(type);
  // look at this later, just going to do this now to test some things
  newblock->target = Register("t_" + std::to_string(counter_.variablecount));

  // Push into vector
  blocks_.push_back(std::move(newblock));

  ++counter_.variablecount;
}

void LowererVisitor::VisitIntegerExpr(const IntegerExpr& exp) {
  // Load value into a target (t <- value)
  auto newblock = make_unique<struct ThreeAddressCode>();

  newblock->arg1 = Operand(exp.value());
  newblock->op = Opcode(LOAD);

  // look at this later,just going to do this now to test some things
  newblock->target = Register("t_" + std::to_string(counter_.variablecount));

  // Push into vector
  blocks_.push_back(std::move(newblock));
  lastchildtype_ = INTCHILD;

  ++counter_.variablecount;
}

void LowererVisitor::VisitAddExpr(const AddExpr& exp) {
  exp.lhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg1 = GetArgument(lastchildtype_);
  exp.rhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg2 = GetArgument(lastchildtype_);
  BinaryOperatorHelper(ADD, arg1, arg2);
}

void LowererVisitor::VisitSubtractExpr(const SubtractExpr& exp) {
  // Visit left then right(eventually reaches the base case of Int)
  exp.lhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg1 = GetArgument(lastchildtype_);
  exp.rhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg2 = GetArgument(lastchildtype_);

  BinaryOperatorHelper(SUB, arg1, arg2);
}

void LowererVisitor::VisitMultiplyExpr(const MultiplyExpr& exp) {
  // Visit lhs,rhs
  exp.lhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg1 = GetArgument(lastchildtype_);
  exp.rhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg2 = GetArgument(lastchildtype_);

  BinaryOperatorHelper(MULT, arg1, arg2);
}

void LowererVisitor::VisitDivideExpr(const DivideExpr& exp) {
  // Visit left/right
  exp.lhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg1 = GetArgument(lastchildtype_);
  exp.rhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg2 = GetArgument(lastchildtype_);

  BinaryOperatorHelper(DIV, arg1, arg2);
}

Type LowererVisitor::JumpConditionalHelper(Type type) {
  // Return the opposite of what was given
  switch(type) {
    case JEQUAL:
      return JNOTEQUAL;
    case JNOTEQUAL:
      return JEQUAL;
    case JGREATER:
      return JLESS;
    case JGREATEREQ:
      return JLESSEQ;
    case JLESS:
      return JGREATER;
    case JLESSEQ:
      return JGREATEREQ;
    default:
      // Should I use a perror or ASSERT here
      // perror("Expected a Jump Type");
      ASSERT(true == false, "Must be a jump type");
      return NOTYPE;
      break;
  }
}

Register LowererVisitor::GetArgument(ChildType type) {
  Register arg;
  switch(lastchildtype_) {
    case INTCHILD:
      arg = blocks_[blocks_.size()-1]->target;
      break;
    case VARCHILD:
      // Check if variable has been assigned here?
      arg = Register(variablestack_.top());
      variablestack_.pop();
      break;
    default:
      // There should be type throw error?
      break;
  }
  return arg;
}


}  // namespace backend
}  // namespace cs160
