#include "route_model.h"
#include <iostream>

RouteModel::RouteModel(const std::vector<std::byte> &xml) : Model(xml) {
  int counter = 0;
  for (Model::Node node: this->Nodes()){
    m_Nodes.push_back(Node(counter, this, node));
    counter++;
  }
  CreateNodeToRoadHashmap();
}
  
// step 14 in notes
void RouteModel::CreateNodeToRoadHashmap() {
  for (const Model::Road &road : Roads()) {
    if (road.type != Model::Road::Type::Footway){
      auto ways = Ways()[road.way].nodes;
      for(int idx : ways) {
        if (node_to_road.find(idx) == node_to_road.end()){
          node_to_road[idx] = std::vector<const Model::Road *> ();
        }
        node_to_road[idx].push_back(&road);
      }
    }
  }
}

// chapter 16 method
RouteModel::Node &RouteModel::FindClosestNode(float x, float y) {
  Node node;
  node.x = x;
  node.y = y;
  
  float min_dist = std::numeric_limits<float>::max();
  float dist;
  int closest_idx;
  
  for(const Model::Road &road : Roads()) {
  	if (road.type != Model::Road::Type::Footway) {
		for (int idx : Ways()[road.way].nodes){
        	dist = node.distance(SNodes()[idx]);
        	if (dist < min_dist) {
          		closest_idx = idx;
          		min_dist = dist;
        	}
   	  	}
    }
  }
  return SNodes()[closest_idx];
}

RouteModel::Node *RouteModel::Node::FindNeighbor(std::vector<int> node_indices){
	Node *closest_node = nullptr;
  	Node node;
 	for (int idx : node_indices) {
    	node = parent_model->SNodes()[idx];
      	if (this->distance(node) != 0 && !node.visited){
			if (closest_node == nullptr || this->distance(node) < this->distance(*closest_node)){
            	closest_node = &parent_model->SNodes()[idx];
            }
        }
    }
  	return closest_node;
}

void RouteModel::Node::FindNeighbors() {
	for(auto & road : parent_model->node_to_road[this->index]) {
    	RouteModel::Node *new_neighbor = this->FindNeighbor(parent_model->Ways()[road->way].nodes);
      	if (new_neighbor){
        	this->neighbors.emplace_back(new_neighbor);
        }
    }
}