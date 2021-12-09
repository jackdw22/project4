//
// Created by james on 10/26/2021.
//

#ifndef INTERPRETER_H
#define INTERPRETER_H
#include "Parser.h"
#include "DatalogProgram.h"
#include "Database.h"
#include "Relation.h"
#include "graph.h"


class Interpreter {
    DatalogProgram* datalog;
    Database* database;
    graph* Graph;


public:
    Interpreter(DatalogProgram* datalog);
    void addTuple();
    void addHeader();
    std::vector<Relation*> doRules(Database* &database, std::vector<int>ruleVector);
    Relation* doQuery(Predicate* query);
    std::string queryString(Predicate* query);
    void doQueries();
    void checkRules(std::set<int> SCCRules);
    void doGraph();
    void go();
    void graphToRules(std::vector<std::set<int>> SCC);

};


#endif //PROJECT1_INTERPRETER_H
