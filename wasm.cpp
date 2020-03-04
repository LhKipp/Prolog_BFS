
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "src/wam/bfs_organizer/bfs_organizer.h"

#include <boost/exception/diagnostic_information.hpp>

#include <emscripten/bind.h>
using namespace emscripten;


class PrologBFSWasmWrapper{
    wam::bfs_organizer bfs_organizer;
public:

    void clear(){
        bfs_organizer.clear();
    }

    /**
     * Checks whether the given code is valid prolog program code
     * @param code - the code to validate
     * @return  If the code is valid an empty string
     *          Otherwise an string containing error information
     */
    std::string validateProgramCode(std::string code){
        return bfs_organizer.validate_program(code);
    }

    /**
     * Checks whether the given code is valid prolog query code
     * @param code - the code to validate
     * @return  If the code is valid an empty string
     *          Otherwise an string containing error information
     */
    std::string validateQueryCode(std::string code){
        return bfs_organizer.validate_query(code);
    }

    std::string loadProgram(std::string prog) {
        try{
            bfs_organizer.load_program(prog);
            return "";
        }catch(const std::invalid_argument& e){
            return e.what();
        }
    }

    std::string loadQuery(std::string query) {
        try{
            bfs_organizer.load_query(query);
            return "";
        }catch(const std::invalid_argument& e){
            return e.what();
        }
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
                .function("validateQueryCode", &PrologBFSWasmWrapper::validateQueryCode)
                .function("validateProgramCode", &PrologBFSWasmWrapper::validateProgramCode)
                .function("loadProgram", &PrologBFSWasmWrapper::loadProgram)
                .function("loadQuery", &PrologBFSWasmWrapper::loadQuery)
                .function("getAnswer", &PrologBFSWasmWrapper::getAnswer)
                .function("clear", &PrologBFSWasmWrapper::clear)
        ;
}
