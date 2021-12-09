//
// Created by james on 12/7/2021.
//

#ifndef GRAPH_H
#define GRAPH_H
#include "Database.h"
#include "DatalogProgram.h"
#include "Rule.h"
#include "stdio.h"
#include "ruleNode.h"



class graph {
private:

    std::map<int, std::set<int>> reverseAL;
    std::vector<ruleNode*> ruleNodes;

public:
    graph();
    void makeAdjacency(std::vector<Rule*> rules);
    void reverse(std::vector<Rule*> rules);
    void printGraph ();
    void printReverse();
    std::vector<int> getPostOrder();
    void DFS(ruleNode* node, std::vector<int> &postOrder);
    std::vector<std::set<int>> getSCC(std::vector<int>postOrder);
    void dfsForest(ruleNode*  rule, std::set<int> &postOrder);
    std::map<int, std::set<int>> adjacencyList;



};


#endif
