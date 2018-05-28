#include "backend/control_flow_graph.h"

namespace cs160 {
namespace backend {

ControlFlowGraph::ControlFlowGraph
(std::vector<std::unique_ptr<struct ThreeAddressCode>> input) {
  CreateCFG(input);
}

void ControlFlowGraph::CreateCFG(std::vector<std::unique_ptr<struct ThreeAddressCode>> input) {
  // Has to branch on:
  // IF/FOR/WHILE/FUNCTION
  // Each one has a different branching pattern
  ControlFlowGraphNode root;  
  //for (std::unique_ptr<struct ThreeAddressCode> iter : input) {
  //  return;
  //}
  for (const auto &iter: input) {
    if (iter->op.opcode() == CONDITIONAL) {

    } else if (iter->op.opcode() == LOOP) {

    } else if (iter->op.opcode() == FUNCALL) {

    }

  } 
  root_ = root;
}

ControlFlowGraphNode::ControlFlowGraphNode() {
  leftnode_ = NULL;
  rightnode_ =   NULL;
}

ControlFlowGraphNode::ControlFlowGraphNode(ControlFlowGraphNode &copy) {
  //std::vector<std::unique_ptr<ThreeAddressCode>> block_copy;
  localblock_.clear();
  for (const auto& iter: copy.GetLocalBlock()) {
    localblock_.push_back(std::make_unique<ThreeAddressCode>(iter) );
  }
  //localblock_ = block_copy;
  leftnode_ = copy.GetLeftNode();
  rightnode_ = copy.GetRightNode();
}

ControlFlowGraphNode ControlFlowGraphNode::operator=(ControlFlowGraphNode copy) {
  localblock_ = copy.GetLocalBlock();
  leftnode_ = copy.GetLeftNode();
  rightnode_ = copy.GetRightNode();
}

void ControlFlowGraphNode::SetLocalBlock(std::vector<std::unique_ptr<struct ThreeAddressCode>> input) {
  localblock_ = input;
}

void ControlFlowGraphNode::SetLeftNode(std::unique_ptr<ControlFlowGraphNode> input) {
  leftnode_ = std::move(input);
}

void ControlFlowGraphNode::SetRightNode(std::unique_ptr<ControlFlowGraphNode> input) {
  rightnode_ = std::move(input);
}


} //backend
} //cs160
