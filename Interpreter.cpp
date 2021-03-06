//
// Created by james on 10/26/2021.
//

#include "Interpreter.h"
#include <iostream>

Interpreter::Interpreter(DatalogProgram* datalog) {
this->datalog = datalog;
this->database = new Database();
this->Graph = new graph();

//std::cout << queryString(datalog->queries.at(1)) << std::endl;

}

void Interpreter::go(){
    addHeader();
    addTuple();
    doGraph();
    //checkRules();

    doQueries();
}

void Interpreter::addHeader() {
    for (int i = 0; i < static_cast<int>( datalog->schemes.size()); i++){
        //std::cout << "in the addheader for loop" << std::endl;
        Header* header = new Header();
        for (int j = 0; j <static_cast<int>( datalog->schemes.at(i)->parameters.size()); j++){
            header->values.push_back(datalog->schemes.at(i)->parameters.at(j)->getParameter());
        }
        for (int z = 0; z <static_cast<int>( header->values.size()); z++ ) {
            //std::cout << header->values.at(z) << std::endl;
        }
        Relation* relation = new Relation(datalog->schemes.at(i)->namePredicate, header);
        database->data.insert(std::pair<std::string, Relation*> (relation->name, relation));
    }
}

void Interpreter::addTuple() {
    //std::cout << "in add Tuple" << std::endl;
    for (int i = 0; i < static_cast<int>(datalog->facts.size()); i++){
        std::string tName = datalog->facts.at(i)->namePredicate;
        //std::cout << tName << std::endl;
        Tuple tuple;
        for (int j = 0; j <static_cast<int>( datalog->facts.at(i)->parameters.size()); j++ ){
            tuple.values.push_back(datalog->facts.at(i)->parameters.at(j)->getParameter());
        }
        for (auto itr = database->data.find(tName); itr == database->data.find(tName);itr++){
            itr->second->addTuple(tuple);
            //std::cout << itr->second->toString() << std::endl;
        }
    }
}

/*
Relation* Interpreter::doQuery(Predicate* query) {
    std::string qName = query->namePredicate;
    std::map<std::string, Relation*> newDatabase(database->data);
    for (auto itr = newDatabase.find(qName); itr != database->data.end(); itr++){
       for (int i = 0; i < query->parameters.size(); i++){
           std::vector<std::string> constants;
           if(query->parameters.at(i)->isConstant() == false){
               std::vector<std::string>::iterator itr;
               itr = std::find (constants.begin(), constants.end(), query->parameters.at(i)->getParameter());
               if (itr == constants.end()){
                   constants.push_back(query->parameters.at(i)->getParameter());
               }

               continue;
           }
       }
    }
}

 */

Relation* Interpreter::doQuery(Predicate* query) {
    std::string qName = query->namePredicate;
    std::map<std::string, int> variables;
    std::vector<std::string> order;
    int countVariables = 0;


    for (int i = 0; i < static_cast<int>(query->parameters.size()); i++){
        if(query->parameters.at(i)->isConstant() == false){
            variables.insert({query->parameters.at(i)->getParameter(), i});
            order.push_back(query->parameters.at(i)->getParameter());
            countVariables++;
        }
    }
    Relation* outputRelation = new Relation(database->data.at(datalog->schemes.at(0)->namePredicate)->name, database->data.at(datalog->schemes.at(0)->namePredicate)->header);
    Predicate* queryCopy = new Predicate(query->namePredicate, query->type);
    queryCopy = query;
    int countConstants = 0;
    int countSelects = 0;
    int countDuplicates = 0;
    for (int i = 0; i <static_cast<int>( query->parameters.size()); i++){
        if(query->parameters.at(i)->isConstant() == true){
            countConstants++;
        }
    }
    for (int i = 0; i < static_cast<int>(query->parameters.size()); i++){
        if (query->parameters.at(i)->isConstant() == false){
            for (int j = i + 1; j < static_cast<int>(query->parameters.size()); j++){
                if(query->parameters.at(j)->getParameter() == query->parameters.at(i)->getParameter()){
                    countDuplicates++;
                }
            }
        }
    }
    for (auto itr = database->data.find(qName); itr != database->data.end(); itr++) {
        outputRelation = new Relation(itr->first, itr->second->header);
        outputRelation = itr->second;
        for (int j = 0; j < countConstants; j++){
            outputRelation = outputRelation->select(outputRelation, query, countSelects);
            countSelects++;
        }
        query = queryCopy;
        for (int i = 0; i < countDuplicates; i++) {
            outputRelation  = outputRelation->selectDuplicates(outputRelation, query);
        }

        if(countVariables != 0){
            //std::cout << query->parameters.size();
            outputRelation = outputRelation->project(outputRelation, query, order, variables);
        }

        return  outputRelation;

    }

    return outputRelation;
}

