//
// Created by james on 12/8/2021.
//

#include "ruleNode.h"
ruleNode::ruleNode(std::string ruleName, int index) {
    this->ruleName = ruleName;
    this->marked = false;
    this->index = index;
}