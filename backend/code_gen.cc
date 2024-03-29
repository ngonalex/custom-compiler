#include "backend/code_gen.h"

namespace cs160 {
namespace backend {

void CodeGen::GeneratePrinter() {
  GenerateDivisionByZeroCheck();
  GenerateTupleFlagCheck();
  GenerateIntegerFlagCheck();
  GenerateExistenceCheck();
  GenerateTupleSizeCheck();
  GenerateCreateNewTuple();
  GeneratePrintTuple();
  GeneratePrintIntTuple();
  GeneratePrintNestedTuple();
  GeneratePrintUnassignedTuple();
  GenerateTupleCreationSizeCheck();

  // printdivisionbyzeroerror
  outfile_ << "printdivisionbyzeroerror:" << std::endl;
  outfile_ << "\tmov $printstring, %rdi" << std::endl;
  outfile_ << "\tmov $divisionbyzeroasciz, %rsi" << std::endl;
  outfile_ << "\tmov $0, %rax" << std::endl;
  outfile_ << "\tcall printf" << std::endl;
  outfile_ << "\tcall exit" << std::endl;

  // printtupletypeerror
  outfile_ << "printtupletypeerror:" << std::endl;
  outfile_ << "\tmov $printstring, %rdi" << std::endl;
  outfile_ << "\tmov $tupletypeasciz, %rsi" << std::endl;
  outfile_ << "\tmov $0, %rax" << std::endl;
  outfile_ << "\tcall printf" << std::endl;
  outfile_ << "\tcall exit" << std::endl;

  // printintegertypeerror
  outfile_ << "printintegertypeerror:" << std::endl;
  outfile_ << "\tmov $printstring, %rdi" << std::endl;
  outfile_ << "\tmov $inttypeasciz, %rsi" << std::endl;
  outfile_ << "\tmov $0, %rax" << std::endl;
  outfile_ << "\tcall printf" << std::endl;
  outfile_ << "\tcall exit" << std::endl;

  // printexistenceerror
  outfile_ << "printexistenceerror:" << std::endl;
  outfile_ << "\tmov $printstring, %rdi" << std::endl;
  outfile_ << "\tmov $existenceerrorasciz, %rsi" << std::endl;
  outfile_ << "\tmov $0, %rax" << std::endl;
  outfile_ << "\tcall printf" << std::endl;
  outfile_ << "\tcall exit" << std::endl;

  // printsizeerror (change this later to print out the size of the tuple
  // and the index that tried to be access)
  outfile_ << "printsizeerror:" << std::endl;
  outfile_ << "\tmov $printstring, %rdi" << std::endl;
  outfile_ << "\tmov $outofboundserrorasciz, %rsi" << std::endl;
  outfile_ << "\tmov $0, %rax" << std::endl;
  outfile_ << "\tcall printf" << std::endl;
  outfile_ << "\tcall exit" << std::endl;

  // printsizeerror
  outfile_ << "printtuplecreationsizeerror:" << std::endl;
  outfile_ << "\tmov $printstring, %rdi" << std::endl;
  outfile_ << "\tmov $tuplesizeerrorascii, %rsi" << std::endl;
  outfile_ << "\tmov $0, %rax" << std::endl;
  outfile_ << "\tcall printf" << std::endl;
  outfile_ << "\tcall exit" << std::endl;

  // negative:
  outfile_ << "negative:" << std::endl;
  outfile_ << "\t.asciz \"-\"" << std::endl;

  // printresult
  outfile_ << "printresult:" << std::endl;
  outfile_ << "\t.asciz \"The program returned: \"" << std::endl;

  // printtupleascii
  outfile_ << "printtupleascii:" << std::endl;
  outfile_ << "\t.asciz \"The program returned a "
           << "tuple with indices:\\n\"" << std::endl;

  // printunassignedtupleascii
  outfile_ << "printunassignedtupleascii:" << std::endl;
  outfile_ << "\t.asciz \": Unassigned\\n\"" << std::endl;

  // printinttupleascii
  outfile_ << "printinttupleascii:" << std::endl;
  outfile_ << "\t.asciz \": Integer with value: \"" << std::endl;

  // printnestedtupleascii
  outfile_ << "printnestedtupleascii:" << std::endl;
  outfile_ << "\t.asciz \": Tuple with size: \"" << std::endl;

  // printfunctionresult
  outfile_ << "printfunctionresult:" << std::endl;
  outfile_ << "\t.asciz \"The function returned: \"" << std::endl;

  // printnewline
  outfile_ << "printnewline:" << std::endl;
  outfile_ << "\t.asciz \"\\n\"" << std::endl;

  // printstring
  outfile_ << "printstring:" << std::endl;
  outfile_ << "\t.asciz \"%s\"" << std::endl;

  // printint
  outfile_ << "printint:" << std::endl;
  outfile_ << "\t.asciz \"%d\\n\"" << std::endl;

  // printintnonewline
  outfile_ << "printintnonewline:" << std::endl;
  outfile_ << "\t.asciz \"%d\"" << std::endl;

  // printstringint
  outfile_ << "printstringint:" << std::endl;
  outfile_ << "\t.asciz \"%s%d\\n\"" << std::endl;

  // existenceerrorasciz
  outfile_ << "existenceerrorasciz:" << std::endl;
  outfile_ << "\t.asciz \"Value does not exist in the tuple\\n\"" << std::endl;

  // tupletypeascii
  outfile_ << "tupletypeasciz:" << std::endl;
  outfile_ << "\t.asciz \"Invalid type: must be a tuple\\n\"" << std::endl;

  // tuplesizeerrorascii
  outfile_ << "tuplesizeerrorascii:" << std::endl;
  outfile_ << "\t.asciz \"Invalid size, tuple size "
           << "must be greater than zero\\n\"" << std::endl;

  // inttypeascii
  outfile_ << "inttypeasciz:" << std::endl;
  outfile_ << "\t.asciz \"Invalid type: must be an int\\n\"" << std::endl;

  // Divisionbyzeroascii
  outfile_ << "\tdivisionbyzeroasciz:" << std::endl;
  outfile_ << "\t.asciz \"Error: Division by zero\\n\"" << std::endl;

  // outofboundserrorascii
  outfile_ << "outofboundserrorasciz:" << std::endl;
  outfile_ << "\t.asciz \"Out of Bounds Error\\n\"" << std::endl;

  // Generate Unique Printers here
  std::set<std::string>::iterator it;
  for (it = assignmentset_.begin(); it != assignmentset_.end(); ++it) {
    GeneratePrintAssignment(*it);
  }
}

void CodeGen::GeneratePrintCall(std::string label) {
  outfile_ << "\t# Calling printf" << std::endl;
  outfile_ << "\tpush %rax" << std::endl;
  outfile_ << "\tmov $" << label << ", %rdi" << std::endl;
  outfile_ << "\tmov $0, %rax" << std::endl;
  outfile_ << "\tcall printf" << std::endl;

  outfile_ << "\tpop %rax" << std::endl;
  outfile_ << "\tmov $printint, %rdi" << std::endl;
  outfile_ << "\tmov %rax, %rsi" << std::endl;
  outfile_ << "\tmov $0, %rax" << std::endl;
  outfile_ << "\tcall printf\n" << std::endl;
}

void CodeGen::TestPrint(std::string label) {
  outfile_ << "\t# Calling printf" << std::endl;
  outfile_ << "\tpop %rax" << std::endl;
  outfile_ << "\tpush %rax" << std::endl;
  outfile_ << "\tmov $" << label << ", %rdi" << std::endl;
  outfile_ << "\tmov $0, %rax" << std::endl;
  outfile_ << "\tcall printf" << std::endl;

  outfile_ << "\tpop %rax" << std::endl;
  outfile_ << "\tmov $printint, %rdi" << std::endl;
  outfile_ << "\tmov %rax, %rsi" << std::endl;
  outfile_ << "\tmov $0, %rax" << std::endl;
  outfile_ << "\tcall printf\n" << std::endl;
}

void CodeGen::GeneratePrintAssignment(std::string variablename) {
  outfile_ << variablename + "ascii:" << std::endl;
  outfile_ << "\t.asciz \"Variable " + variablename + " is equal to: \""
           << std::endl;
}

// This needs to change
void CodeGen::GenerateData(std::set<std::string> variableset) {
  // Creates a .data section for variables
  outfile_ << ".data " << std::endl;
  outfile_ << "\theap:\n\t\t.zero 100000" << std::endl;
  outfile_ << "\treturnobj:\n\t\t.zero 16" << std::endl;
  outfile_ << "\tbumpptr:\n\t\t.zero 8" << std::endl;
  for (auto iter = variableset.begin(); iter != variableset.end(); ++iter) {
    outfile_ << "\t" << *iter << ":\n\t\t.zero 16" << std::endl;
  }
}

void CodeGen::GenerateEpilogue() { outfile_ << "\tcall exit\n"; }

void CodeGen::GenerateBoiler() {
  outfile_ << "\t.global main" << std::endl;
  outfile_ << "\t.text" << std::endl;
  outfile_ << "main:" << std::endl;
}

void CodeGen::ClearRegister(std::string reg) {
  outfile_ << "\txor %" + reg + ", %" + reg << std::endl;
}

void CodeGen::GenerateLoadInstructions(std::unique_ptr<ThreeAddressCode> tac) {
  OpcodeType loadtype = tac->op.opcode();
  int argumentnum;
  std::string varname;
  std::vector<std::string> parsedstring;

  switch (loadtype) {
    case INT_LOAD:  // 2 pushes
      outfile_ << "\t# Loading in an integer" << std::endl;
      outfile_ << "\tmov $" + std::to_string(tac->arg1.value()) + ", %rax"
               << std::endl;
      outfile_ << "\tmov $0x100, %rbx" << std::endl;
      outfile_ << "\tpush %rbx" << std::endl;
      outfile_ << "\tpush %rax\n" << std::endl;
      break;
    case VAR_LOAD:  // 2 pushes
      outfile_ << "\t# Loading from " << tac->arg1.reg().name() << std::endl;
      outfile_ << "\tmov "
               << VariableNameHelper(tac->arg1.reg().name(), NO_FLAG)
               << ", %rbx" << std::endl;

      // Then load it into register
      outfile_ << "\tpush " << FlagHelper() << std::endl;
      outfile_ << "\tmov "
               << VariableNameHelper(tac->arg1.reg().name(), OBJECT_FLAG)
               << ", %rbx" << std::endl;
      outfile_ << "\tpush %rbx\n" << std::endl;
      break;
    case VAR_ASSIGN_LOAD:  // Always pop 3 things

      // Two cases one of it it's a deref, other if it's a var
      // Only add to map if it's a var
      // Case down below is for vars
      parsedstring = DereferenceParserHelper(tac->target.reg().name());

      if (parsedstring.size() == 1) {
        if (symbollocations_.count(tac->target.reg().name()) == 0) {
          // Add it to the map if its new
          symbollocations_.insert(std::pair<std::string, int>(
              tac->target.reg().name(),
              -16 + -16 * (symbollocations_.size() + 1)));
        }

        outfile_ << "\t# Loading a value into variable " +
                        tac->target.reg().name()
                 << std::endl;
        ClearRegister("rcx");
        outfile_ << "\tpop %rcx" << std::endl;  // VALUE
        outfile_ << "\tpop %rax" << std::endl;  // This is the FLAG

        if (currscope_ == GLOBAL) {
          outfile_ << "\tmov "
                   << VariableNameHelper(tac->target.reg().name(), NO_FLAG)
                   << ", %rbx" << std::endl;
        }

        outfile_ << "\tmov %rax,"
                 << VariableNameHelper(tac->target.reg().name(), TYPE_FLAG)
                 << std::endl;
        outfile_ << "\tmovq %rcx, "
                 << VariableNameHelper(tac->target.reg().name(), OBJECT_FLAG)
                 << "\n"
                 << std::endl;

        // Add it to the set, then call the print function
        // Needs to be changed
        if (currscope_ == GLOBAL) {
          assignmentset_.insert(tac->target.reg().name());

          if (flag_ == PRINT_DEBUG || flag_ == PRINT_PROGRAM) {
            outfile_ << "\tmov %rcx, %rax\n" << std::endl;
            // Call on correct print function
            outfile_ << "\t# Going to print " << tac->target.reg().name()
                     << std::endl;
            GeneratePrintCall(tac->target.reg().name() + "ascii");
            outfile_ << "\t# Returning from printing "
                     << tac->target.reg().name() << std::endl;
            outfile_ << std::endl;
          }
        }
      } else {
        // Handle Derefs
        outfile_ << "\t# Loading an int into " << tac->target.reg().name()
                 << std::endl;
        // Rewrite flags
        // RCX is the RHS value to be assigned
        // RAX is the flag of RHS
        // RBX is the address
        outfile_ << "\tpop %rcx" << std::endl;
        outfile_ << "\tpop %rax" << std::endl;
        outfile_ << "\tpop %rbx" << std::endl;
        // Copy flags into
        outfile_ << "\tmovq %rax, (%rbx)" << std::endl;
        outfile_ << "\tmovq %rcx, 8(%rbx)\n" << std::endl;
      }
      break;
    case FUN_ARG_LOAD:
      // Here we're moving arguments loaded into the stack before the
      // function call and moving it to the local stack
      argumentnum = tac->arg1.value() + 1;
      varname = tac->target.reg().name();
      outfile_ << "\t# Moving argument " << std::to_string(argumentnum)
               << " into the stack" << std::endl;
      outfile_ << "\tmov " << std::to_string(16 * argumentnum) << "(%rbp), %rax"
               << std::endl;
      outfile_ << "\tmov %rax, " << std::to_string(-8 + argumentnum * -16)
               << "(%rbp)" << std::endl;
      // Remake the objects flags
      outfile_ << "\tmov " << std::to_string(16 * argumentnum + 8)
               << "(%rbp), %rax" << std::endl;
      outfile_ << "\tmov %rax, " << std::to_string(-16 + argumentnum * -16)
               << "(%rbp)\n"
               << std::endl;
      // Include it in the symbol table
      if (symbollocations_.count(varname) == 0) {
        symbollocations_.insert(
            std::pair<std::string, int>(varname, -16 + -16 * argumentnum));
      } else {
        std::cerr << "FUN_ARG_LOAD VARIABLE ASSIGNMENT PROBLEM\n";
        exit(1);
      }
      break;
    case FUN_RET_LOAD:
      // All we do here is move the value from the function to the correct
      // variable
      if (symbollocations_.count(tac->target.reg().name()) == 0) {
        // Add it to the map then create a spot for it (if its a function)
        symbollocations_.insert(std::pair<std::string, int>(
            tac->target.reg().name(),
            -16 + -16 * (symbollocations_.size() + 1)));
      }
      outfile_ << "\t# Returning from function and loading value into "
               << tac->target.reg().name() << std::endl;
      outfile_ << "\tmov (%rax), %rcx" << std::endl;

      if (currscope_ == GLOBAL) {
        outfile_ << "\tmov "
                 << VariableNameHelper(tac->target.reg().name(), NO_FLAG)
                 << ", %rbx" << std::endl;
      }

      // Update the whole structure with the correct type
      outfile_ << "\tmov %rcx,"
               << VariableNameHelper(tac->target.reg().name(), TYPE_FLAG)
               << std::endl;
      // now load the value of rax in
      outfile_ << "\tmov 8(%rax), %rax" << std::endl;
      outfile_ << "\tmovq %rax, "
               << VariableNameHelper(tac->target.reg().name(), OBJECT_FLAG)
               << "\n"
               << std::endl;
      break;
    default:
      break;
  }
}

void CodeGen::GenerateArithmeticExpr(std::unique_ptr<ThreeAddressCode> tac,
                                     OpcodeType type) {
  switch (type) {
    case ADD:
      outfile_ << "\t# Addition\n";
      GenerateBinaryExprHelper(std::move(tac));
      ClearRegister("rcx");
      outfile_ << "\tadd %rax, %rcx\n\tadd %rbx, %rcx" << std::endl;
      outfile_ << "\tpush %rdx" << std::endl;
      outfile_ << "\tpush %rcx\n" << std::endl;
      break;
    case SUB:
      outfile_ << "\t# Subtraction\n";
      GenerateBinaryExprHelper(std::move(tac));
      outfile_ << "\tsub %rbx, %rax" << std::endl;
      outfile_ << "\tpush %rcx" << std::endl;
      outfile_ << "\tpush %rax\n" << std::endl;
      break;
    case MULT:
      outfile_ << "\t# Multiplication\n";
      GenerateBinaryExprHelper(std::move(tac));
      outfile_ << "\timul %rax, %rbx" << std::endl;
      outfile_ << "\tpush %rcx" << std::endl;
      outfile_ << "\tpush %rbx\n" << std::endl;
      break;
    case DIV:
      ClearRegister("rdx");
      outfile_ << "\t# Division\n";
      GenerateBinaryExprHelper(std::move(tac));
      // Check for division by zero
      outfile_ << "\tmov %rbx, %rdi" << std::endl;
      outfile_ << "\tcall divisionbyzerocheck" << std::endl;
      outfile_ << "\tcqto" << std::endl;  // indicating its a signed division
      outfile_ << "\tidiv %rbx" << std::endl;
      outfile_ << "\tpush %rcx" << std::endl;
      outfile_ << "\tpush %rax\n" << std::endl;
      break;
    default:
      std::cerr << "Inside GenerateArithmeticExpr, something went"
                << "went very wrong\n";
      exit(1);
  }
}

void CodeGen::GenerateRelationalExpr(std::unique_ptr<ThreeAddressCode> tac,
                                     OpcodeType type) {
  // Note to self you can abstract this out even more
  switch (type) {
    case LESS_THAN:
      outfile_ << "\t# LESS_THAN Comparision\n";
      GenerateBinaryExprHelper(std::move(tac));
      outfile_ << "\tpush %rdx\n" << std::endl;
      outfile_ << "\tcmp %rbx, %rax" << std::endl;
      outfile_ << "\tsetl %dl" << std::endl;
      outfile_ << "\tmovzx %dl, %rcx" << std::endl;
      outfile_ << "\tpush %rcx\n" << std::endl;
      break;
    case LESS_THAN_EQ:
      outfile_ << "\t# LESS_THAN_EQ Comparision\n";
      GenerateBinaryExprHelper(std::move(tac));
      outfile_ << "\tpush %rdx\n" << std::endl;
      outfile_ << "\tcmp %rbx, %rax" << std::endl;
      outfile_ << "\tsetle %dl" << std::endl;
      outfile_ << "\tmovzx %dl, %rcx" << std::endl;
      outfile_ << "\tpush %rcx\n" << std::endl;
      break;
    case GREATER_THAN:
      outfile_ << "\t# GREATER_THAN Comparision\n";
      GenerateBinaryExprHelper(std::move(tac));
      outfile_ << "\tpush %rdx\n" << std::endl;
      outfile_ << "\tcmp %rbx, %rax" << std::endl;
      outfile_ << "\tsetg %dl" << std::endl;
      outfile_ << "\tmovzx %dl, %rcx" << std::endl;
      outfile_ << "\tpush %rcx\n" << std::endl;
      break;
    case GREATER_THAN_EQ:
      outfile_ << "\t# GREATER_THAN_EQ Comparision\n";
      GenerateBinaryExprHelper(std::move(tac));
      outfile_ << "\tpush %rdx\n" << std::endl;
      outfile_ << "\tcmp %rbx, %rax" << std::endl;
      outfile_ << "\tsetge %dl" << std::endl;
      outfile_ << "\tmovzx %dl, %rcx" << std::endl;
      outfile_ << "\tpush %rcx\n" << std::endl;
      break;
    case EQUAL:
      outfile_ << "\t# Equals Comparision\n";
      GenerateBinaryExprHelper(std::move(tac));
      outfile_ << "\tpush %rdx\n" << std::endl;
      outfile_ << "\tcmp %rbx, %rax" << std::endl;
      outfile_ << "\tsete %dl" << std::endl;
      outfile_ << "\tmovzx %dl, %rcx" << std::endl;
      outfile_ << "\tpush %rcx\n" << std::endl;
      break;
    default:
      std::cerr << "Inside GenerateRelationalExpr, something went"
                << "went very wrong\n";
      exit(1);
  }
}

void CodeGen::GenerateLogicalExpr(std::unique_ptr<ThreeAddressCode> tac,
                                  OpcodeType type) {
  switch (type) {
    case LOG_AND:
      outfile_ << "\t# LogicalAnd\n";
      GenerateBinaryExprHelper(std::move(tac));
      outfile_ << "\tand %rbx, %rax" << std::endl;
      outfile_ << "\tpush %rdx\n" << std::endl;
      outfile_ << "\tpush %rax\n" << std::endl;
      break;
    case LOG_OR:
      outfile_ << "\t# LogicalOr\n";
      GenerateBinaryExprHelper(std::move(tac));
      outfile_ << "\tor %rbx, %rax" << std::endl;
      outfile_ << "\tpush %rdx\n" << std::endl;
      outfile_ << "\tpush %rax\n" << std::endl;
      break;
    case LOG_NOT:
      outfile_ << "\t# LogicalNot\n";
      outfile_ << "\tpop %rbx" << std::endl;
      outfile_ << "\tpop %rdx" << std::endl;
      outfile_ << "\txor $1, %rbx" << std::endl;
      outfile_ << "\tpush %rdx\n" << std::endl;
      outfile_ << "\tpush %rbx\n" << std::endl;
      break;
    default:
      std::cerr << "Inside GenerateLogicalExpr, something went"
                << "went very wrong\n";
      exit(1);
  }
}

void CodeGen::GenerateBinaryExprHelper(std::unique_ptr<ThreeAddressCode> tac) {
  outfile_ << "\tpop %r8" << std::endl;   // rbx = right value
  outfile_ << "\tpop %r9" << std::endl;   // rcx = right flag
  outfile_ << "\tpop %r10" << std::endl;  // rax = left value
  outfile_ << "\tpop %r11" << std::endl;  // rdx = left flag

  outfile_ << "\tmovzbl %r9b, %rdi" << std::endl;

  // Error Handling here
  // Both things MUST be an integer as our language
  // does not support "tuple addition"
  outfile_ << "\tcall integerflagcheck" << std::endl;

  outfile_ << "\tmovzbl %r11b, %rdi" << std::endl;
  outfile_ << "\tcall integerflagcheck" << std::endl;
  outfile_ << "\tmov %r8, %rbx" << std::endl;   // rbx = right value
  outfile_ << "\tmov %r9, %rcx" << std::endl;   // rcx = right flag
  outfile_ << "\tmov %r10, %rax" << std::endl;  // rax = left value
  outfile_ << "\tmov %r11, %rdx" << std::endl;  // rdx = left flag
}

std::string CodeGen::FlagHelper() {
  if (currscope_ == GLOBAL) {
    return "(%rbx)";
  } else {
    return "%rbx";
  }
}

std::string CodeGen::VariableNameHelper(std::string variablename,
                                        FlagType flag) {
  std::string mappedname;
  if (currscope_ == GLOBAL) {
    switch (flag) {
      case TYPE_FLAG:
        return "(%rbx)";
        break;
      case EXISTENCE_FLAG:
        return "1(%rbx)";
        break;
      case SIZE_FLAG:
        return "2(%rbx)";
        break;
      case OBJECT_FLAG:
        return "8(%rbx)";
        break;
      default:
        return "$" + variablename;
        break;
    }
  } else {
    // We're inside a function
    // Check if it exists first
    if (symbollocations_.count(variablename) != 1) {
      // something bad happened
      std::cerr << "Variable name helper could not find the mapping for "
                << variablename << " \n";
      exit(1);
    }
    int index = symbollocations_.find(variablename)->second;
    switch (flag) {
      case TYPE_FLAG:
        mappedname = std::to_string(index) + "(%rbp)";
        return mappedname;
        break;
      case EXISTENCE_FLAG:
        mappedname = std::to_string(index + 1) + "(%rbp)";
        return mappedname;
        break;
      case SIZE_FLAG:
        mappedname = std::to_string(index + 2) + "(%rbp)";
        return mappedname;
        break;
      case OBJECT_FLAG:
        mappedname = std::to_string(index + 8) + "(%rbp)";
        return mappedname;
        break;
      default:
        mappedname = std::to_string(index) + "(%rbp)";
        return mappedname;
        break;
    }
  }
}

std::vector<std::string> CodeGen::DereferenceParserHelper(
    std::string variablename) {
  std::istringstream issvarname(variablename);
  std::string token;
  std::string result = "";
  std::vector<std::string> resultvector;

  while (std::getline(issvarname, token, '-')) {
    result = result + token;
  }

  issvarname = std::istringstream(result);
  result = "";

  while (std::getline(issvarname, token, '>')) {
    resultvector.push_back(token);
  }
  return resultvector;
}

void CodeGen::GenerateCreateNewTuple() {
  outfile_ << "newtuple:" << std::endl;
  outfile_ << "\tmov $heap, %rbx" << std::endl;
  outfile_ << "\tmov $bumpptr, %rcx" << std::endl;
  outfile_ << "\tmov (%rcx), %rdx" << std::endl;

  outfile_ << "\tadd %rdx, %rbx" << std::endl;
  outfile_ << "\tadd %rdi, %rdx" << std::endl;
  outfile_ << "\tmov %rdx, (%rcx)" << std::endl;
  outfile_ << "\tmov %rbx, %rax" << std::endl;
  outfile_ << "\tret" << std::endl;
}

void CodeGen::GeneratePrintTuple() {
  // rdi is the address
  // rsi is the flags
  outfile_ << "printtuple:" << std::endl;
  outfile_ << "\tpush %rdi" << std::endl;
  outfile_ << "\tpush %rsi" << std::endl;

  outfile_ << "\tmov $printstring, %rdi" << std::endl;
  outfile_ << "\tmov $printtupleascii, %rsi" << std::endl;
  outfile_ << "\tmov $0, %rax" << std::endl;
  outfile_ << "\tcall printf" << std::endl;

  outfile_ << "\tpop %rsi" << std::endl;
  outfile_ << "\tpop %rdi" << std::endl;
  // comment this line out alter to finish the subroutine
  // GenerateEpilogue();

  outfile_ << "\tshr $16, %rsi" << std::endl;
  outfile_ << "\txor %rdx, %rdx" << std::endl;
  outfile_ << "\tcall printtupleloopguard" << std::endl;
  outfile_ << "\tret" << std::endl;

  outfile_ << "printtupleloopguard:" << std::endl;
  outfile_ << "\tcmp %rsi, %rdx" << std::endl;
  outfile_ << "\tjl printtupleloop" << std::endl;
  outfile_ << "\tret" << std::endl;

  outfile_ << "printtupleloop:" << std::endl;
  outfile_ << "\tmov %rdx, %r8" << std::endl;
  outfile_ << "\tmov %rdi, %r9" << std::endl;
  outfile_ << "\timul $16, %r8" << std::endl;
  outfile_ << "\tadd %r8, %r9" << std::endl;
  outfile_ << "\tmov %r9, %r10" << std::endl;
  outfile_ << "\tmovb 1(%r10), %r11b" << std::endl;
  outfile_ << "\tcmp $0, %r11b" << std::endl;
  outfile_ << "\tje unassignedtuple" << std::endl;

  outfile_ << "\tmov (%r10), %r11b" << std::endl;
  outfile_ << "\tcmp $0, %r11b" << std::endl;
  outfile_ << "\tje inttuple" << std::endl;
  outfile_ << "\tjmp nestedtuple" << std::endl;
}

void CodeGen::GeneratePrintIntTuple() {
  outfile_ << "inttuple:" << std::endl;
  outfile_ << "\tpush %rdi" << std::endl;
  outfile_ << "\tpush %rsi" << std::endl;
  outfile_ << "\tpush %rdx" << std::endl;
  outfile_ << "\tpush %r10" << std::endl;

  outfile_ << "\tmov $printintnonewline, %rdi" << std::endl;
  outfile_ << "\tmov %rdx, %rsi" << std::endl;
  outfile_ << "\tadd $1, %rsi" << std::endl;
  outfile_ << "\tmov $0, %rax" << std::endl;
  outfile_ << "\tcall printf" << std::endl;

  outfile_ << "\tmov $printstring, %rdi" << std::endl;
  outfile_ << "\tmov $printinttupleascii, %rsi" << std::endl;
  outfile_ << "\tmov $0, %rax" << std::endl;
  outfile_ << "\tcall printf" << std::endl;

  outfile_ << "\tpop %r10" << std::endl;
  outfile_ << "\tmovl 8(%r10), %esi" << std::endl;
  outfile_ << "\tmov $printint, %rdi" << std::endl;
  outfile_ << "\tmov $0, %rax" << std::endl;
  outfile_ << "\tcall printf" << std::endl;

  outfile_ << "\tpop %rdx" << std::endl;
  outfile_ << "\tpop %rsi" << std::endl;
  outfile_ << "\tpop %rdi" << std::endl;
  outfile_ << "\tadd $1, %rdx" << std::endl;
  outfile_ << "\tjmp printtupleloopguard" << std::endl;
}
void CodeGen::GeneratePrintNestedTuple() {
  outfile_ << "nestedtuple:" << std::endl;
  outfile_ << "\tpush %rdi" << std::endl;
  outfile_ << "\tpush %rsi" << std::endl;
  outfile_ << "\tpush %rdx" << std::endl;
  outfile_ << "\tpush %r10" << std::endl;

  outfile_ << "\tmov $printintnonewline, %rdi" << std::endl;
  outfile_ << "\tmov %rdx, %rsi" << std::endl;
  outfile_ << "\tadd $1, %rsi" << std::endl;
  outfile_ << "\tmov $0, %rax" << std::endl;
  outfile_ << "\tcall printf" << std::endl;

  outfile_ << "\tmov $printstring, %rdi" << std::endl;
  outfile_ << "\tmov $printnestedtupleascii, %rsi" << std::endl;
  outfile_ << "\tmov $0, %rax" << std::endl;
  outfile_ << "\tcall printf" << std::endl;

  outfile_ << "\tpop %r10" << std::endl;
  outfile_ << "\tmovl 2(%r10), %esi" << std::endl;
  outfile_ << "\tmov $printint, %rdi" << std::endl;
  outfile_ << "\tmov $0, %rax" << std::endl;
  outfile_ << "\tcall printf" << std::endl;

  outfile_ << "\tpop %rdx" << std::endl;
  outfile_ << "\tpop %rsi" << std::endl;
  outfile_ << "\tpop %rdi" << std::endl;
  outfile_ << "\tadd $1, %rdx" << std::endl;
  outfile_ << "\tjmp printtupleloopguard" << std::endl;
}
void CodeGen::GeneratePrintUnassignedTuple() {
  outfile_ << "unassignedtuple:" << std::endl;
  outfile_ << "\tpush %rdi" << std::endl;
  outfile_ << "\tpush %rsi" << std::endl;
  outfile_ << "\tpush %rdx" << std::endl;

  outfile_ << "\tmov $printintnonewline, %rdi" << std::endl;
  outfile_ << "\tmov %rdx, %rsi" << std::endl;
  outfile_ << "\tadd $1, %rsi" << std::endl;
  outfile_ << "\tmov $0, %rax" << std::endl;
  outfile_ << "\tcall printf" << std::endl;

  outfile_ << "\tmov $printstring, %rdi" << std::endl;
  outfile_ << "\tmov $printunassignedtupleascii, %rsi" << std::endl;
  outfile_ << "\tmov $0, %rax" << std::endl;
  outfile_ << "\tcall printf" << std::endl;
  outfile_ << "\tpop %rdx" << std::endl;
  outfile_ << "\tpop %rsi" << std::endl;
  outfile_ << "\tpop %rdi" << std::endl;
  outfile_ << "\tadd $1, %rdx" << std::endl;
  outfile_ << "\tjmp printtupleloopguard" << std::endl;
}

void CodeGen::GenerateDivisionByZeroCheck() {
  // rdi contains the address of what to check
  outfile_ << "divisionbyzerocheck:" << std::endl;
  outfile_ << "\tcmp $0, %rdi" << std::endl;
  outfile_ << "\tje printdivisionbyzeroerror" << std::endl;
  outfile_ << "\tret" << std::endl;
}

void CodeGen::GenerateTupleFlagCheck() {
  // rdi contains the address of what to check
  outfile_ << "tupleflagcheck:" << std::endl;
  outfile_ << "\tcmp $1, %rdi" << std::endl;
  outfile_ << "\tjne printtupletypeerror" << std::endl;
  outfile_ << "\tret" << std::endl;
}
void CodeGen::GenerateIntegerFlagCheck() {
  // rdi contains the address of what to check
  outfile_ << "integerflagcheck:" << std::endl;
  outfile_ << "\tcmp $0, %rdi" << std::endl;
  outfile_ << "\tjne printintegertypeerror" << std::endl;
  outfile_ << "\tret" << std::endl;
}
void CodeGen::GenerateExistenceCheck() {
  // rdi contains the address of what to check
  outfile_ << "existencecheck:" << std::endl;
  outfile_ << "\tcmp $1, %rdi" << std::endl;
  outfile_ << "\tjne printexistenceerror" << std::endl;
  outfile_ << "\tret" << std::endl;
}
void CodeGen::GenerateTupleCreationSizeCheck() {
  outfile_ << "tuplecreationsizecheck:" << std::endl;
  outfile_ << "cmp $0, %rdi" << std::endl;
  outfile_ << "jle printtuplecreationsizeerror" << std::endl;
  outfile_ << "\tret" << std::endl;
}
void CodeGen::GenerateTupleSizeCheck() {
  // rdi contains the address of what to check
  // rsi contains the integer of the size of the rhs
  // basically this is just a tuple out of bounds check
  outfile_ << "tuplesizecheck:" << std::endl;
  // Move arguments into registers so we can compare them
  outfile_ << "\tmovl %edi, %eax" << std::endl;
  outfile_ << "\tmovl %esi, %ebx" << std::endl;

  // Check for greater than size of tuple
  outfile_ << "\tcmp %eax, %ebx" << std::endl;
  outfile_ << "\tsetle %dl" << std::endl;
  outfile_ << "\tmovzx %dl, %rcx" << std::endl;
  outfile_ << "\tcmp $0, %rcx" << std::endl;
  outfile_ << "\tje printsizeerror" << std::endl;

  // Check for less than 1
  outfile_ << "\tcmp $1, %ebx" << std::endl;
  outfile_ << "\tsetge %dl" << std::endl;
  outfile_ << "\tmovzx %dl, %rcx" << std::endl;
  outfile_ << "\tcmp $0, %rcx" << std::endl;
  outfile_ << "\tje printsizeerror" << std::endl;
  outfile_ << "\tret" << std::endl;
}

void CodeGen::GenerateNestedDeref() {
  outfile_ << "\tpop %rcx" << std::endl;  // value
  outfile_ << "\tpop %rax" << std::endl;  // flags
  outfile_ << "\tpop %rbx" << std::endl;  // address
  outfile_ << "\tpush %rcx" << std::endl;
  // Error Handling here to check the size + if you're actually an tuple
  outfile_ << "\t#Error Handling Here DerefChild" << std::endl;
  outfile_ << "\tmovb 1(%rbx), %al" << std::endl;
  outfile_ << "\tmovzx %al, %rdi" << std::endl;
  outfile_ << "\tpush %rbx" << std::endl;
  outfile_ << "\tcall existencecheck" << std::endl;
  outfile_ << "\tpop %rbx" << std::endl;

  outfile_ << "\tmovb (%rbx), %al" << std::endl;
  outfile_ << "\tmovzx %al, %rdi" << std::endl;
  outfile_ << "\tpush %rbx" << std::endl;
  outfile_ << "\tcall tupleflagcheck" << std::endl;
  outfile_ << "\tpop %rbx\n" << std::endl;

  outfile_ << "\tmovl 2(%rbx), %eax" << std::endl;
  outfile_ << "\tmovslq %eax, %rdi" << std::endl;
  outfile_ << "\tpop %rsi" << std::endl;
  outfile_ << "\tpush %rsi" << std::endl;
  outfile_ << "\tpush %rbx" << std::endl;
  outfile_ << "\tcall tuplesizecheck" << std::endl;
  outfile_ << "\tpop %rbx" << std::endl;

  // Get the actual object (x->whatever)
  outfile_ << "\tmovq 8(%rbx), %rbx" << std::endl;

  // Get the correct offset
  outfile_ << "\tpop %rax" << std::endl;
  outfile_ << "\tsub $1, %rax" << std::endl;
  outfile_ << "\timul $16, %rax" << std::endl;
  outfile_ << "\tadd %rax, %rbx" << std::endl;
  outfile_ << "\tpush %rbx" << std::endl;
}

void CodeGen::GenerateBaseDeref(std::string variablename) {
  outfile_ << "\tmovb " << VariableNameHelper(variablename, EXISTENCE_FLAG)
           << ", %al" << std::endl;
  outfile_ << "\tmovzx %al, %rdi" << std::endl;
  outfile_ << "\tpush %rbx" << std::endl;
  outfile_ << "\tcall existencecheck" << std::endl;
  outfile_ << "\tpop %rbx" << std::endl;

  outfile_ << "\tmovb " << VariableNameHelper(variablename, TYPE_FLAG)
           << ", %al" << std::endl;
  outfile_ << "\tmovzx %al, %rdi" << std::endl;
  outfile_ << "\tpush %rbx" << std::endl;
  outfile_ << "\tcall tupleflagcheck" << std::endl;
  outfile_ << "\tpop %rbx\n" << std::endl;

  outfile_ << "\tmovl " << VariableNameHelper(variablename, SIZE_FLAG)
           << ", %eax" << std::endl;
  outfile_ << "\tmovslq %eax, %rdi" << std::endl;
  outfile_ << "\tpop %rsi" << std::endl;
  outfile_ << "\tpush %rsi" << std::endl;
  outfile_ << "\tpush %rbx" << std::endl;
  outfile_ << "\tcall tuplesizecheck" << std::endl;
  outfile_ << "\tpop %rbx" << std::endl;

  // Get the actual object
  outfile_ << "\tmovq " << VariableNameHelper(variablename, OBJECT_FLAG)
           << ", %rbx" << std::endl;

  // Get the correct index of the object
  outfile_ << "\tpop %rax" << std::endl;
  outfile_ << "\tpop %rcx" << std::endl;
  outfile_ << "\tsub $1, %rax" << std::endl;
  outfile_ << "\timul $16, %rax" << std::endl;
  outfile_ << "\tadd %rax, %rbx" << std::endl;
  outfile_ << "\tpush %rbx\n" << std::endl;
}

void CodeGen::GenerateRHSDerefEpilogue(std::string arg2) {
  if (arg2.compare("Parent") == 0) {
    // If you're a parent check that what you're
    // accessing is actually an integer
    // get the value stored and push it on the stack
    outfile_ << "\tpop %rbx" << std::endl;
    // Get the correct offset
    outfile_ << "\tpush (%rbx)" << std::endl;
    outfile_ << "\tadd $8, %rbx" << std::endl;
    outfile_ << "\tmov (%rbx), %rcx" << std::endl;
    outfile_ << "\tpush %rcx\n" << std::endl;
  }
}

void CodeGen::Generate(
    std::vector<std::unique_ptr<struct ThreeAddressCode>> blocks) {
  // boiler code here
  GenerateBoiler();

  // Belongs to LHS/RHS Deref, factor it out later
  std::vector<std::string> parsedstring;

  // IR to assembly inst
  for (unsigned int i = 0; i < blocks.size(); ++i) {
    auto code = std::move(blocks[i]);
    if (code.get() == NULL) {
      continue;
    }
    OpcodeType opcode = code->op.opcode();

    switch (opcode) {
      case INT_LOAD:
        GenerateLoadInstructions(std::move(code));
        break;
      case VAR_LOAD:
        GenerateLoadInstructions(std::move(code));
        break;
      case VAR_ASSIGN_LOAD:
        GenerateLoadInstructions(std::move(code));
        break;
      case FUN_ARG_LOAD:
        GenerateLoadInstructions(std::move(code));
        break;
      case FUN_RET_LOAD:
        GenerateLoadInstructions(std::move(code));
        break;
      case ADD:
        GenerateArithmeticExpr(std::move(code), ADD);
        break;
      case SUB:
        GenerateArithmeticExpr(std::move(code), SUB);
        break;
      case MULT:
        GenerateArithmeticExpr(std::move(code), MULT);
        break;
      case DIV:
        GenerateArithmeticExpr(std::move(code), DIV);
        break;
      case LESS_THAN:
        GenerateRelationalExpr(std::move(code), LESS_THAN);
        break;
      case LESS_THAN_EQ:
        GenerateRelationalExpr(std::move(code), LESS_THAN_EQ);
        break;
      case GREATER_THAN:
        GenerateRelationalExpr(std::move(code), GREATER_THAN);
        break;
      case GREATER_THAN_EQ:
        GenerateRelationalExpr(std::move(code), GREATER_THAN_EQ);
        break;
      case EQUAL:
        GenerateRelationalExpr(std::move(code), EQUAL);
        break;
      case LOG_AND:
        GenerateLogicalExpr(std::move(code), LOG_AND);
        break;
      case LOG_OR:
        GenerateLogicalExpr(std::move(code), LOG_OR);
        break;
      case LOG_NOT:
        GenerateLogicalExpr(std::move(code), LOG_NOT);
        break;
      case LOOP:
        outfile_ << "\t# LOOP\n";
        outfile_ << "\tpop %rax" << std::endl;
        outfile_ << "\tcmp $" << std::to_string(code->arg1.value())
                 << ", %rax\n"
                 << std::endl;
        break;
      case CONDITIONAL:
        outfile_ << "\t# CONDITIONAL\n";
        outfile_ << "\tpop %rax" << std::endl;
        outfile_ << "\tcmp $" << std::to_string(code->arg1.value())
                 << ", %rax\n"
                 << std::endl;
        break;
      case JUMP:
        outfile_ << "\t# JUMP\n";
        outfile_ << "\tjmp " << code->target.label().name() << "\n\n";
        break;
      case JMP_EQUAL:
        outfile_ << "\t# Jump on Equal\n";
        outfile_ << "\tje " << code->target.label().name() << "\n\n";
        break;
      case JMP_NOT_EQUAL:
        outfile_ << "\t# Jump on Not Equal\n";
        outfile_ << "\tjne " << code->target.label().name() << "\n\n";
        break;
      case JMP_GREATER_THAN:
        outfile_ << "\t# Jump on greater than\n";
        outfile_ << "\tjg " << code->target.label().name() << "\n\n";
        break;
      case JMP_GREATER_THAN_EQ:
        outfile_ << "\t# Jump on greater or equal\n";
        outfile_ << "\tjge " << code->target.label().name() << "\n\n";
        break;
      case JMP_LESS_THAN:
        outfile_ << "\t# Jump on less than\n";
        outfile_ << "\tjl " << code->target.label().name() << "\n\n";
      case JMP_LESS_THAN_EQ:
        outfile_ << "\t# Jump on less or equal\n";
        outfile_ << "\tjle " << code->target.label().name() << "\n\n";
        break;
      case LABEL:
        outfile_ << code->target.label().name() << ":" << std::endl;
        break;
      case FUN_CALL:
        outfile_ << "\t# Calling Function" << std::endl;
        outfile_ << "\tcall " << code->target.label().name() << "\n\n";
        break;
      case FUN_RET_EP:
        outfile_ << "\t# FunctionRetEpilogue (Restore Stack)" << std::endl;
        outfile_ << "\tadd $" << code->arg1.value() * 16 << ", %rsp\n";
        if (flag_ == PRINT_DEBUG || flag_ == PRINT_PROGRAM) {
          GeneratePrintCall("printfunctionresult");
        }
        break;
      case FUN_DEF:
        // Change scope
        currscope_ = FUNCTION;
        // Clear map
        symbollocations_.clear();
        break;
      case FUN_PROLOGUE:
        outfile_ << "\t# Function Prologue " << std::endl;
        outfile_ << "\tpush %rbp" << std::endl;
        outfile_ << "\tmov %rsp, %rbp" << std::endl;
        outfile_ << "\tpush %rbx" << std::endl;
        outfile_ << "\tsub $" << 16 + code->arg1.value() * 16 << ", %rsp\n\n";
        break;
      case FUN_EPILOGUE:
        outfile_ << "\t# Function Epilogue " << std::endl;
        // Do a correct load
        outfile_ << "\tpop %rax" << std::endl;
        outfile_ << "\tpop %rbx" << std::endl;

        // Rebuild the object into return obj
        outfile_ << "\tmov $returnobj, %rcx" << std::endl;
        outfile_ << "\tmov %rbx, (%rcx)" << std::endl;
        outfile_ << "\tmov %rax, 8(%rcx)" << std::endl;
        outfile_ << "\tmov $returnobj, %rax" << std::endl;

        outfile_ << "\tpop %rbx" << std::endl;
        outfile_ << "\tmov %rbp, %rsp" << std::endl;
        outfile_ << "\tpop %rbp" << std::endl;
        outfile_ << "\tret\n" << std::endl;
        break;
      case PRINT_ARITH:
        // Can be a tuple so if it's a tuple check and print it
        // Correctly
        outfile_ << "\tpop %rax" << std::endl;
        outfile_ << "\tpop %rbx" << std::endl;

        outfile_ << "\tmov %rax, %rdi" << std::endl;
        outfile_ << "\tmov %rbx, %rsi" << std::endl;
        outfile_ << "\tmovzbl %bl, %r12" << std::endl;
        outfile_ << "\tcmp $0, %r12" << std::endl;
        outfile_ << "\tjne printtuple" << std::endl;
        GeneratePrintCall("printresult");
        GenerateEpilogue();
        break;
      case LHS_DEREFERENCE:
        parsedstring = DereferenceParserHelper(code->target.reg().name());
        outfile_ << "\t# LHS_DEREFERENCE of variable "
                 << code->target.reg().name() << std::endl;
        if (parsedstring.size() == 2) {
          if (currscope_ == GLOBAL) {
            outfile_ << "\tmov "
                     << VariableNameHelper(code->arg1.reg().name(), NO_FLAG)
                     << ", %rbx" << std::endl;
          }
          // Error Handling here to check the size
          // and if it's actually a tuple
          outfile_ << "\t#Error Handling Here LHS VARCHILD" << std::endl;
          GenerateBaseDeref(code->arg1.reg().name());
        } else {
          GenerateNestedDeref();
        }
        break;
      case RHS_DEREFERENCE:  // Needs to handle functions
        parsedstring = DereferenceParserHelper(code->target.reg().name());
        outfile_ << "\t#Dereference of variable " << code->target.reg().name()
                 << std::endl;
        if (parsedstring.size() == 2) {
          if (currscope_ == GLOBAL) {
            outfile_ << "\tmov "
                     << VariableNameHelper(code->arg1.reg().name(), NO_FLAG)
                     << ", %rbx" << std::endl;
          }

          // Error Handling here to check the size
          // if you're actually an tuple
          outfile_ << "\t#Error Handling Here RHS Varchild" << std::endl;
          GenerateBaseDeref(code->arg1.reg().name());
          GenerateRHSDerefEpilogue(code->arg2.reg().name());
        } else {
          GenerateNestedDeref();
          GenerateRHSDerefEpilogue(code->arg2.reg().name());
        }
        break;
      case NEW_TUPLE:
        outfile_ << "\t# Making a tuple for variable: "
                 << code->target.reg().name() << std::endl;
        outfile_ << "\tpop %rcx" << std::endl;
        outfile_ << "\tpop %rdx" << std::endl;
        outfile_ << "\tpush %rcx" << std::endl;

        // Check if the rhs is actually a integer
        outfile_ << "\tmovzbl %dl, %rdi" << std::endl;
        outfile_ << "\tcall integerflagcheck" << std::endl;
        outfile_ << "\tpop %rcx" << std::endl;
        outfile_ << "\tpush %rcx" << std::endl;

        // Check if the rhs is greater than zero
        outfile_ << "\tmov %rcx, %rdi" << std::endl;
        outfile_ << "\tcall tuplecreationsizecheck" << std::endl;
        outfile_ << "\tpop %rcx" << std::endl;
        outfile_ << "\tpush %rcx" << std::endl;

        outfile_ << "\timul $16, %rcx" << std::endl;
        outfile_ << "\tmov %rcx, %rdi" << std::endl;
        outfile_ << "\tcall newtuple" << std::endl;

        outfile_ << "\tpop %rcx" << std::endl;
        outfile_ << "\tpop %rbx" << std::endl;

        if (DereferenceParserHelper(code->target.reg().name()).size() == 1 &&
            currscope_ == FUNCTION) {
          // Rewrite flags
          outfile_ << "\tmovb $1,"
                   << VariableNameHelper(code->target.reg().name(), TYPE_FLAG)
                   << std::endl;
          outfile_ << "\tmovb $1, "
                   << VariableNameHelper(code->target.reg().name(),
                                         EXISTENCE_FLAG)
                   << std::endl;
          outfile_ << "\tmovl %ecx, "
                   << VariableNameHelper(code->target.reg().name(), SIZE_FLAG)
                   << std::endl;
          outfile_ << "\tmovq %rax, "
                   << VariableNameHelper(code->target.reg().name(), OBJECT_FLAG)
                   << "\n"
                   << std::endl;
        } else {
          // Rewrite flags
          outfile_ << "\tmovb $1, (%rbx)" << std::endl;
          outfile_ << "\tmovb $1, 1(%rbx)" << std::endl;
          outfile_ << "\tmovl %ecx, 2(%rbx)" << std::endl;
          // Write pointer given by heap into value field
          outfile_ << "\tmovq %rax, 8(%rbx)\n" << std::endl;
        }
        break;
      case VAR_CHILD_TUPLE:
        if (symbollocations_.count(code->target.reg().name()) == 0) {
          // Add it to the map then create a spot for it (if its a function)
          symbollocations_.insert(std::pair<std::string, int>(
              code->target.reg().name(),
              -16 + -16 * (symbollocations_.size() + 1)));
        }
        outfile_ << "\t#Getting value of " << code->target.reg().name() << "\n";
        outfile_ << "\tmov "
                 << VariableNameHelper(code->target.reg().name(), NO_FLAG)
                 << ", %rbx\n";
        outfile_ << "\tpush %rbx\n" << std::endl;
        break;
      default:
        std::cerr << "Unknown opcode " << std::to_string(opcode) << std::endl;
    }
  }

  if (flag_ == PRINT_LAST_ARITHMETIC_EXPR) {
    TestPrint("printresult");
    GenerateEpilogue();
  }

  if ((flag_ == PRINT_DEBUG && currscope_ == GLOBAL)) {
    GenerateEpilogue();
  }

  GeneratePrinter();
  outfile_.close();
}

}  // namespace backend
}  // namespace cs160
