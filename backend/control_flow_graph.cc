#include "backend/control_flow_graph.h"

namespace cs160 {
namespace backend {

ControlFlowGraph::ControlFlowGraph() {
  
}

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
    error_node->SetBlockType(ERROR_TYPE);
    std::cerr << "Error block created, edge creation failed. Creating Error Node" << std::endl;
    return error_node.get();
  } else if (graph_set.size() == 1) {
    ControlFlowGraphNode * temp = graph_set[0];
    graph_set.erase(graph_set.begin());
    return temp;
  } else {
    if (graph_set.front()->GetBlockType() == CONDITIONAL_BLOCK) {
      // Should look like this:
      //      Condition
      //      |     |
      //    True   False
      //      \     /
      //        END
      // temp0 = Condition
      // temp1 = True
      // temp2 = False
      // temp3 = END
      //ControlFlowGraphNode * temp0 = RecursiveCreate(graph_set,edge_graph);
      ControlFlowGraphNode * temp0 = graph_set.front(); 
      graph_set.erase(graph_set.begin());
      ControlFlowGraphNode * next_true_node = graph_set.front();
      ControlFlowGraphNode * temp1 = RecursiveCreate(graph_set,edge_graph);
      ControlFlowGraphNode * temp2 = RecursiveCreate(graph_set,edge_graph);
      ControlFlowGraphNode * temp3 = RecursiveCreate(graph_set,edge_graph);
      int node0 = temp0->GetCreationOrder();
      int next_true = next_true_node->GetCreationOrder();
      int node1 = temp1->GetCreationOrder();
      int next_false = node1 + 1;
      int node2 = temp2->GetCreationOrder();
      int node3 = temp3->GetCreationOrder();
      Edge edge1(std::make_pair(node0,next_true), CONDITIONAL_TRUE);
      Edge edge2(std::make_pair(node0,next_false), CONDITIONAL_FALSE);
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
      //ControlFlowGraphNode * temp0 = RecursiveCreate(graph_set,edge_graph);
      ControlFlowGraphNode * temp0 = graph_set.front();
      graph_set.erase(graph_set.begin());
      ControlFlowGraphNode * next_node = graph_set.front(); //Needed for nested statements
      ControlFlowGraphNode * temp1 = RecursiveCreate(graph_set,edge_graph);
      ControlFlowGraphNode * temp2 = RecursiveCreate(graph_set,edge_graph);
      int node0 = temp0->GetCreationOrder();
      int next = next_node->GetCreationOrder();
      int node1 = temp1->GetCreationOrder();
      int node2 = temp2->GetCreationOrder();
      Edge edge1(std::make_pair(node0,next), LOOP_TRUE);
      Edge edge2(std::make_pair(node1,node0), LOOP_FALSE);
      Edge edge3(std::make_pair(node0,node2), LOOP_RETURN);
      edge_graph.push_back(edge1);
      edge_graph.push_back(edge2);
      edge_graph.push_back(edge3);
      return temp2;
    } else { // It is a type that we don't need to branch on
      ControlFlowGraphNode * temp = graph_set[0];
      graph_set.erase(graph_set.begin());
      return temp; 
    }
  }
}

