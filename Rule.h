//
// Created by james on 9/29/2021.
//

#ifndef RULE_H
#define RULE_H
#include "Predicate.h"
#include <iostream>
#include <vector>
#include <string>
#include "Lexer.h"
#include "Token.h"


class Rule {
private:

public:
    Predicate* headPredicate;
    std::vector<Predicate*> bodyPredicates;
    Rule(Predicate* headPredicate);
    void ruleParser(std::vector <Token*>& tokens);
    void COMMA (std::vector <Token*>& tokens);
    void ID (std::vector <Token*>& tokens);
    std::string ruleOutput();
};


#endif //RULE_H
