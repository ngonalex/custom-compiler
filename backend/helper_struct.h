#ifndef BACKEND_HELPER_STRUCT_H_
#define BACKEND_HELPER_STRUCT_H_


namespace cs160 {
namespace backend {

  struct Counter {
    int variablecount;
    int branchcount;
    int continuecount;
    Counter() : variablecount(0), branchcount(0), continuecount(0) {}
  };

}  // namespace backend
}  // namespace cs160

#endif  // BACKEND_HELPER_STRUCT_H_