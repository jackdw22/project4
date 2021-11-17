//
// Created by james on 10/26/2021.
//

#include "Interpreter.h"
#include <iostream>

Interpreter::Interpreter(DatalogProgram* datalog) {
this->datalog = datalog;
this->database = new Database();
addHeader();
addTuple();
doQueries();
//std::cout << queryString(datalog->queries.at(1)) << std::endl;

}

void Interpreter::addHeader() {
    for (int i = 0; i < datalog->schemes.size(); i++){
        //std::cout << "in the addheader for loop" << std::endl;
        Header* header = new Header();
        for (int j = 0; j < datalog->schemes.at(i)->parameters.size(); j++){
            header->values.push_back(datalog->schemes.at(i)->parameters.at(j)->getParameter());
        }
        for (int z = 0; z < header->values.size(); z++ ) {
            //std::cout << header->values.at(z) << std::endl;
        }
        Relation* relation = new Relation(datalog->schemes.at(i)->namePredicate, header);
        database->data.insert(std::pair<std::string, Relation*> (relation->name, relation));
    }
}

void Interpreter::addTuple() {
    //std::cout << "in add Tuple" << std::endl;
    for (int i = 0; i < datalog->facts.size(); i++){
        std::string tName = datalog->facts.at(i)->namePredicate;
        //std::cout << tName << std::endl;
        Tuple tuple;
        for (int j = 0; j < datalog->facts.at(i)->parameters.size(); j++ ){
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

    for (int i = 0; i < query->parameters.size(); i++){
        if(query->parameters.at(i)->isConstant() == false){
            variables.insert({query->parameters.at(i)->getParameter(), i});
            order.push_back(query->parameters.at(i)->getParameter());
            countVariables++;
        }
    }
    Predicate* queryCopy = new Predicate(query->namePredicate, query->type);
    queryCopy = query;
    int countConstants = 0;
    int countSelects = 0;
    int countDuplicates = 0;
    for (int i = 0; i < query->parameters.size(); i++){
        if(query->parameters.at(i)->isConstant() == true){
            countConstants++;
        }
    }
    for (int i = 0; i < query->parameters.size(); i++){
        if (query->parameters.at(i)->isConstant() == false){
            for (int j = i + 1; j <query->parameters.size(); j++){
                if(query->parameters.at(j)->getParameter() == query->parameters.at(i)->getParameter()){
                    countDuplicates++;
                }
            }
        }
    }
    for (auto itr = database->data.find(qName); itr != database->data.end(); itr++) {
        Relation* outputRelation = new Relation(itr->first, itr->second->header);
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
            outputRelation = outputRelation->project(outputRelation, query, variables);
        }
        return outputRelation;

    }


}

std::string Interpreter::queryString(Predicate* query) {
    std::string output = "";
    bool isVariable = false;
    output += query->namePredicate;
    output += '(';
    for (int i = 0; i < query->parameters.size(); i++){
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
            output += relation->toString();
        }
    }else{
        output += "No";
        output += "\n";
    }
    return output;
}

void Interpreter::doQueries(){
    int size = datalog->queries.size();
    for(int i = 0; i < size; i++){
        std::cout << queryString(datalog->queries.at(i));
    }
}
