#ifndef BACKEND_CONTROL_FLOW_GRAPH_H_
#define BACKEND_CONTROL_FLOW_GRAPH_H_

#include <vector>

#include "utility/memory.h"
#include "backend/lowerer_visitor.h"
#include "backend/ir.h"

namespace cs160 {
namespace backend {

class ControlFlowGraph {
 public:
  ControlFlowGraph(std::vector<std::unique_ptr<struct ThreeAddressCode>>);
  ControlFlowGraphNode GetRoot() {
   return root_;
  }
  void CreateCFG(std::vector<std::unique_ptr<struct ThreeAddressCode>>);
  void Optimize();
  std::vector<std::unique_ptr<struct ThreeAddressCode>> MakeThreeAddressCode();

 private:
  ControlFlowGraphNode root_;
};


enum BlockType {
  CONDITIONAL_BLOCK, LOOP_BLOCK,
  FUNCTION_BLOCK, NO_TYPE
};

// Individual nodes in the CGF
// leftnode_ will be true branches
// rightnode_ will be false branches
class ControlFlowGraphNode {
 public:
  ControlFlowGraphNode();
  ControlFlowGraphNode(std::vector<std::unique_ptr<struct ThreeAddressCode>>);
  std::vector<std::unique_ptr<struct ThreeAddressCode>> GetLocalBlock() {
   return localblock_;
  }
  ControlFlowGraphNode(ControlFlowGraphNode &copy);
  std::unique_ptr<ControlFlowGraphNode> GetLeftNode() {
    return std::move(leftnode_); 
  }
  std::unique_ptr<ControlFlowGraphNode> GetRightNode() {
    return std::move(rightnode_); 
  }
  int GetCreationOrder() {
    return creation_order;
  }
  BlockType GetBlcokType() {
    return blockttype_;
  }
  ControlFlowGraphNode operator=(ControlFlowGraphNode);
  void SetLocalBlock(std::vector<std::unique_ptr<struct ThreeAddressCode>>);
  void SetLeftNode(std::unique_ptr<ControlFlowGraphNode>);
  void SetRightNode(std::unique_ptr<ControlFlowGraphNode>);
  void SetCreationOrder(int order) {
    creation_order = order;
  }
  void SetBlockType(BlockType settype) {
    blocktype_ = settype;
  }

 private:
  BlockType blocktype_
  int creation_order;
  std::vector<std::unique_ptr<struct ThreeAddressCode>> localblock_;
  std::unique_ptr<ControlFlowGraphNode> leftnode_; 
  std::unique_ptr<ControlFlowGraphNode> rightnode_;
};

} // namespace backend
} // namespace cs160

#endif // CONTROL_FLOW_GRAPH_H_