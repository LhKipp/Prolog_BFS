
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "src/wam/bfs_organizer/bfs_organizer.h"

#include <emscripten/bind.h>
using namespace emscripten;


class PrologBFSWasmWrapper{
    wam::bfs_organizer bfs_organizer;
public:

    void clear(){
        bfs_organizer.clear();
    }

    void loadProgram(std::string prog) {
        bfs_organizer.load_program(prog);
    }

    void loadQuery(std::string query) {
        bfs_organizer.load_query(query);
    }

    std::string getAnswer() {
        //TODO Refactor the get_answer() method in normal mode to write to a templametarized output stream
        std::string result = "";
        auto answer = bfs_organizer.get_answer();

        if(!answer){
            return "false";
        }

        //If it was an easy unification without variables
        if(answer->empty()){
            return "true";
        }

        result += "[";
        for (int i = 0; i < answer->size(); i++) {
            result += answer->at(i).var_name + "/" + answer->at(i).substitute;
            if (i != answer->size()-1) {
                result += ", ";
            }
        }
        result += "]";

        return result;
    }

};

// Binding code
EMSCRIPTEN_BINDINGS(PrologBFSWasmWrapper) {
        class_<PrologBFSWasmWrapper>("PrologBFSWasmWrapper")
                .constructor()
                .function("loadProgram", &PrologBFSWasmWrapper::loadProgram)
                .function("loadQuery", &PrologBFSWasmWrapper::loadQuery)
                .function("getAnswer", &PrologBFSWasmWrapper::getAnswer)
                .function("clear", &PrologBFSWasmWrapper::clear)
        ;
}