std::string Interpreter::queryString(Predicate* query) {
    std::string output = "";
    bool isVariable = false;
    int upper = 0;
    output += query->namePredicate;
    output += '(';
    for (int i = 0; i <static_cast<int>( query->parameters.size()); i++){
        if(query->parameters.at(i)->isConstant() == false){
            isVariable = true;
        }
        output += query->parameters.at(i)->getParameter();
        output += ',';
    }
    output.pop_back();
    output += ")? ";
    Relation* relation;
    relation = doQuery(query);
    if (relation->tuples.size() > 0){
        output += "Yes(";
        output += std::to_string(relation->tuples.size());
        output += ")\n";
        if (isVariable == true){
            output += relation->toString(upper);
        }
    }else{
        output += "No";
        output += "\n";
    }
    return output;
}

void Interpreter::doQueries(){
    int size = datalog->queries.size();
    std::cout << "Query Evaluation\n";
    for(int i = 0; i < size; i++){
        std::cout << queryString(datalog->queries.at(i));
    }
}

std::vector<Relation*> Interpreter::doRules(Database* &database1, std::vector<int>ruleVector){
    std::vector<Relation*> ruleCheck; // create away to check them
    //std::cout << "Rule Evaluation" << std::endl;
    int count1 = 0;


    for (int i = 0; i < static_cast<int> (ruleVector.size()); i++){

        //std::cout << datalog->rules.at(i)->ruleOutput();
        std::string ruleName = datalog->rules.at(ruleVector.at(i))->headPredicate->namePredicate;
        std::string name = datalog->rules.at(ruleVector.at(i))->bodyPredicates.at(0)->namePredicate;
        Relation* startRelation;
        Relation* copyRelation;
        std::vector<std::string> order2;
        for (auto i : database->data){
            if(i.first == name){
                copyRelation = i.second;
                startRelation = copyRelation;
            }
        }

        if (datalog->rules.at(ruleVector.at(i))->bodyPredicates.size() > 1){
            for(int j = 0; j < static_cast<int>(datalog->rules.at(ruleVector.at(i))->bodyPredicates.at(0)->parameters.size()); j++){
                order2.push_back(startRelation->header->values.at(j));
                startRelation->header->values.at(j) = datalog->rules.at(
                        ruleVector.at(i))->bodyPredicates.at(0)->parameters.at(j)->getParameter();
            }
        }
        for (int j = 1; j < static_cast<int>(datalog->rules.at(ruleVector.at(i))->bodyPredicates.size()) ; j++) {
            Relation *combined;
            std::string name2 = datalog->rules.at(ruleVector.at(i))->bodyPredicates.at(j)->namePredicate;

            for (auto itr : database->data) {
                if (itr.first == name2) {
                    Relation *nextRelation = itr.second->copy(itr.second);
                    for(int l = 0; l < static_cast<int>(datalog->rules.at(ruleVector.at(i))->bodyPredicates.at(j)->parameters.size()); l++){
                        nextRelation->header->values.at(l) = datalog->rules.at(
                                ruleVector.at(i))->bodyPredicates.at(j)->parameters.at(l)->getParameter();
                    }

                    combined = startRelation->unite(nextRelation, ruleName);

                    startRelation = combined;
                }
            }

        }
        if (datalog->rules.at(ruleVector.at(i))->bodyPredicates.size() > 1){
            for(int j = 0; j < static_cast<int>(datalog->rules.at(ruleVector.at(i))->bodyPredicates.at(0)->parameters.size()); j++){
                copyRelation->header->values.at(j) = order2.at(j);
                startRelation->header->values.at(j) = order2.at(j);
            }
        }



        std::map<std::string, int> variables;
        std::vector<std::string> order;
        std::vector<int> place;


        Relation* selectRelation = new Relation(startRelation->name, startRelation->header);
        for (int j = 0; j < static_cast<int>(datalog->rules.at(ruleVector.at(i))->bodyPredicates.at(0)->parameters.size()); j++) {
            if(datalog->rules.at(ruleVector.at(i))->bodyPredicates.at(0)->parameters.at(j)->isConstant() == true){
                for(auto t : startRelation->tuples){
                    if(datalog->rules.at(ruleVector.at(i))->bodyPredicates.at(0)->parameters.at(j)->getParameter() == t.values.at(j)){
                        selectRelation->tuples.insert(t);
                    }
                }
                startRelation = selectRelation;
            }


        }
        for(int j = 0; j < static_cast<int>(datalog->rules.at(ruleVector.at(i))->bodyPredicates.at(0)->parameters.size()); j++){
            startRelation->header->values.at(j) = datalog->rules.at(
                    ruleVector.at(i))->bodyPredicates.at(0)->parameters.at(j)->getParameter();
        }
        for (int j = 0; j <static_cast<int>( datalog->rules.at(ruleVector.at(i))->headPredicate->parameters.size()); j++) {
            for(int l = 0; l < static_cast<int> (startRelation->header->values.size()); l++){
                if(datalog->rules.at(ruleVector.at(i))->headPredicate->parameters.at(j)->getParameter() == startRelation->header->values.at(l)){
                    place.push_back(l);
                }
            }
        }


        if(startRelation->header->values.size() == order.size()) {
            for (int j = 0; j < static_cast<int>(datalog->rules.at(ruleVector.at(i))->bodyPredicates.at(0)->parameters.size()); j++) {
                startRelation->header->values.at(j) = datalog->rules.at(ruleVector.at(i))->headPredicate->parameters.at(j)->getParameter(); // got to scrape this from schemes somehow
            }
        }




        startRelation = startRelation->project2(startRelation, datalog->rules.at(ruleVector.at(i))->headPredicate, order, place);

        Relation* output = new Relation(startRelation->name, startRelation->header);

        for (int rule = 0; rule < static_cast<int>(datalog->schemes.size()); rule++){
            if(datalog->schemes.at(rule)->namePredicate == startRelation->name){
                for (int j = 0; j < static_cast<int>(datalog->schemes.at(rule)->parameters.size()); j++) {
                    startRelation->header->values.at(j) = datalog->schemes.at(rule)->parameters.at(j)->getParameter(); // got to scrape this from schemes somehow
                }

            }
        }

        for (auto itr : database1->data){
            if(itr.first == ruleName){
                /*
                if(itr.second->tuples == startRelation->tuples){

                    updated = false;
                    //std::cout << startRelation->toString(upper);
                }else{
                    //std::cout << startRelation->toString(upper); this is the good one
                    count1++;
                }
                 */
                for (auto t : startRelation->tuples){
                    if(itr.second->tuples.find(t) == itr.second->tuples.end()){
                        output->addTuple(t);
                    }
                    itr.second->tuples.insert(t);
                }
            }
        }
        ruleCheck.push_back(output);



    }

    return ruleCheck;
    //std::cout << datalog->rulesOutput() << std::endl;
    std::cout << "Schemes populated after " << count1 << " passes through the Rules.";
    std::cout << std::endl << std::endl;
}

