
#include "backend/code_gen.h"

namespace cs160 {
namespace backend {

void CodeGen::Generate(std::vector<struct ThreeAddressCode*> blocks) {
  // boiler code here

  // IR to assembly inst
  for (unsigned int i = 0; i < blocks.size(); ++i) {
    ThreeAddressCode* code = blocks[i];
    std::string OpCode = code->op;
    if (OpCode == "load") {
      outfile_ << "movb $" + code-> arg1 + " %ecx" << std::endl;
      
      // Temporary solution to limited mem locations, keep track of 
      // where we're storing

      outfile_ << "movb %ecx (some memory location + i*4)" << std::endl;
      // memorymap_.insert(std::pair<string,int>(code->target,some memory location + i*4));

    } else if (OpCode == "+") {
        // Load arg1,arg2 then add them into target
        outfile_ << "movb memory_loc_arg1 %eax" << std::endl;
        outfile_ << "movb memory_loc_arg2 %ebx" << std::endl;
        outfile_ << "add %eax %ecx\n add %ebx %ecx" << std::endl; 

        outfile_ << "movb %ecx (some memory location + i*4)" << std::endl;
        // memorymap_.insert(std::pair<string,int>(code->target,some memory location + i*4));

    } else if (OpCode == "-") {
        // Load arg1,arg2 then add them into target
        outfile_ << "movb memory_loc_arg1 %ebx" << std::endl;
        outfile_ << "movb memory_loc_arg2 %ecx" << std::endl;
        outfile_ << "sub %eax %ecx\nsub %ebx %ecx" << std::endl; 

        outfile_ << "movb %ecx (some memory location + i*4)" << std::endl;
        // memorymap_.insert(std::pair<string,int>(code->target,some memory location + i*4));
    } else if (OpCode == "*") {

        outfile_ << "movb memory_loc_arg1 %ebx" << std::endl;
        outfile_ << "movb memory_loc_arg2 %ecx" << std::endl;
        outfile_ << "imul %ebx %ecx" << std::endl; 

        outfile_ << "movb %ecx (some memory location + i*4)" << std::endl;

        // memorymap_.insert(std::pair<string,int>(code->target,some memory location + i*4));
    } else if (OpCode == "/") {
        // Load dividend (arg1) into %eax (Do we need to clear out %edx?)
        //outfile_<< "mov $0, %edx" << endl;
        outfile_ << "movb memory_loc_arg1 %eax" << std::endl;
        outfile_ << "movb memory_loc_arg2 %ebx" << std::endl;
        outfile_ << "cdq" << std::endl;
        // Div
        outfile_ << "idiv %ebx" << std::endl;
    }
  }

  outfile_.close();
}

}  // namespace backend
}  // namespace cs160
