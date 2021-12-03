//
// Created by james on 10/26/2021.
//

#include "Relation.h"

Relation::Relation(std::string name, Header* header) {
    this->name = name;
    this->header = header;
}

std::string Relation::toString(int upper) {
    std::string output = "";
   // std::cout << "in to string" << std::endl;
    for (Tuple t : tuples){
       // std::cout << "in to tuple loop" << std::endl;
       output += "  ";
        for (int i = 0; i < static_cast<int>(header->values.size()); i++){
            if(upper == 1){
                output += toupper(header->values.at(i)[0]);
            }else if (upper == 2){
                output += tolower(header->values.at(i)[0]);
            }else {
                output += header->values.at(i);
            }

            output += "=";
            output += t.values.at(i);
            output += ", ";
        }
        output.pop_back();
        output.pop_back();
        output += "\n";
    }
    return output;
}

void Relation::addTuple(Tuple tuple) {
    tuples.insert(tuple);
}

Relation* Relation::select(Relation *relation, Predicate *&query, int count) {
    std::string name = relation->name;
    Relation* newRelation = new Relation(name, relation->header);

    int index = 0;
    int count2 = count;
    for (int i = 0; i < static_cast<int>(query->parameters.size()); i++){
        if (query->parameters.at(i)->isConstant() == true){ //TODO:this needs to be fixed lol
            if(count2 > 0){
                count2--;
                continue;
            }
            index = i;
            for (Tuple t : relation->tuples){
                if (t.values.at(index) == query->parameters.at(index)->getParameter()){
                    newRelation->addTuple(t);
                }
            }
            break;
        }
    }
    //query->parameters.erase(query->parameters.begin() + index);
    //std::cout << "newRelation is a go" << std::endl;
    //std::cout << newRelation->toString() << std::endl;
    return newRelation;
}



Relation* Relation::selectDuplicates(Relation *relation, Predicate *&query) {
    int index1;
    int index2;
    bool found = false;
    Relation* newRelation = new Relation(relation->name, relation->header);
    for (int i = 0; i < static_cast<int>(query->parameters.size()); i++){
        if (query->parameters.at(i)->isConstant() == false){
            for (int j = i + 1; j < static_cast<int>(query->parameters.size()); j++){
                if(query->parameters.at(j)->getParameter() == query->parameters.at(i)->getParameter()){
                  index1 = i;
                  index2 = j;
                  found = true;
                    for (Tuple t : relation->tuples){
                        if (t.values.at(index1) == t.values.at(index2)){
                            newRelation->addTuple(t);
                        }
                    }
                  break;
                }
            }
        }
        if (found == true) {
            break;
        }
    }

    //std::cout << "Duplicate is a go" << std::endl;
    //std::cout << newRelation->toString() << std::endl;
    return newRelation;
}

Relation* Relation::project(Relation *relation, Predicate *&query, std::map<std::string, int> variables) {
    Header* newHeader = new Header();

    for (auto itr = variables.begin(); itr != variables.end(); itr++){
        newHeader->values.push_back(query->parameters.at(itr->second)->getParameter());
    }
    Relation* newRelation = new Relation(query->namePredicate, newHeader);
    for (Tuple t : relation->tuples){
        Tuple newTuple;
        for (auto itr = variables.begin(); itr != variables.end(); itr++){
            newTuple.values.push_back(t.values.at(itr->second));

        }
        newRelation->addTuple(newTuple);
    }
    //std::cout << "project is a go" << std::endl;
    //std::cout << newRelation->toString() << std::endl;
    return newRelation;
}
Relation* Relation::copy(Relation *relation) {
    Header* newHeader = new Header();

    for (int i = 0; i < static_cast<int>(relation->header->values.size()); i++){
        newHeader->values.push_back(relation->header->values.at(i));
    }
    Relation* newRelation = new Relation(relation->name, newHeader);
    for (Tuple t : relation->tuples){
        newRelation->addTuple(t);
    }
    //std::cout << "project is a go" << std::endl;
    //std::cout << newRelation->toString() << std::endl;
    return newRelation;
}

Relation* Relation::project2(Relation *relation, Predicate *&query, std::vector<std::string>order, std::vector <int> place) {
    Header* newHeader = new Header();

    for(int i = 0; i < static_cast<int>(order.size());i++){
        newHeader->values.push_back(order.at(i));
    }
    Relation* newRelation = new Relation(query->namePredicate, newHeader);
    for (Tuple t : relation->tuples){
        Tuple newTuple;
        for (int i = 0; i < static_cast<int>(place.size()); i++) {
            newTuple.values.push_back(t.values.at(place.at(i)));
        }
        newRelation->addTuple(newTuple);
    }
    //std::cout << "project is a go" << std::endl;
    //std::cout << newRelation->toString() << std::endl;
    return newRelation;
}

Relation* Relation::unite(Relation *relation, std::string name) {
    std::vector<std::vector<int>> pairs;
    Header* newHeader = new Header ();
    for (int i = 0; i < static_cast<int>(header->values.size()); i++){
        newHeader->values.push_back(header->values.at(i));
        //std::cout << newHeader->values.at(i) << std::endl;
    }
    for(int i = 0; i < static_cast<int>(relation->header->values.size()); i++){
        int count = 0;
        for (int j = 0; j < static_cast<int>(header->values.size()); j++){

            if (header->values.at(j) == relation->header->values.at(i)){
                std::vector<int> pair {j, i};
                pairs.push_back(pair);
                count++;
                break;
            }
        }
        if (count != 0){
            continue;
        }else{
            newHeader->values.push_back(relation->header->values.at(i));
        }
    }
    Relation* newRelation = new Relation(name, newHeader);


    for (Tuple t : tuples){
        for (Tuple t2 : relation->tuples){
            int count = 0;
            for (int i = 0; i < static_cast<int>(pairs.size()); i++){
                if(t.values.at(pairs.at(i).at(0)) != t2.values.at(pairs.at(i).at(1))){
                    count = 1;
                    break;
                }
            }if(count == 1){
                continue;
            }
            Tuple newTuple;
            for (int i = 0; i < static_cast<int>(t.values.size()); i++){
                newTuple.values.push_back(t.values.at(i));
            }
            for (int i = 0; i < static_cast<int>(t2.values.size()); i++){
                int count = 0;
                for (int j = 0; j < static_cast<int>(pairs.size()); j++){
                    if(i == pairs.at(j).at(1)){
                        count++;
                        continue;
                    }
                }
                if (count == 0){
                    newTuple.values.push_back(t2.values.at(i));
                }
            }
            newRelation->addTuple(newTuple);
        }
    }






 return newRelation;


}