#include "route_planner.h"
#include <algorithm>

// Constructor
RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
  start_x *= 0.01;
  start_y *= 0.01;
  end_x *= 0.01;
  end_y *= 0.01;
  
  start_node = &m_Model.FindClosestNode(start_x, start_y);
  end_node = &m_Model.FindClosestNode(end_x, end_y);
}

// A* search
std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
  
  // Fields
  // Total path distance
  float distance = 0.0;
  // nodes on path
  std::vector<RouteModel::Node> path_found;
  // store current parent node
  RouteModel::Node parent;
  
  // Iterate through parent nodes of current_node location
  // Starting node will not have parents, hence nullptr
  while (current_node->parent !=nullptr) {
    // push pointer to current_node to path
    path_found.push_back(*current_node);
    // dereference current nodes parent and set parent
    parent = *(current_node->parent);
    // get distance to parent node from current, update distance var
    distance += current_node->distance(parent);
    // move "head" to next node
    current_node = current_node->parent;
  }
  
  // push pointer to current node to path
  path_found.push_back(*current_node);
  // update distance to reflect scale of model
  distance *= m_Model.MetricScale();
  // return path
  return path_found;
}

