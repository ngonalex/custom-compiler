#include "backend/control_flow_graph.h"

namespace cs160 {
namespace backend {

ControlFlowGraph::ControlFlowGraph
(std::vector<std::unique_ptr<struct ThreeAddressCode>> input) {
  CreateCFG(input);
}

void ControlFlowGraph::CreateCFG(std::vector<std::unique_ptr<struct ThreeAddressCode>> input) {
  // Has to branch on:
  // IF/WHILE/FUNCTION
  // Each one has a different branching pattern
  ControlFlowGraphNode root;  
  //for (std::unique_ptr<struct ThreeAddressCode> iter : input) {
  //  return;
  //}
  std::vector<ControlFlowGraphNode> cfg_vector;
  //ControlFlowGraphNode new_node,true_node, false_node; 
  std::vector<std::unique_ptr<struct ThreeAddressCode>> new_block;
  int creation_order = 0;
  for (const auto &iter: input) {
    if (iter->op.opcode() == CONDITIONAL) {
      ControlFlowGraphNode conditional_block(new_block);
      conditional_block.SetCreationOrder(creation_order);
      new_block.clear();
      cfg_vector.pushback(conditional_block);
      ++creation_order;
    } else if (iter->op.opcode() == LOOP) {
      ControlFlowGraphNode loop_block(new_block);
      loop_block.SetCreationOrder(creation_order);
      new_block.clear();
      cfg_vector.pushback(loop_block);
      ++creation_order;
    } else if (iter->op.opcode() == FUNCALL) {
      ControlFlowGraphNode function_block(new_block);
      function_block.SetCreationOrder(creation_order);
      new_block.clear();
      cfg_vector.pushback(function_block);
      ++creation_order;
    } else if (iter->op.opcode() == JUMP) {
      ControlFlowGraphNode block(new_block);
      block.SetCreationOrder(creation_order);
      new_block.clear();
      cfg_vector.pushback(block);
      ++creation_order;
    } else {
      new_block.pushback(std::make_unique<ThreeAddressCode>(iter));
    }

  } 
  root = RecursiveCreate(cfg_vector);
  root_ = root;
}
ControlFlowGraphNode RecursiveCreate(std::vector<ControlFlowGraphNode> graph_set) {
  if (graph.empty()) {
    return NULL;
  } else if (graph.first(){

  }
}
ControlFlowGraphNode::ControlFlowGraphNode() {
  creation_order = 0;
  leftnode_ = NULL;
  rightnode_ = NULL;
  blocktype_ = NO_TYPE;
}

ControlFlowGraphNode::ControlFlowGraphNode(std::vector<std::unique_ptr<struct ThreeAddressCode>> input) {
  for (const auto& iter: copy.GetLocalBlock()) {
    localblock_.push_back(std::make_unique<ThreeAddressCode>(iter));
  }
  creation_order = 0;
  leftnode_ = NULL;
  rightnode_ = NULL;
  blocktype_ = NO_TYPE;
}

ControlFlowGraphNode::ControlFlowGraphNode(ControlFlowGraphNode &copy) {
  //std::vector<std::unique_ptr<ThreeAddressCode>> block_copy;
  localblock_.clear();
  // Because it is a vector of unique pointers
  // Each element has to be manually copied
  for (const auto& iter: copy.GetLocalBlock()) {
    localblock_.push_back(std::make_unique<ThreeAddressCode>(iter));
  }
  //localblock_ = block_copy;
  creation_order = copy.GetCreationOrder();
  leftnode_ = copy.GetLeftNode();
  rightnode_ = copy.GetRightNode();
  blockttype_ = copy.GetBlockType();
}

ControlFlowGraphNode ControlFlowGraphNode::operator=(ControlFlowGraphNode copy) {
  //localblock_ = copy.GetLocalBlock();
  localblock_.clear();
  for (const auto& iter: copy.GetLocalBlock()) {
    localblock_.push_back(std::make_unique<ThreeAddressCode>(iter));
  }
  creation_order = copy.GetCreationOrder();
  leftnode_ = copy.GetLeftNode();
  rightnode_ = copy.GetRightNode();
  blockttype_ = copy.GetBlockType();
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
