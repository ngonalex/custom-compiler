
#include "backend/lowerer_visitor.h"

namespace cs160 {
namespace backend {

std::string LowererVisitor::GetOutputArithmeticHelper(std::string output,
  int index, std::vector<std::string> printhelper) {
  output = output + blocks_[index]->target.reg().name()
    + " <- " + blocks_[index]->arg1.reg().name();

  output = output + " " + printhelper[blocks_[index]->op.opcode()]
    + " " + blocks_[index]->arg2.reg().name();

  return output;
}

std::string LowererVisitor::GetOutput() {
  // Iterate through the vector and print out each basic block
  std::string output = "";

  // Probably make this a private variable or something?
  std::vector<std::string> printhelper = {"intload", "varload", "funload", "+",
    "-", "*", "/", "<", "<=", ">", ">=", "==", "&&", "||", "¬",
    "while", "if", "jmp", "je", "jne", "jg", "jge", "jl",
    "jle", "MkLabel"};

  for (unsigned int i = 0; i < blocks_.size(); ++i) {
    // If it's a just a int (Register without a name then access it's value)
    // Otherwise access its name
    Type opcodetype = blocks_[i]->op.opcode();
    switch (opcodetype) {
      case INTLOAD:
        output = output + blocks_[i]->target.reg().name()
          + " <- " + std::to_string(blocks_[i]->arg1.value());
        break;
      case VARLOAD:
        output = output + blocks_[i]->target.reg().name()
              + " <- " + blocks_[i]->arg1.reg().name();
      case FUNLOAD:
        break;
      // case ADD:
      //   GetOutputArithmeticHelper(output, i, printhelper);
      //   break;
      // case SUB:
      //   GetOutputArithmeticHelper(output, i, printhelper);
      //   break;
      // case MULT:
      //   GetOutputArithmeticHelper(output, i, printhelper);
      //   break;
      // case DIV:
      //   GetOutputArithmeticHelper(output, i, printhelper);
      //   break;
      // case LESSTHAN:
      //   GetOutputArithmeticHelper(output, i, printhelper);
      //   break;
      // case LESSTHANEQ:
      case LOGNOT:
        output = output + blocks_[i]->target.reg().name()
          + " <- " + printhelper[LOGNOT] + blocks_[i]->arg1.reg().name();
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
      case JEQUAL:
        output = output + printhelper[JEQUAL] + " " +
          blocks_[i]->target.label().name();
        break;
      case JNOTEQUAL:
        output = output + printhelper[JNOTEQUAL] + " " +
          blocks_[i]->target.label().name();
        break;
      case JGREATER:
        output = output + printhelper[JGREATER] + " " +
          blocks_[i]->target.label().name();
        break;
      case JGREATEREQ:
        output = output + printhelper[JGREATEREQ] + " " +
          blocks_[i]->target.label().name();
        break;
      case JLESS:
        output = output + printhelper[JLESS] + " " +
          blocks_[i]->target.label().name();
        break;
      case JLESSEQ:
        output = output + printhelper[JLESSEQ] + " " +
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

// To Do:
void LowererVisitor::VisitFunctionCall(const FunctionCall& call) {
  // Notes:
  // The function call form looks like this
  // VarExpr = funcname(arg1,...,argn) where args are arithmetic exprs
  // So high level wise (didn't read fully so may be missing steps like
  // stack restoration but im p sure that's in the callee)
  // 1) Signal to code gen that there is a function call

  // 2) Now load in arguments at a offset to the stack frame
  // 3) Create the call TAC, e.g call funcname
  // 4) TAC to indicate "pop" off the retval
  // 5) Assignment to the lhs

  // Evaluate the arguments to a single value
  // Do it backwards to make loading into the stack easier
  for (int i = call.arguments().size() - 1 ; i >= 0 ; --i) {
    call.arguments()[i]->Visit(this);
  }

  // At this point the top X things on the stack should be
  // the arguments of the function in the correct order

  // Function should return here get its return value
  // lhs is a variable expr
  call.lhs().Visit(this);

  // Restore the stack (Based on # of args)? gcc doesn't do this
  // (add $8*#args %esp)

  // Basically do an assignment here
  CreateLoadBlock(FUNRETLOAD, Operand(0));
}

void LowererVisitor::VisitFunctionDef(const FunctionDef& def) {
  // Problem! - creating a function is essentially just
  // creating a label + using the stack frame correctly
  // then returning correctly
  // the thing is though the way code_gen's generate works
  // is it will create a function body directly in the middle of
  // a main function

  // Couple of ideas to fix this, have a vector of vectors that store TACS
  // e.g. vector<vector<uni_ptr ThreeAddressCode>>
  // so basically at the end/beg of the first for loop we just have a nested
  // that "creates" the functions at the end/beg

  // In codegen the second you see a FunctionDef TAC,
  // immediately write to a different
  // buffer and concat that to the end
  // or store the index of that TAC, figure out the index of the end of the
  // block and then create them at the end
  // My opinion is that the first idea is probably the easiest.

  // Notes:
  // Form of func def in a "C++ function style is"
  // ArithmeticExpr (Return value) funcname(arg1...argn) {(funcbody)}
  // where arguments are VarExpr

  // High level wise
  // 1) Do you actually need to do anything with the variables?
  // 2) Eval the body
  // -> Codegen here should probably get the stack ready here
  // e.g push %rbp
  // mov %rsp, %rbp

  // then gcc likes to store the arguments from the func inside the stack here
  // Also gcc likes to use these registers in this order
  // arg1 : edi, arg2: esi, arg3: edx, arg4: ecx, arg5:r8d?

  // sub $X,%rsp where X is the size of all variables used in the func
  // gcc does the last statement manually by loading them in a offset
  // as local variables are encountered

  // 3) move the ret value into the correct register
  // gcc uses eax, yales says eax so probably eax or maybe rax for us
  // 4) TAC for ret (probably wont need this)

  // Create a label for the function
  CreateLabelBlock(def.function_name());

  // Create function prologue
  // push %rbp
  // mov %rsp, %rbp
  // CreateFunctionPrologue();

  // Move arguments into the local stack
  for (auto& param : def.parameters()) {
    param->Visit(this);
    // FUNLOAD by popping off the stack
  }

  // Eval the body
  for (auto& statement : def.function_body()) {
    statement->Visit(this);
  }

  // Load the ret value into eax
  def.retval().Visit(this);

  // CreateLoadBlock();

  // Create a returnblock
  // CreateReturnBlock();
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
void LowererVisitor::VisitGreaterThanEqualToExpr(
  const GreaterThanEqualToExpr& exp) {
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

  // Create a cmp to let codegen know it's a branch coming
  CreateComparisionBlock(CONDITIONAL);

  // Jump conditional to the false branch here
  std::string falselabel = JumpLabelHelper();
  CreateJumpBlock(falselabel, JEQUAL);

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

  // Check here if there's an issue with unassigned variables
  if (localsets_[localsets_.size()-1] != localsets_[localsets_.size()-2]) {
    std::cerr << "Unassigned Variable Detected\n";
    exit(1);
  } else {
    // Otherwise we can now safely copy one of the local sets
    // as a globalset
    globalset_ = localsets_[localsets_.size()-1];

    // Delete the last two
    localsets_.pop_back();
    localsets_.pop_back();
  }

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
  CreateJumpBlock(continuelabel, JEQUAL);

  // Save the state of the last variable set
  // Not as tested as the conditional version
  std::set<std::string> originalset_ = globalset_;

  // Do this normally
  for (auto& statement : loop.body()) {
    statement->Visit(this);
  }

  // Not as tested as the conditional version
  if (globalset_ != originalset_) {
    std::cerr << "Unassigned Variable Detected (Loop)\n";
    exit(1);
  }

  // Jump to the loop again
  CreateJumpBlock(looplabel, JUMP);

  // Create a continue label
  CreateLabelBlock(continuelabel);
}

void LowererVisitor::VisitAssignment(const Assignment& assignment) {
  auto newblock = make_unique<struct ThreeAddressCode>();

  // Visit the left which will add its variable name to the stack
  assignment.lhs().Visit(const_cast<LowererVisitor*>(this));

  // assign the right hand side to be equal to the left hand side
  // The latest vector addition is the final register to be set to the
  // string name
  assignment.rhs().Visit(const_cast<LowererVisitor*>(this));

  Operand arg1 = Operand(blocks_[blocks_.size()-1]->target.reg());

  CreateLoadBlock(VARLOAD, arg1);
}

// NEEDS TO BE UPDATED (Updated)
void LowererVisitor::VisitProgram(const Program& program) {
  // Do all the Assignments, then the AE, then the functions

  for (auto& statement : program.statements()) {
      statement->Visit(this);
  }

  program.arithmetic_exp().Visit(this);

  for (auto& def : program.function_defs()) {
      def->Visit(this);
  }
}

void LowererVisitor::VisitVariableExpr(const VariableExpr& exp) {
  // Just get the string stored in VariableExpr and push it to
  // the stack
  variablestack_.push(exp.name());
  lastchildtype_ = VARCHILD;
}

void LowererVisitor::VisitIntegerExpr(const IntegerExpr& exp) {
  // Load value into a target (t <- value)

  Operand arg1 = Operand(exp.value());
  CreateLoadBlock(INTLOAD, arg1);

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

void LowererVisitor::CreateLoadBlock(Type type, Operand arg1) {
  ASSERT(type == INTLOAD || type == VARLOAD || type == FUNLOAD
  || type == FUNRETLOAD, "Must be an Int, Variable, or function load\n");

  auto newblock = make_unique<struct ThreeAddressCode>();
  std::string varname;
  switch (type) {
    case VARLOAD:
      varname = variablestack_.top();
      variablestack_.pop();

      // Push variablename into the set (Flagging it as being assigned)
      globalset_.insert(varname);

      newblock->target = Target(Register(varname, VARIABLEREG));
      newblock->op = Opcode(VARLOAD);
      newblock->arg1 = arg1;

      blocks_.push_back(std::move(newblock));
      break;
    case INTLOAD:
      newblock->arg1 = arg1;
      newblock->op = Opcode(INTLOAD);

      // look at this later,just going to do this now to test some things
      newblock->target = Target(Register("t_" +
        std::to_string(counter_.variablecount), VIRTUALREG));

      // Push into vector
      blocks_.push_back(std::move(newblock));
      break;
    default:
      break;
  }
}

void LowererVisitor::CreateComparisionBlock(Type type) {
  ASSERT(type == CONDITIONAL || type == LOOP,
    "Must be a loop or a conditional type");

  auto newblock = make_unique<struct ThreeAddressCode>();
  newblock->arg2 = Operand(blocks_[blocks_.size()-1]->target.reg());
  // Flip the comparision so it jumps if it's negative
  newblock->arg1 = Operand(0);

  newblock->op = Opcode(type);

  blocks_.push_back(std::move(newblock));
}

void LowererVisitor::CreateLabelBlock(std::string labelname) {
  auto labelblock  = make_unique<struct ThreeAddressCode>();
  labelblock->target = Target(Label(labelname));
  labelblock->op = Opcode(LABEL);
  blocks_.push_back(std::move(labelblock));
}

void LowererVisitor::CreateJumpBlock(std::string jumpname, Type type) {
  // Probably a better way to do this
  ASSERT(type == JUMP || type == JEQUAL || type == JGREATER
    || type == JGREATEREQ || type == JLESS || type == JLESSEQ
    || type == JNOTEQUAL, "Must be a jump type");

  auto jumpblock = make_unique<struct ThreeAddressCode>();
  jumpblock->target = Target(Label(jumpname));
  jumpblock->op = Opcode(type);

  blocks_.push_back(std::move(jumpblock));
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

Register LowererVisitor::GetArgument(ChildType type) {
  Register arg;
  switch (type) {
    case INTCHILD:
      arg = blocks_[blocks_.size()-1]->target.reg();
      break;
    case VARCHILD:
      // Check if variable has been assigned here?
      arg = Register(variablestack_.top(), VARIABLEREG);
      variablestack_.pop();

      if ( globalset_.count(arg.name()) == 0 ) {
        std::cerr << "Variable "+ arg.name() +" not assigned\n";
        exit(1);
      }
      break;
    default:
      // There should be type throw error?
      break;
  }
  return arg;
}

void LowererVisitor::BinaryOperatorHelper(Type type,
  Register arg1, Register arg2) {
  // Load value into target (t <- prev->target + prev->prev->target)
  // Last two elements of the vector should be the integers to load in

  auto newblock = make_unique<struct ThreeAddressCode>();

  // Check for variable non assignment here?
  newblock->arg1 = Operand(arg1);

  if (arg2.name() != "") {
    newblock->arg2 = Operand(arg2);
  }

  // if (type == DIV) {check for div zero?}

  newblock->op = Opcode(type);
  // look at this later, just going to do this now to test some things
  newblock->target = Target(Register("t_" +
    std::to_string(counter_.variablecount), VIRTUALREG));

  // Push into vector
  blocks_.push_back(std::move(newblock));

  ++counter_.variablecount;
  lastchildtype_ = INTCHILD;
}


}  // namespace backend
}  // namespace cs160
