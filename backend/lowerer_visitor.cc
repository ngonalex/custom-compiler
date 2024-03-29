
#include "backend/lowerer_visitor.h"

namespace cs160 {
namespace backend {

std::string LowererVisitor::GetOutputArithmeticHelper(
    std::string output, int index, std::vector<std::string> printhelper) {
  output = output + blocks_[index]->target.reg().name() + " <- " +
           blocks_[index]->arg1.reg().name();

  output = output + " " + printhelper[blocks_[index]->op.opcode()] + " " +
           blocks_[index]->arg2.reg().name();

  return output;
}

std::string LowererVisitor::GetOutput() {
  // Iterate through the vector and print out each basic block
  std::string output = "";
  std::vector<std::string> printhelper = {"INT_LOAD",
                                          "VAR_LOAD",
                                          "VAR_ASSIGN_LOAD",
                                          "FUN_ARG_LOAD",
                                          "FUN_RET_LOAD",
                                          "+",
                                          "-",
                                          "*",
                                          "/",
                                          "<",
                                          "<=",
                                          ">",
                                          ">=",
                                          "==",
                                          "&&",
                                          "||",
                                          "¬",
                                          "while",
                                          "if",
                                          "jmp",
                                          "je",
                                          "jne",
                                          "jg",
                                          "jge",
                                          "jl",
                                          "jle",
                                          "MkLabel",
                                          "FUNCTIONCALL",
                                          "FUNRETURNEPILOGUE",
                                          "FUNCTIONDEF",
                                          "FUN_PROLOGUE",
                                          "FUN_EPILOGUE",
                                          "PRINT_ARITH",
                                          "NO_TYPE",
                                          "LHS_DEREFERENCE",
                                          "RHS_DEREFERENCE",
                                          "NEW_TUPLE",
                                          "VAR_CHILD_TUPLE"};

  for (unsigned int i = 0; i < blocks_.size(); ++i) {
    // If it's a just a int (Register without a name then access it's value)
    // Otherwise access its name
    OpcodeType opcodetype = blocks_[i]->op.opcode();
    switch (opcodetype) {
      case INT_LOAD:
        output = output + blocks_[i]->target.reg().name() + " <- " +
                 std::to_string(blocks_[i]->arg1.value());
        break;
      case VAR_ASSIGN_LOAD:
        output = output + blocks_[i]->target.reg().name() + " <- " +
                 blocks_[i]->arg1.reg().name();
        break;
      case FUN_ARG_LOAD:
        output = output + blocks_[i]->target.reg().name() + " <- " +
                 std::to_string(blocks_[i]->arg1.value());
        break;
      case LOG_NOT:
        output = output + blocks_[i]->target.reg().name() + " <- " +
                 printhelper[LOG_NOT] + blocks_[i]->arg1.reg().name();
        break;
      case LOOP:
        output = output + printhelper[LOOP] + " " +
                 blocks_[i]->arg2.reg().name() + " == 0";
        break;
      case CONDITIONAL:
        output = output + printhelper[CONDITIONAL] + " " +
                 blocks_[i]->arg2.reg().name() + " == 0";
        break;
      // Abstract jumps out
      case JUMP:
        output = output + printhelper[JUMP] + " " +
                 blocks_[i]->target.label().name();
        break;
      case JMP_EQUAL:
        output = output + printhelper[JMP_EQUAL] + " " +
                 blocks_[i]->target.label().name();
        break;
      case JMP_NOT_EQUAL:
        output = output + printhelper[JMP_NOT_EQUAL] + " " +
                 blocks_[i]->target.label().name();
        break;
      case JMP_GREATER_THAN:
        output = output + printhelper[JMP_GREATER_THAN] + " " +
                 blocks_[i]->target.label().name();
        break;
      case JMP_GREATER_THAN_EQ:
        output = output + printhelper[JMP_GREATER_THAN_EQ] + " " +
                 blocks_[i]->target.label().name();
        break;
      case JMP_LESS_THAN:
        output = output + printhelper[JMP_LESS_THAN] + " " +
                 blocks_[i]->target.label().name();
        break;
      case JMP_LESS_THAN_EQ:
        output = output + printhelper[JMP_LESS_THAN_EQ] + " " +
                 blocks_[i]->target.label().name();
        break;
      case LABEL:
        output = output + printhelper[LABEL] + " " +
                 blocks_[i]->target.label().name();
        break;
      default:
        output = GetOutputArithmeticHelper(output, i, printhelper);
        break;
    }
    output = output + "\n";
  }

  return output;
}

void LowererVisitor::VisitDereference(const Dereference& exp) {
  currvariabletype_ = LEFT_HAND_VAR;
  exp.lhs().Visit(const_cast<LowererVisitor*>(this));
  ChildType lhschildtype = lastchildtype_;
  currvariabletype_ = RIGHT_HAND_VAR;
  int indexoflastchild = blocks_.size() - 1;

  exp.rhs().Visit(const_cast<LowererVisitor*>(this));

  auto rhsvirtualreg = blocks_[blocks_.size() - 1]->target.reg().name();
  std::string leftbasevariable;
  std::string leftderefvariable;

  switch (lhschildtype) {
    case DEREFCHILD:
      leftderefvariable = variablestack_.top();
      variablestack_.pop();
      leftbasevariable = variablestack_.top();
      leftderefvariable = leftderefvariable + "->" + rhsvirtualreg;
      variablestack_.push(leftderefvariable);
      break;
    case VARCHILD:
      leftbasevariable = variablestack_.top();
      if (globalset_.count(leftbasevariable) == 0) {
        std::cerr << "Variable " + leftbasevariable +
                         " cannot be "
                         "dereferenced because it has not been assigned\n";
        exit(1);
      }
      leftderefvariable = leftbasevariable + "->" + rhsvirtualreg;
      variablestack_.push(leftderefvariable);
      indexoflastchild = -1;
      break;
    default:
      std::cerr << "Inside Dereference Something went very wrong\n";
      break;
  }

  CreateDereference(leftbasevariable, leftderefvariable, indexoflastchild);
  lastchildtype_ = DEREFCHILD;
}

void LowererVisitor::VisitAssignmentFromNewTuple(
    const AssignmentFromNewTuple& assignment) {
  // Visit the left which will add its variable name to the stack

  currvariabletype_ = LEFT_HAND_VAR;
  currdereferencetype_ = LHS_DEREFERENCE;
  assignment.lhs().Visit(const_cast<LowererVisitor*>(this));
  currdereferencetype_ = RHS_DEREFERENCE;
  currvariabletype_ = RIGHT_HAND_VAR;

  std::string lhstarget;
  std::string lhsbase;
  if (lastchildtype_ == DEREFCHILD) {
    lhstarget = variablestack_.top();
    variablestack_.pop();
    lhsbase = variablestack_.top();
    variablestack_.pop();
  } else {  // It must be a varchild or somethign went really wrong
    lhstarget = variablestack_.top();
    variablestack_.pop();
    lhsbase = "";
    // Add to set
    globalset_.insert(lhstarget);
    totalset_.insert(lhstarget);

    auto block = make_unique<struct ThreeAddressCode>();
    block->target = Target(Register(lhstarget, VARIABLE_REG));
    block->op = Opcode(VAR_CHILD_TUPLE);
    blocks_.push_back(std::move(block));
  }

  // assign the right hand side to be equal to the left hand side
  // The latest vector addition is the final register to be set to the
  // string name
  assignment.rhs().Visit(const_cast<LowererVisitor*>(this));

  Operand arg1 = Operand(blocks_[blocks_.size() - 1]->target.reg());

  CreateTupleAssignment(lhstarget, arg1);
}

void LowererVisitor::VisitAssignmentFromArithExp(
    const AssignmentFromArithExp& assignment) {
  // Visit the left which will add its variable name to the stack
  currvariabletype_ = LEFT_HAND_VAR;
  currdereferencetype_ = LHS_DEREFERENCE;
  assignment.lhs().Visit(const_cast<LowererVisitor*>(this));
  currdereferencetype_ = RHS_DEREFERENCE;
  currvariabletype_ = RIGHT_HAND_VAR;

  std::string lhstarget;
  std::string lhsbase;

  // Handled differently to support how we used to load
  // to variables, consider having a function for just
  // Assignments
  if (lastchildtype_ == DEREFCHILD) {
    // Change this stuff later
    lhstarget = variablestack_.top();
    variablestack_.pop();
    // This should be base
    lhsbase = variablestack_.top();
    variablestack_.pop();
  } else {  // It must be a varchild or something went really wrong
    lhstarget = variablestack_.top();
    variablestack_.pop();
    lhsbase = "";
    globalset_.insert(lhstarget);
    totalset_.insert(lhstarget);
  }

  // assign the right hand side to be equal to the left hand side
  // The latest vector addition is the final register to be set to the
  // string name
  assignment.rhs().Visit(const_cast<LowererVisitor*>(this));

  Operand arg1 = Operand(blocks_[blocks_.size() - 1]->target.reg());
  // if (lastchildtype_ == DEREFCHILD) {
  //   blocks_[blocks_.size()-1]->op = Opcode(RHSTUPLEDEREFERENCE);
  // }
  CreateArithmeticAssignment(lhstarget, arg1);
}

void LowererVisitor::VisitVariableExpr(const VariableExpr& exp) {
  // Just get the string stored in VariableExpr and push it to
  // the stack
  variablestack_.push(exp.name());
  if (currvariabletype_ == LEFT_HAND_VAR) {
    // It's a load into operation
    // Var assignload will take care of it
    lastchildtype_ = VARCHILD;
    return;
  } else {
    // It's a right hand side so just access its
    // value and put it in a virt reg
    CreateLoadBlock(VAR_LOAD, Operand(0));
    ++counter_.variablecount;
  }
}

void LowererVisitor::VisitFunctionCall(const FunctionCall& call) {
  // - This DOES NOT Signal to code gen
  // CreateFunctionCallSignal(call.callee_name()));
  // Check if this functions is defined
  if (functioncheck_.find(call.callee_name()) == functioncheck_.end() ||
      functioncheck_.find(call.callee_name())->second !=
          call.arguments().size()) {
    std::cerr << "called undefined function" << std::endl;
    exit(1);
  }

  // Evaluate the arguments to a single value
  // Do it backwards to make loading into the stack easier
  for (int i = call.arguments().size() - 1; i >= 0; --i) {
    call.arguments()[i]->Visit(this);
  }

  // Creae a call TAC
  CreateFunctionCallBlock(call.callee_name());

  // At this point the top X things on the stack should be
  // the arguments of the function in the correct order

  // Function should return here, get its return value
  // lhs is a variable expr
  currvariabletype_ = LEFT_HAND_VAR;
  call.lhs().Visit(this);
  currvariabletype_ = RIGHT_HAND_VAR;

  // Basically do an assignment here
  CreateLoadBlock(FUN_RET_LOAD, Operand(0));

  // Restore the stack (Based on # of args)
  // (add $8*#args %esp)
  CreateFunctionCallReturnEpilogue(call.arguments().size());
}

void LowererVisitor::VisitFunctionDef(const FunctionDef& def) {
  // Signal to codegen a function def is occuring
  CreateFunctionDefSignal(def.function_name());

  // Create a label for the function
  CreateLabelBlock(def.function_name());

  // Create function prologue
  // push %rbp
  // mov %rsp, %rbp
  // Potentially push rbx?
  CreateFunctionDefPrologue(def.function_name());
  int prologueindex = blocks_.size() - 1;

  // Use a function specific stack here to keep track of variables
  std::set<std::string> originalglobalset(globalset_);
  globalset_.clear();

  std::set<std::string> originaltotalset(totalset_);
  totalset_.clear();

  // Move arguments into the local stack
  currvariabletype_ = LEFT_HAND_VAR;
  for (int i = def.parameters().size() - 1; i >= 0; --i) {
    def.parameters()[i]->Visit(this);
    CreateLoadBlock(FUN_ARG_LOAD, Operand(i));
  }
  currvariabletype_ = RIGHT_HAND_VAR;

  // Eval the body
  for (auto& statement : def.function_body()) {
    statement->Visit(this);
  }

  // Check how many variables there are after
  int numoflocalvars = totalset_.size();
  // std::cerr << "SIZE: " << numoflocalvars << std::endl;
  // Modify the prologue so it can create space for ALL local
  // variables
  blocks_[prologueindex]->arg1 = Operand(numoflocalvars);

  // Eval the arithmetic expr (will get pushed to stack)
  def.retval().Visit(this);

  // Create a returnblock
  CreateFunctionDefEpilogue(def.function_name());

  globalset_ = originalglobalset;
  totalset_ = originaltotalset;
}

void LowererVisitor::VisitLessThanExpr(const LessThanExpr& exp) {
  // Visit left hand side (Last thing should be the target where it stores it)
  exp.lhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg1 = GetArgument(lastchildtype_);
  exp.rhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg2 = GetArgument(lastchildtype_);

  BinaryOperatorHelper(LESS_THAN, arg1, arg2);
}
void LowererVisitor::VisitLessThanEqualToExpr(const LessThanEqualToExpr& exp) {
  // Visit left hand side (Last thing should be the target where it stores it)
  exp.lhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg1 = GetArgument(lastchildtype_);
  exp.rhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg2 = GetArgument(lastchildtype_);

  BinaryOperatorHelper(LESS_THAN_EQ, arg1, arg2);
}
void LowererVisitor::VisitGreaterThanExpr(const GreaterThanExpr& exp) {
  // Visit left hand side (Last thing should be the target where it stores it)
  exp.lhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg1 = GetArgument(lastchildtype_);
  exp.rhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg2 = GetArgument(lastchildtype_);

  BinaryOperatorHelper(GREATER_THAN, arg1, arg2);
}
void LowererVisitor::VisitGreaterThanEqualToExpr(
    const GreaterThanEqualToExpr& exp) {
  // Visit left hand side (Last thing should be the target where it stores it)
  exp.lhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg1 = GetArgument(lastchildtype_);
  exp.rhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg2 = GetArgument(lastchildtype_);

  BinaryOperatorHelper(GREATER_THAN_EQ, arg1, arg2);
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

  BinaryOperatorHelper(LOG_AND, arg1, arg2);
}
void LowererVisitor::VisitLogicalOrExpr(const LogicalOrExpr& exp) {
  // Visit left hand side (Last thing should be the target where it stores it)
  exp.lhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg1 = GetArgument(lastchildtype_);
  exp.rhs().Visit(const_cast<LowererVisitor*>(this));
  Register arg2 = GetArgument(lastchildtype_);

  BinaryOperatorHelper(LOG_OR, arg1, arg2);
}
void LowererVisitor::VisitLogicalNotExpr(const LogicalNotExpr& exp) {
  // Visit left hand side (Last thing should be the target where it stores it)
  exp.operand().Visit(const_cast<LowererVisitor*>(this));
  Register arg1 = GetArgument(lastchildtype_);

  BinaryOperatorHelper(LOG_NOT, arg1, Register());
}
void LowererVisitor::VisitConditional(const Conditional& conditional) {
  // Make the IR of the guard first
  // We don't "evaluate" the conditional until the boolean
  // value of the guard has been reached
  conditional.guard().Visit(const_cast<LowererVisitor*>(this));

  // Create a cmp to let codegen know it's a branch coming
  CreateComparisionBlock(CONDITIONAL);

  // Jump conditional to the false branch here
  std::string falselabel = JumpLabelHelper();
  CreateJumpBlock(falselabel, JMP_EQUAL);

  // Save the state of the last variable set
  std::set<std::string> originalset_ = globalset_;

  // Do this normally
  for (auto& statement : conditional.true_branch()) {
    statement->Visit(this);
  }

  // Push the set into the vector to check after
  localsets_.push_back(globalset_);
  // Restore the state of the globalset
  globalset_ = originalset_;

  // Create a jump to the continue
  std::string continuelabel = ContinueLabelHelper();
  CreateJumpBlock(continuelabel, JUMP);

  // Create a false label
  CreateLabelBlock(falselabel);

  // This should be inside the false label
  for (auto& statement : conditional.false_branch()) {
    statement->Visit(this);
  }

  // Push the set into the vector
  localsets_.push_back(globalset_);
  // Restore the state of the global set
  globalset_ = originalset_;

  std::set<std::string> s1 = localsets_[localsets_.size() - 1];
  std::set<std::string> s2 = localsets_[localsets_.size() - 2];
  std::set<std::string> intersectionset(GetSetIntersection(s1, s2));
  std::set<std::string> differenceset(GetSetDifference(s1, s2));

  // std::cerr << "SIZE OF SET1: " << s1.size() << std::endl;
  // std::cerr << "SIZE OF SET2: " << s2.size() << std::endl;
  // std::cerr << "SIZE OF INTER SET: " << intersectionset.size() << std::endl;
  // std::cerr << "SIZE OF DIFF SET: " << differenceset.size() << std::endl;

  totalset_.insert(intersectionset.begin(), intersectionset.end());
  totalset_.insert(differenceset.begin(), differenceset.end());

  // Check here if there's are new variable assigned in BOTH branches
  if (differenceset.size() == 0) {
    // we can now safely copy one of the local sets
    // as a globalset
    globalset_ = localsets_[localsets_.size() - 1];
  } else {
    // otherwise use only the intersection set
    // std::cerr << "A variable assignment in only one branch has been detected"
    //   << ", may cause problems later" << std::endl;
    globalset_ = intersectionset;
  }

  // Delete the last two
  localsets_.pop_back();
  localsets_.pop_back();

  // TAC to jump to continue here
  CreateJumpBlock(continuelabel, JUMP);

  // Create a continue label (Entrypoint back to main prog)
  CreateLabelBlock(continuelabel);
}
void LowererVisitor::VisitLoop(const Loop& loop) {
  // Similar to branching (Again flip conditionals + eval variables)
  // Probably have a TAC to create a new label here

  // create a looplabel
  std::string looplabel = LoopLabelHelper();
  CreateLabelBlock(looplabel);

  // Eval guard
  loop.guard().Visit(const_cast<LowererVisitor*>(this));

  // Create a loop to let codegen know it's a loop coming
  CreateComparisionBlock(LOOP);

  // Jump conditional to the continue here
  std::string continuelabel = ContinueLabelHelper();
  CreateJumpBlock(continuelabel, JMP_EQUAL);

  // Save the state of the last variable set
  // Not as tested as the conditional version
  std::set<std::string> originalset_ = globalset_;

  // Do this normally
  for (auto& statement : loop.body()) {
    statement->Visit(this);
  }

  std::set<std::string> intersectionset(
      GetSetIntersection(originalset_, globalset_));
  std::set<std::string> differenceset(
      GetSetDifference(originalset_, globalset_));

  // Add set difference to the totalset
  totalset_.insert(intersectionset.begin(), intersectionset.end());
  totalset_.insert(differenceset.begin(), differenceset.end());

  if (differenceset.size() > 0) {
    // std::cerr << "Unassigned Variable Detected (Loop)"
    //   <<", may cause problems later\n";

    // reset the global set
    globalset_ = originalset_;
  }

  // Jump to the loop again
  CreateJumpBlock(looplabel, JUMP);

  // Create a continue label
  CreateLabelBlock(continuelabel);
}

void LowererVisitor::VisitProgram(const Program& program) {
  // Do all the Assignments, then the AE, then the functions

  for (auto& def : program.function_defs()) {
    if (functioncheck_.find(def->function_name()) == functioncheck_.end()) {
      functioncheck_.insert(std::pair<std::string, int>(
          def->function_name(), def->parameters().size()));
    } else {
      if (functioncheck_.find(def->function_name())->second ==
          def->parameters().size()) {
        std::cerr << "function redefined" << std::endl;
        exit(1);
      }
    }
  }

  for (auto& statement : program.statements()) {
    statement->Visit(this);
  }

  program.arithmetic_exp().Visit(this);
  // Make a print block
  auto newblock = make_unique<struct ThreeAddressCode>();
  newblock->op = Opcode(PRINT_ARITH);
  blocks_.push_back(std::move(newblock));

  for (auto& def : program.function_defs()) {
    def->Visit(this);
  }
}

void LowererVisitor::VisitIntegerExpr(const IntegerExpr& exp) {
  // Load value into a target (t <- value)

  Operand arg1 = Operand(exp.value());
  CreateLoadBlock(INT_LOAD, arg1);
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

void LowererVisitor::CreateLoadBlock(OpcodeType type, Operand arg1) {
  ASSERT(type == INT_LOAD || type == VAR_ASSIGN_LOAD || type == FUN_ARG_LOAD ||
             type == FUN_RET_LOAD || type == VAR_LOAD,
         "Must be an Int, Variable, or function load\n");

  auto newblock = make_unique<struct ThreeAddressCode>();
  std::string varname;
  switch (type) {
    case VAR_LOAD:
      varname = variablestack_.top();
      variablestack_.pop();

      if (globalset_.count(varname) == 0) {
        std::cerr << "Variable " + varname + " not assigned\n";
        exit(1);
      }

      newblock->target = Target(
          Register("t_" + std::to_string(counter_.variablecount), VIRTUAL_REG));
      newblock->op = Opcode(VAR_LOAD);
      // Not consistent rework later
      newblock->arg1 = Operand(Register(varname, VARIABLE_REG));

      blocks_.push_back(std::move(newblock));
      break;
    case INT_LOAD:
      newblock->arg1 = arg1;
      newblock->op = Opcode(INT_LOAD);

      // look at this later,just going to do this now to test some things
      newblock->target = Target(
          Register("t_" + std::to_string(counter_.variablecount), VIRTUAL_REG));

      // Push into vector
      blocks_.push_back(std::move(newblock));
      break;
    case FUN_RET_LOAD:
      varname = variablestack_.top();
      variablestack_.pop();

      // Push variablename into the set (Flagging it as being assigned)
      globalset_.insert(varname);
      totalset_.insert(varname);

      newblock->target = Target(Register(varname, VARIABLE_REG));
      newblock->op = Opcode(FUN_RET_LOAD);
      newblock->arg1 = Operand(Register("FUN_RET_LOAD", VIRTUAL_REG));

      blocks_.push_back(std::move(newblock));
      break;
    case FUN_ARG_LOAD:
      varname = variablestack_.top();
      variablestack_.pop();

      // Push variablename into the set (Flagging it as being assigned)
      globalset_.insert(varname);
      totalset_.insert(varname);

      newblock->target = Target(Register(varname, VARIABLE_REG));
      newblock->op = Opcode(FUN_ARG_LOAD);
      newblock->arg1 = arg1;

      blocks_.push_back(std::move(newblock));
      break;
    default:
      break;
  }
}

void LowererVisitor::CreateComparisionBlock(OpcodeType type) {
  ASSERT(type == CONDITIONAL || type == LOOP,
         "Must be a loop or a conditional type");

  auto newblock = make_unique<struct ThreeAddressCode>();
  newblock->arg2 = Operand(blocks_[blocks_.size() - 1]->target.reg());
  // Flip the comparision so it jumps if it's negative
  newblock->arg1 = Operand(0);

  newblock->op = Opcode(type);

  blocks_.push_back(std::move(newblock));
}

void LowererVisitor::CreateLabelBlock(std::string labelname) {
  auto labelblock = make_unique<struct ThreeAddressCode>();
  labelblock->target = Target(Label(labelname));
  labelblock->op = Opcode(LABEL);
  blocks_.push_back(std::move(labelblock));
}

void LowererVisitor::CreateJumpBlock(std::string jumpname, OpcodeType type) {
  ASSERT(type == JUMP || type == JMP_EQUAL || type == JMP_GREATER_THAN ||
             type == JMP_GREATER_THAN_EQ || type == JMP_LESS_THAN ||
             type == JMP_LESS_THAN_EQ || type == JMP_NOT_EQUAL,
         "Must be a jump type");

  auto jumpblock = make_unique<struct ThreeAddressCode>();
  jumpblock->target = Target(Label(jumpname));
  jumpblock->op = Opcode(type);
  blocks_.push_back(std::move(jumpblock));
}

void LowererVisitor::CreateFunctionCallBlock(std::string funname) {
  auto callblock = make_unique<struct ThreeAddressCode>();
  callblock->target = Target(Label(funname));
  callblock->op = Opcode(FUN_CALL);

  blocks_.push_back(std::move(callblock));
}

void LowererVisitor::CreateFunctionCallReturnEpilogue(int numofregs) {
  auto callblock = make_unique<struct ThreeAddressCode>();
  callblock->target = Target(Register());
  callblock->op = Opcode(FUN_RET_EP);
  callblock->arg1 = Operand(numofregs);
  blocks_.push_back(std::move(callblock));
}

void LowererVisitor::CreateFunctionDefSignal(std::string name) {
  auto block = make_unique<struct ThreeAddressCode>();
  block->target = Target(Label(name));
  block->op = Opcode(FUN_DEF);

  blocks_.push_back(std::move(block));
}

void LowererVisitor::CreateFunctionDefPrologue(std::string name) {
  auto block = make_unique<struct ThreeAddressCode>();
  block->target = Target(Label(name));
  block->op = Opcode(FUN_PROLOGUE);

  blocks_.push_back(std::move(block));
}

void LowererVisitor::CreateFunctionDefEpilogue(std::string name) {
  auto block = make_unique<struct ThreeAddressCode>();
  block->target = Target(Label(name));
  block->op = Opcode(FUN_EPILOGUE);

  blocks_.push_back(std::move(block));
}

void LowererVisitor::CreateDereference(std::string basevariable,
                                       std::string targetvariable,
                                       int indexofchild) {
  auto block = make_unique<struct ThreeAddressCode>();
  block->target = Target(Register(targetvariable, DEREF_REG));
  block->op = Opcode(currdereferencetype_);
  block->arg1 = Operand(Register(basevariable, DEREF_REG));
  block->arg2 = Operand(Register("Parent", DEREF_REG));
  if (indexofchild > 0) {
    blocks_[indexofchild]->arg2 = Operand(Register("Child", DEREF_REG));
  }
  blocks_.push_back(std::move(block));
}

// Can combine CreateTupleAssignment and CreateArithmeticAssignment
// into one function
void LowererVisitor::CreateTupleAssignment(std::string target,
                                           Operand operand) {
  auto block = make_unique<struct ThreeAddressCode>();
  block->target = Target(Register(target, VARIABLE_REG));
  block->op = Opcode(NEW_TUPLE);
  block->arg1 = operand;

  blocks_.push_back(std::move(block));
}

void LowererVisitor::CreateArithmeticAssignment(std::string target,
                                                Operand operand) {
  auto block = make_unique<struct ThreeAddressCode>();
  block->target = Target(Register(target, VARIABLE_REG));
  block->op = Opcode(VAR_ASSIGN_LOAD);
  block->arg1 = operand;

  blocks_.push_back(std::move(block));
}

std::string LowererVisitor::JumpLabelHelper() {
  std::string newlabel = "falsebranch" + std::to_string(counter_.branchcount);
  ++counter_.branchcount;
  return newlabel;
}

std::string LowererVisitor::LoopLabelHelper() {
  std::string newloop = "loop" + std::to_string(counter_.loopcount);
  ++counter_.loopcount;
  return newloop;
}

std::string LowererVisitor::ContinueLabelHelper() {
  std::string newcontinue = "continue" + std::to_string(counter_.continuecount);
  ++counter_.continuecount;
  return newcontinue;
}

// Obsolete Function now
Register LowererVisitor::GetArgument(ChildType type) {
  Register arg = blocks_[blocks_.size() - 1]->target.reg();
  return arg;
}

void LowererVisitor::BinaryOperatorHelper(OpcodeType type, Register arg1,
                                          Register arg2) {
  // Load value into target (t <- prev->target + prev->prev->target)
  // Last two elements of the vector should be the integers to load in

  auto newblock = make_unique<struct ThreeAddressCode>();

  // Check for variable non assignment here?
  newblock->arg1 = Operand(arg1);

  if (arg2.name() != "") {
    newblock->arg2 = Operand(arg2);
  }

  newblock->op = Opcode(type);
  newblock->target = Target(
      Register("t_" + std::to_string(counter_.variablecount), VIRTUAL_REG));

  // Push into vector
  blocks_.push_back(std::move(newblock));

  ++counter_.variablecount;
  lastchildtype_ = BINOPCHILD;
}

std::set<std::string> LowererVisitor::GetSetDifference(
    std::set<std::string> set1, std::set<std::string> set2) {
  std::set<std::string> differenceset;
  set_symmetric_difference(set1.begin(), set1.end(), set2.begin(), set2.end(),
                           std::inserter(differenceset, differenceset.begin()));
  return differenceset;
}

std::set<std::string> LowererVisitor::GetSetIntersection(
    std::set<std::string> set1, std::set<std::string> set2) {
  // get the intersection of the two sets
  std::set<std::string> intersectionset;
  set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(),
                   std::inserter(intersectionset, intersectionset.begin()));
  return intersectionset;
}

}  // namespace backend
}  // namespace cs160