void ControlFlowGraph::CreateCFG(std::vector<std::unique_ptr<struct ThreeAddressCode>> input) {
  // Has to branch on:
  // IF/WHILE/FUNCTION
  // Each one has a different branching pattern
  //std::vector<std::unique_ptr<ControlFlowGraphNode>> cfg_vector;
  cfg_nodes_.clear();
  std::vector<std::unique_ptr<struct ThreeAddressCode>> new_block;
  int creation_order = 0;
  for (auto &iter: input) {
    //Move IR to the block
    OpcodeType op_type = iter->op.opcode();
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

//Apply a local optimization
std::vector<std::unique_ptr<struct ThreeAddressCode>> MarkSweep(
    std::vector<std::string> &live_set,
    ControlFlowGraphNode * apply_sweep) {
  std::vector<std::unique_ptr<struct ThreeAddressCode>> optimize_block = std::move(apply_sweep->GetLocalBlock());
  for (auto &iter: optimize_block) {
    //Target is the LHS check
    //Opcode is the RHS check
    if (iter->target.reg().type() == VARIABLEREG) {
      //Check if the variable is in the liveset
      if (std::find(live_set.begin(),live_set.end(), iter->target.reg().name()) != live_set.end()) {
        //Check if variable isn't being used on the RHS
        if(iter->target.reg().name() != iter->arg1.reg().name() || iter->target.reg().name() != iter->arg2.reg().name()) {
          //Delete if its not in the set
          iter.reset();
        }
      } else {
        live_set.erase(std::remove(live_set.begin(),live_set.end(),iter->target.reg().name()),live_set.end());
        //Remove it from the live set

      }
    } 
    if (iter->arg1.reg().type() == VARIABLEREG) {
      live_set.push_back(iter->arg1.reg().name());
    }
    if (iter->arg2.reg().type() == VARIABLEREG) {
      live_set.push_back(iter->arg2.reg().name());
    }
  }
  //return std::move(optimize_block); 


}

std::vector<std::string> MergeVector(std::vector<std::string> vector1, std::vector<std::string> vector2) {
  std::vector<std::string> return_vector = vector1;
  for (auto iter: vector2) {
    std::vector<std::string>::iterator find_value = std::find(vector2.begin(), vector2.end(), iter);
    if (find_value != vector2.end()) {
      return_vector.push_back(iter);
    }
  }
  return return_vector;
}

std::pair<std::vector<std::string>, ControlFlowGraphNode *> RecursiveFindPath(
  std::vector<ControlFlowGraphNode *> passed_tac,
  std::vector<Edge> edges, std::vector<std::string> live_set,
   ControlFlowGraphNode * optimize_node) {
    //Optimize the local block
    int node_number = optimize_node->GetCreationOrder();
    optimize_node->SetLocalBlock(std::move(MarkSweep(live_set, optimize_node)));
    EdgeType edge1 = TYPELESS_EDGE;
    EdgeType edge2 = TYPELESS_EDGE;
    int block1 = -1;
    int block2 = -1;
    for (auto iter : edges) {
      if (iter.edge_pair.second == node_number) {
        if (edge1 = TYPELESS_EDGE) {
          edge1 = iter.edge_type;
          block1 = iter.edge_pair.first;
        } else if (edge2 = TYPELESS_EDGE) { 
          edge2 = iter.edge_type;
          block2 = iter.edge_pair.second;
        } else {
          std::cerr << "More than 2 edges detected" << std::endl;
        }
      }
    }
    ControlFlowGraphNode * node1;
    ControlFlowGraphNode * node2;
    for (auto tac_iter : passed_tac) {
      if (tac_iter->GetCreationOrder() == block1) {
        node1 = tac_iter;
      } else if (tac_iter->GetCreationOrder() == block2) {
        node2 = tac_iter;
      }
    }
    if (edge1 == CONDITIONAL_TRUE_RETURN || edge1 == CONDITIONAL_FALSE_RETURN) {
      std::vector<std::string> live_set_copy = live_set;
      std::pair<std::vector<std::string>,ControlFlowGraphNode * > return1, return2, return3;
      std::vector<std::string> str_return1, str_return2, merged_return;
      return1 = RecursiveFindPath(passed_tac, edges, live_set, node1);
      return2 = RecursiveFindPath(passed_tac, edges, live_set_copy, node2);
      str_return1 = return1.first;
      str_return2 = return2.first;
      merged_return = MergeVector(str_return1,str_return2);
      return3 = RecursiveFindPath(passed_tac, edges, merged_return, return1.second);
      return return3; //Idunno actually
    } else if (edge1 == CONDITIONAL_TRUE_RETURN || edge1 == CONDITIONAL_FALSE_RETURN) {
      return std::make_pair(live_set, node1);
    } else if (edge1 == LOOP_TRUE) {
      RecursiveFindPath(passed_tac, edges, live_set, node1);
      if(block2 != -1) { //If there is something that branches off of this 
        //Maybe I need a visited?
        RecursiveFindPath(passed_tac, edges, live_set, node2);
      }
      return std::make_pair(live_set, node1); //Dunno if I need any of these
    } else if (edge1 == LOOP_FALSE) {
      RecursiveFindPath(passed_tac, edges, live_set, node1);
      return std::make_pair(live_set, node1);
    } else if (edge1 == LOOP_RETURN) {
      RecursiveFindPath(passed_tac, edges, live_set, node1);
      return std::make_pair(live_set, node1);
    }


}

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

void ControlFlowGraph::DebugPrint() {
  for (auto &iter: cfg_nodes_) {
    std::cout << "Creation Order: "<< iter->GetCreationOrder() << 
    " Block Type: "<< iter->GetBlockType() << std::endl;
  }
  std::cout << "EDGES: " << std::endl;
  for (auto iter1: edges_) {
    std::cout << "From: " << iter1.edge_pair.first << " To: " << iter1.edge_pair.second
    << " Edge Type: " << iter1.edge_type << std::endl;
  }
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

std::string ControlFlowGraph::GetOutput() {
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
  // Iterate through the vector and print out each basic block
  std::string output = "";
  std::vector<std::string> printhelper = {"INTLOAD", "VARLOAD", "VARASSIGNLOAD",
    "FUNARGLOAD", "FUNRETLOAD", "+", "-", "*", "/", "<", "<=", ">", ">=",
    "==", "&&", "||", "¬", "while", "if", "jmp", "je", "jne", "jg", "jge",
    "jl", "jle", "MkLabel", "FUNCTIONCALL", "FUNRETURNEPILOGUE",
    "FUNCTIONDEF", "FUNPROLOGUE", "FUNEPILOGUE", "PRINTARITH", "NOTYPE",
    "LHSDEREFERENCE", "RHSDEREFERENCE", "NEWTUPLE",
    "VARCHILDTUPLE"};

  for (unsigned int i = 0; i < return_three_address.size(); ++i) {
    // If it's a just a int (Register without a name then access it's value)
    // Otherwise access its name
    OpcodeType opcodetype = return_three_address[i]->op.opcode();
    switch (opcodetype) {
      case INTLOAD:
        output = output + return_three_address[i]->target.reg().name()
          + " <- " + std::to_string(return_three_address[i]->arg1.value());
        break;
      case VARASSIGNLOAD:
        output = output + return_three_address[i]->target.reg().name()
              + " <- " + return_three_address[i]->arg1.reg().name();
        break;
      case FUNARGLOAD:
        output = output + return_three_address[i]->target.reg().name()
              + " <- " + std::to_string(return_three_address[i]->arg1.value());
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
        output = output + return_three_address[i]->target.reg().name() + " <- " +
                 printhelper[LOGNOT] + return_three_address[i]->arg1.reg().name();
        break;
      case LOOP:
        output = output + printhelper[LOOP] + " " +
                 return_three_address[i]->arg2.reg().name() + " == 0";
        break;
      case CONDITIONAL:
        output = output + printhelper[CONDITIONAL] + " " +
                 return_three_address[i]->arg2.reg().name() + " == 0";
        break;
      // Abstract jumps out
      case JUMP:
        output = output + printhelper[JUMP] + " " +
                 return_three_address[i]->target.label().name();
        break;
      case JEQUAL:
        output = output + printhelper[JEQUAL] + " " +
                 return_three_address[i]->target.label().name();
        break;
      case JNOTEQUAL:
        output = output + printhelper[JNOTEQUAL] + " " +
                 return_three_address[i]->target.label().name();
        break;
      case JGREATER:
        output = output + printhelper[JGREATER] + " " +
                 return_three_address[i]->target.label().name();
        break;
      case JGREATEREQ:
        output = output + printhelper[JGREATEREQ] + " " +
                 return_three_address[i]->target.label().name();
        break;
      case JLESS:
        output = output + printhelper[JLESS] + " " +
                 return_three_address[i]->target.label().name();
        break;
      case JLESSEQ:
        output = output + printhelper[JLESSEQ] + " " +
                 return_three_address[i]->target.label().name();
        break;
      case LABEL:
        output = output + printhelper[LABEL] + " " +
                 return_three_address[i]->target.label().name();
        break;
      default:
        output = output + return_three_address[i]->target.reg().name() + " <- " +
           return_three_address[i]->arg1.reg().name();

        output = output + " " + printhelper[return_three_address[i]->op.opcode()] + " " +
           return_three_address[i]->arg2.reg().name();
        break;
    }
    output = output + "\n";
  }

  return output;
}
ControlFlowGraphNode::ControlFlowGraphNode() {
  creation_order = 0;
  blocktype_ = NO_TYPE;
}

ControlFlowGraphNode::ControlFlowGraphNode(std::vector<std::unique_ptr<struct ThreeAddressCode>> input) {
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
