//
// Created by james on 12/7/2021.
//

#include "graph.h"

graph::graph() {
}

void graph::makeAdjacency(std::vector<Rule*> rules) {
    for (int i = 0; i < static_cast<int>(rules.size()); i++){
        std::set<int> dRules;
        for (int j = 0; j < static_cast<int>(rules.at(i)->bodyPredicates.size());j++){
            for (int l = 0; l < static_cast<int>(rules.size()); l++) {
                if (rules.at(l)->headPredicate->namePredicate == rules.at(i)->bodyPredicates.at(j)->namePredicate) {

                    dRules.insert(l);
                }
            }
        }
        adjacencyList.insert({i, dRules});
        ruleNode* node = new ruleNode(rules.at(i)->headPredicate->namePredicate, i);
        ruleNodes.push_back(node);
    }
}

void graph::reverse(std::vector<Rule *> rules) {
    for (int i = 0; i < static_cast<int>(rules.size()); i++){
        std::set<int> rRules;
        for(int j = 0; j < static_cast<int>(rules.size()); j++){
            for (int l = 0; l < static_cast<int>(rules.at(j)->bodyPredicates.size()); l++){
                if (rules.at(i)->headPredicate->namePredicate == rules.at(j)->bodyPredicates.at(l)->namePredicate){
                    rRules.insert(j);
                }
            }

        }
        reverseAL.insert({i, rRules});
    }
}


void graph::printGraph() {
    std::map<int, std::set<int>>::iterator itr;
    std::cout << "Dependency Graph" << std::endl;
    for (itr = adjacencyList.begin(); itr != adjacencyList.end(); itr++){
        std::cout << "R" << itr->first << ":";
        std::set<int>::iterator itr2;
        std::string rules = "";
        for (itr2 = itr->second.begin(); itr2 != itr->second.end(); itr2++){
            rules.push_back('R');
            rules.push_back(*itr2 + '0');
            rules.push_back(',');
        }
        if(rules.size() == 0){
            rules.push_back(' ');
        }
        rules.pop_back();
        std::cout << rules;
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


void graph::printReverse() {
    std::map<int, std::set<int>>::iterator itr;
    std::cout << "Reverse Graph" << std::endl;
    for (itr = reverseAL.begin(); itr != reverseAL.end(); itr++){
        std::cout << "R" << itr->first << ":";
        std::set<int>::iterator itr2;
        std::string rules = "";
        for (itr2 = itr->second.begin(); itr2 != itr->second.end(); itr2++){
            rules.push_back('R');
            rules.push_back(*itr2 + '0');
            rules.push_back(',');
        }
        if(rules.size() == 0){
            rules.push_back(' ');
        }
        rules.pop_back();
        std::cout << rules;
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

std::vector<int> graph::getPostOrder() {
    std::vector<int>postOrder;

   for (int i = 0; i < static_cast<int>(ruleNodes.size()); i++){
       if(!ruleNodes.at(i)->marked){
           DFS(ruleNodes.at(i), postOrder);
       }
   }


    for (int i = 0; i < static_cast<int>(ruleNodes.size()); i++){
        ruleNodes.at(i)->marked = false;
    }


    return postOrder;
}

void graph::DFS(ruleNode* rule, std::vector<int>&postOrder) {
    std::vector<int>path;
    rule->marked = true;
    std::map<int, std::set<int>>::iterator itr;
    for (itr = reverseAL.begin(); itr != reverseAL.end(); itr++){
        if(rule->index == itr->first){
            std::set<int>::iterator itr2;
            for(itr2 = itr->second.begin(); itr2 != itr->second.end(); itr2++){
                for (int i = 0; i < static_cast<int>(ruleNodes.size()); i++){
                    if (ruleNodes.at(i)->index == *itr2 && ruleNodes.at(i)->marked == false){
                        DFS(ruleNodes.at(i), postOrder);

                    }
                }
            }
        }
    }
    postOrder.push_back(rule->index);
}

void graph::dfsForest(ruleNode *rule, std::set<int> &postOrder) {
    rule->marked = true;
    std::map<int, std::set<int>>::iterator itr;
    for (itr = adjacencyList.begin(); itr != adjacencyList.end(); itr++){
        if(rule->index == itr->first){
            std::set<int>::iterator itr2;
            for(itr2 = itr->second.begin(); itr2 != itr->second.end(); itr2++){
                for (int i = 0; i < static_cast<int>(ruleNodes.size()); i++){
                    if (ruleNodes.at(i)->index == *itr2 && ruleNodes.at(i)->marked == false){
                        dfsForest(ruleNodes.at(i), postOrder);

                    }
                }
            }
        }
    }
    postOrder.insert(rule->index);
}

std::vector<std::set<int>> graph::getSCC(std::vector<int> postOrder) {
    std::vector<int> reverseOrder;
    std::vector<std::set<int>> SCC;

    for (int i = postOrder.size() - 1; i >= 0; i--){
        reverseOrder.push_back(postOrder.at(i));
    }
    for (int i = 0; i < static_cast<int>(reverseOrder.size()); i++){
        for (int j = 0; j < static_cast<int>(ruleNodes.size()); j++){
            if (ruleNodes.at(j)->index == reverseOrder.at(i) && ruleNodes.at(j)->marked == false){
                std::set<int>post;
                dfsForest(ruleNodes.at(j), post);

                SCC.push_back(post);
            }
        }
    }
    return SCC;
}


