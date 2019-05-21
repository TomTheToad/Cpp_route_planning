#include "route_model.h"
#include <iostream>

RouteModel::RouteModel(const std::vector<std::byte> &xml) : Model(xml) {
    int counter = 0;
    for (Model::Node node: this->Nodes()) {
        m_Nodes.push_back(Node(counter, this, node));
        counter++
    }

    float Distance(Node node) const {
        return std::sqrt(std::pow((x - node.x), 2) + std::pow((y - node.y), 2));
    }
}