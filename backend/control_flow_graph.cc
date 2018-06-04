#include "backend/control_flow_graph.h"

namespace cs160 {
namespace backend {

ControlFlowGraph::ControlFlowGraph
(std::vector<std::unique_ptr<struct ThreeAddressCode>> input) {
  CreateCFG(std::move(input));
}

//Use Recursion to find all the edges in the graph
ControlFlowGraphNode* RecursiveCreate(std::vector<ControlFlowGraphNode*> graph_set, 
  std::vector<Edge> &edge_graph) {
  if (graph_set.empty()) {    
    //If this occurs then there's a bigger problem
    std::vector<std::unique_ptr<struct ThreeAddressCode>> empty;
    auto error_node = make_unique<class ControlFlowGraphNode>(std::move(empty));
    //ControlFlowGraphNode error_node;
    error_node->SetBlockType(ERROR_TYPE);
    std::cout << "Error block created" << std::endl;
    return error_node.get();
  } else if (graph_set.size() == 1) {
    ControlFlowGraphNode * temp = graph_set[0];
    graph_set.erase(graph_set.begin());
    return temp;
  } else {
    if (graph_set.front()->GetBlockType() == CONDITIONAL_BLOCK) {
      // Should look like this:
      //      Condition
      //      /     \
      //    True   False
      //      \     /
      //        END
      // temp0 = Condition
      // temp1 = True
      // temp2 = False
      // temp3 = END
      ControlFlowGraphNode * temp0 = RecursiveCreate(graph_set,edge_graph);
      ControlFlowGraphNode * temp1 = RecursiveCreate(graph_set,edge_graph);
      ControlFlowGraphNode * temp2 = RecursiveCreate(graph_set,edge_graph);
      ControlFlowGraphNode * temp3 = RecursiveCreate(graph_set,edge_graph);
      int node0 = temp0->GetCreationOrder();
      int node1 = temp1->GetCreationOrder();
      int node2 = temp2->GetCreationOrder();
      int node3 = temp3->GetCreationOrder();
      Edge edge1(std::make_pair(node0,node1), CONDITIONAL_TRUE);
      Edge edge2(std::make_pair(node0,node2), CONDITIONAL_FALSE);
      Edge edge3(std::make_pair(node1,node3), CONDITIONAL_TRUE_RETURN);
      Edge edge4(std::make_pair(node2,node3), CONDITIONAL_FALSE_RETURN);
      edge_graph.push_back(edge1);
      edge_graph.push_back(edge2);
      edge_graph.push_back(edge3);
      edge_graph.push_back(edge4);
      return temp3;
    } else if (graph_set.front()->GetBlockType() == LOOP_BLOCK) { 
      // Should look like this:
      //      Loop
      //      /\  \
      //     True  \
      //           False
      // temp0 = Loop
      // temp1 = True
      // temp2 = False
      ControlFlowGraphNode * temp0 = RecursiveCreate(graph_set,edge_graph);
      ControlFlowGraphNode * temp1 = RecursiveCreate(graph_set,edge_graph);
      ControlFlowGraphNode * temp2 = RecursiveCreate(graph_set,edge_graph);
      int node0 = temp0->GetCreationOrder();
      int node1 = temp1->GetCreationOrder();
      int node2 = temp2->GetCreationOrder();
      Edge edge1(std::make_pair(node0,node1), LOOP_TRUE);
      Edge edge2(std::make_pair(node1,node0), LOOP_FALSE);
      Edge edge3(std::make_pair(node0,node2), LOOP_RETURN);
      edge_graph.push_back(edge1);
      edge_graph.push_back(edge2);
      edge_graph.push_back(edge3);
      return temp2;
    } else { // NO TYPE
      ControlFlowGraphNode * temp = graph_set[0];
      graph_set.erase(graph_set.begin());
      return temp; 
    }
  }
}
// std::vector<std::unique_ptr<struct ThreeAddressCode>> CopyBlock(std::vector<std::unique_ptr<struct ThreeAddressCode>> copy) {
//   std::vector<std::unique_ptr<struct ThreeAddressCode>> copy_object;
//   for (const auto& iter: copy) {
//     auto block = make_unique<struct ThreeAddressCode>();
//     block->target = iter->target;
//     block->op = iter->op;
//     copy_object.push_back(std::move(block));
//   }
//   return std::move(copy_object);
// }

void ControlFlowGraph::CreateCFG(std::vector<std::unique_ptr<struct ThreeAddressCode>> input) {
  // Has to branch on:
  // IF/WHILE/FUNCTION
  // Each one has a different branching pattern
  //std::vector<std::unique_ptr<ControlFlowGraphNode>> cfg_vector;
  cfg_nodes_.clear();
  std::vector<std::unique_ptr<struct ThreeAddressCode>> new_block;
  int creation_order = 0;
  for (auto &iter: input) {
    //Create a copy of the IR
    // auto block = make_unique<struct ThreeAddressCode>();
    // block->target = iter->target;
    // block->op = iter->op;
    // new_block.push_back(std::move(block));
    Type op_type = iter->op.opcode();
    bool is_end = (iter == input.back());
    new_block.push_back(std::move(iter));
    //Create a block if neccessary
    if (op_type == CONDITIONAL) {
      auto conditional_block = make_unique<class ControlFlowGraphNode>(std::move(new_block));
      conditional_block->SetCreationOrder(creation_order);
      conditional_block->SetBlockType(CONDITIONAL_BLOCK);
      std::vector<std::unique_ptr<struct ThreeAddressCode>> new_block;
      cfg_nodes_.push_back(std::move(conditional_block));
      ++creation_order;
    } else if (op_type == LOOP) {
      auto loop_block = make_unique<class ControlFlowGraphNode>(std::move(new_block));
      loop_block->SetCreationOrder(creation_order);
      loop_block->SetBlockType(LOOP_BLOCK);
      std::vector<std::unique_ptr<struct ThreeAddressCode>> new_block;
      cfg_nodes_.push_back(std::move(loop_block));
      ++creation_order;
    } else if (op_type == JUMP) {
      auto block = make_unique<class ControlFlowGraphNode>(std::move(new_block));
      block->SetCreationOrder(creation_order);
      block->SetBlockType(NO_TYPE);
      std::vector<std::unique_ptr<struct ThreeAddressCode>> new_block;
      cfg_nodes_.push_back(std::move(block));
      ++creation_order;
    } else if (is_end) {
      auto block = make_unique<class ControlFlowGraphNode>(std::move(new_block));
      block->SetCreationOrder(creation_order);
      block->SetBlockType(END_BLOCK);
      std::vector<std::unique_ptr<struct ThreeAddressCode>> new_block;
      cfg_nodes_.push_back(std::move(block));
      ++creation_order;
    } else {
      //No need to make a block
    }

  } 
  //Use CFGN to recursively create edge vector
  std::vector<ControlFlowGraphNode *> cfg_pointers;
  for(auto &iter: cfg_nodes_) {
    cfg_pointers.push_back(iter.get());
  }
  std::vector<Edge> edge_vector;
  RecursiveCreate(cfg_pointers,edge_vector);
  edges_ = edge_vector;
  //cfg_nodes_ = std::move(cfg_vector);
}

std::vector<std::unique_ptr<struct ThreeAddressCode>> MarkSweep(
  std::vector<std::string> &live_set,
  ControlFlowGraphNode * apply_sweep) {

}

std::vector<std::string> RecursiveFindPath (
  std::vector<ControlFlowGraphNode *> passed_tac,
  std::vector<Edge> edges, std::vector<std::string> live_set,
   ControlFlowGraphNode * optimize_node) {
    //Optimize the local block
    int node_number = optimize_node->GetCreationOrder();
    optimize_node->SetLocalBlock(std::move(MarkSweep(live_set, optimize_node)));
    EdgeType edge1 = TYPELESS_EDGE;
    EdgeType edge2 = TYPELESS_EDGE;
    for (Edge iter : edges) {
      if(iter.edge_pair.second == node_number) {
        if(edge1 = TYPELESS_EDGE) {
          edge1 = iter.edge_type;
        }
        else { 
          edge2 = iter.edge_type;
        }
      }
    }
    if(edge1 == CONDITIONAL_FALSE_RETURN || edge1 == CONDITIONAL_TRUE_RETURN) {


      
    }


}
// std::vector<std::unique_ptr<struct ThreeAddressCode>> Sweep(
//   std::pair<bool,std::vector<std::unique_ptr<struct ThreeAddressCode>>> marked_block) {

// }

std::vector<std::unique_ptr<ControlFlowGraphNode>> OptimizeHelp(
  std::vector<std::unique_ptr<ControlFlowGraphNode>> cfg_node,
  std::vector<Edge> edges) {
    std::vector<ControlFlowGraphNode *> cfg_pointer;
    for (auto &iter: cfg_node) {
      cfg_pointer.push_back(iter.get());
    }
    std::vector<std::string> live_set;
    ControlFlowGraphNode * end = cfg_node.end()->get();
    RecursiveFindPath(cfg_pointer,edges,live_set, end);
    return cfg_node;
 }

void ControlFlowGraph::Optimize() {
  cfg_nodes_ = std::move(OptimizeHelp(std::move(cfg_nodes_),edges_));
}

std::vector<std::unique_ptr<struct ThreeAddressCode>> ControlFlowGraph::MakeThreeAddressCode() {
  int creation_check = 0;
  std::vector<std::unique_ptr<struct ThreeAddressCode>> return_three_address;
  while(!cfg_nodes_.empty()) {
    for(auto &iter: cfg_nodes_) {
      if(iter->GetCreationOrder() == creation_check) {
        for(auto &three_iter: iter->GetLocalBlock()) {
          return_three_address.push_back(std::move(three_iter));
        }
      }
    }
    creation_check++;
  }
  return std::move(return_three_address);
}

ControlFlowGraphNode::ControlFlowGraphNode() {
  creation_order = 0;
  blocktype_ = NO_TYPE;
}

ControlFlowGraphNode::ControlFlowGraphNode(std::vector<std::unique_ptr<struct ThreeAddressCode>> input) {
  // for (const auto& iter: input) {
  //   auto block = make_unique<struct ThreeAddressCode>();
  //   block->target = iter->target;
  //   block->op = iter->op;
  //   block->arg1 = iter->arg1;
  //   block->arg2 = iter->arg2;
  //   localblock_.push_back(std::move(block));
  // }
  localblock_ = std::move(input);
  creation_order = 0;
  blocktype_ = NO_TYPE;
}

ControlFlowGraphNode::ControlFlowGraphNode( ControlFlowGraphNode &copy) {
  localblock_.clear();
  // Because it is a vector of unique pointers
  // Each element has to be manually copied
  for (const auto& iter: copy.GetLocalBlock()) {
    auto block = make_unique<struct ThreeAddressCode>();
    block->target = iter->target;
    block->op = iter->op;
    block->arg1 = iter->arg1;
    block->arg2 = iter->arg2;
    localblock_.push_back(std::move(block));
  }
  //localblock_ = std::move(copy.GetLocalBlock());
  creation_order = copy.GetCreationOrder();
  blocktype_ = copy.GetBlockType();
}

ControlFlowGraphNode& ControlFlowGraphNode::operator=(ControlFlowGraphNode &copy) {
  localblock_.clear();
  for (const auto& iter: copy.GetLocalBlock()) {
    auto block = make_unique<struct ThreeAddressCode>();
    block->target = iter->target;
    block->op = iter->op;
    block->arg1 = iter->arg1;
    block->arg2 = iter->arg2;
    localblock_.push_back(std::move(block));
  }
  //localblock_ = std::move(copy.GetLocalBlock());
  creation_order = copy.GetCreationOrder();
  blocktype_ = copy.GetBlockType();
  return *this;
}

ControlFlowGraphNode ControlFlowGraphNode::operator=(ControlFlowGraphNode copy) {
  localblock_.clear();
  // for (const auto& iter: copy.GetLocalBlock()) {
  //   auto block = make_unique<struct ThreeAddressCode>();
  //   block->target = iter->target;
  //   block->op = iter->op;
  //   block->arg1 = iter->arg1;
  //   block->arg2 = iter->arg2;
  //   localblock_.push_back(std::move(block));
  // }
  localblock_ = std::move(copy.GetLocalBlock());
  creation_order = copy.GetCreationOrder();
  blocktype_ = copy.GetBlockType();
  return *this;
}

void ControlFlowGraphNode::SetLocalBlock(std::vector<std::unique_ptr<struct ThreeAddressCode>> input) {
  // for (const auto& iter: input) {
  //   auto block = make_unique<struct ThreeAddressCode>();
  //   block->target = iter->target;
  //   block->op = iter->op;
  //   localblock_.push_back(std::move(block));
  // }
  localblock_ = std::move(input);
}

} //backend
} //cs160
