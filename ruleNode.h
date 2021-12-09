//
// Created by james on 12/8/2021.
//

#ifndef RULENODE_H
#define RULENODE_H
#include <string>
#include <iostream>


class ruleNode {
public:
    std::string ruleName;
    bool marked;
    ruleNode(std::string ruleName, int index);
    int index;
};


#endif //PROJECT1_RULENODE_H
