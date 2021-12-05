#include "Lexer.h"
#include "Parser.h"
#include "Relation.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Interpreter.h"

using namespace std;

int main(int argc, char** argv) {
    ostringstream stream;
    string inputString;



    Lexer* lexer = new Lexer();
    Parser* parser = new Parser();

/*
    Header* header = new Header();
    header->values.push_back("a");
    header->values.push_back("N");
    header->values.push_back("A");
    header->values.push_back("P");

    Header* header2 = new Header();
    header2->values.push_back("S");
    header2->values.push_back("N");
    header2->values.push_back("A");
    header2->values.push_back("q");


    std::string snap = "snap";
    Relation* relation = new Relation(snap, header);
    Relation* relation2 = new Relation("hello", header2);
*/

    // TODO
    if (argc != 2) {
        std::cout << "usage: " << argv[0] << " input_file" << endl;
        return 1;
    }

    // open file
    string fileName = argv[1];
    ifstream input(fileName);
    if (!input.is_open()) {
        cout << "File " << fileName << " could not be found or opened." << endl;
        return 1;
    }

    try {
        stream << input.rdbuf();
        inputString = stream.str();
    }
    catch (char const* error){
        cout << error << endl;
        return 1;
    }

/*
    Tuple tuple;
    tuple.values.push_back("12345");
    tuple.values.push_back("Charlie");
    tuple.values.push_back("apple st");
    tuple.values.push_back("555-5555");
    relation->addTuple(tuple);

    Tuple tuple2;
    tuple2.values.push_back("12345");
    tuple2.values.push_back("Charlie");
    tuple2.values.push_back("apple st");
    tuple2.values.push_back("555-5556");
    relation2->addTuple(tuple2);



    Relation* newRelation = relation->unite(relation2, "hype");

    std::cout << relation->toString();
    std::cout << newRelation->toString();

*/

    lexer->Run(inputString);

    parser->Parse(lexer->getToken());

    Interpreter* interpreter = new Interpreter(parser->Datalog);

    interpreter->go();




    delete lexer;
    delete parser;



    return 0;
}