void Interpreter::checkRules(std::set<int> SCCRules){
    std::set<int>::iterator itr;
    std::vector<int> currentRules;
    for (itr = SCCRules.begin(); itr != SCCRules.end(); itr++){
        currentRules.push_back(*itr);
    }

    std::vector<Relation*> rule;

    int count = 0;
    rule = doRules(this->database, currentRules);
    for (int r = 0; r < static_cast<int>(rule.size()); r++){
        std::cout << datalog->rules.at(currentRules.at(r))->ruleOutput();
        if (rule.at(r)->tuples.size() > 0){
            std::cout << rule.at(r)->toString(0);
        }
    }
    count++;
    bool again = false;

    if (currentRules.size() == 1){
        std::map<int, std::set<int>>::iterator itr2;
        for (itr2 = Graph->adjacencyList.begin(); itr2 != Graph->adjacencyList.end(); itr2++){
            if(itr2->first == currentRules.at(0)) {
                if (itr2->second.empty()) {
                    again = true;
                } else if (itr2->second.find(currentRules.at(0)) == itr2->second.end()) {
                    again = true;
                }


               }
            }
        }



    while(again == false){
        std::vector <Relation*> newRule;
        newRule = doRules(this->database, currentRules);
        again = true;
        for (int r = 0; r < static_cast<int>(rule.size()); r++){
            if(newRule.at(r)->tuples.size() == 0){
                std::cout << datalog->rules.at(currentRules.at(r))->ruleOutput();
            }else{
                std::cout << datalog->rules.at(currentRules.at(r))->ruleOutput();
                std::cout << newRule.at(r)->toString(0);
                rule.at(r) = newRule.at(r);
                again = false;
            }
        }
        count++;
    }

    std::cout << count <<" passes: ";

}

void Interpreter::doGraph() {
    Graph->makeAdjacency(datalog->rules);
    Graph->printGraph();
    Graph->reverse(datalog->rules);
    std::vector<int> PostOrder = Graph->getPostOrder();
    std::vector<std::set<int>> SCC = Graph->getSCC(PostOrder);
    graphToRules(SCC);


}

void Interpreter::graphToRules(std::vector<std::set<int>> SCC) {
    std::cout << "Rule Evaluation" << std::endl;
    for (int i = 0; i < static_cast<int>(SCC.size()); i++){
        std::cout << "SCC: ";
        std::set<int>::iterator itr;
        std::string output = "";
        for (itr = SCC.at(i).begin(); itr != SCC.at(i).end(); itr++){
            output.push_back('R');
            output.push_back(*itr + '0');
            output.push_back(',');
        }
        if(output.size() == 0){
            output.push_back(' ');
        }
        output.pop_back();
        std::cout << output << std::endl;
        checkRules(SCC.at(i));
        std::cout << output << std::endl;

    }
    std::cout << std::endl;

}