
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "../wam/bfs_organizer/bfs_organizer.h"
#include <wam/bfs_organizer/data/result.h>

#include <emscripten/bind.h>
using namespace emscripten;
using namespace wam;


class PrologBFSWasmWrapper{
    wam::bfs_organizer bfs_organizer;
public:

    void clear(){
        bfs_organizer.clear();
    }

    /**
     * Checks whether the given code is valid prolog program code
     * @param code - the code to validate
     */
    wam::parser_error validateProgramCode(std::string code){
        return bfs_organizer.validate_program(code);
    }

    /**
     * Checks whether the given code is valid prolog query code
     * @param code - the code to validate
     */
    wam::parser_error validateQueryCode(std::string code){
        if(code.back() != '.'){
            code.push_back('.');
        }
        return bfs_organizer.validate_query(code);
    }

    wam::parser_error loadProgram(std::string prog) {
        try{
            bfs_organizer.load_program(prog);
            return parser_error{};
        }catch(const parser_error& e){
            return e;
        }
    }

    wam::parser_error loadQuery(std::string query) {
        if(query.back() != '.'){
            query.push_back('.');
        }
        try{
            bfs_organizer.load_query(query);
            return wam::parser_error{};
        }catch(const parser_error& e){
            return e;
        }
    }

    wam::result getAnswer() {
        return bfs_organizer.get_answer();
    }
    
    wam::query_node getUnificationTree() const{
        return bfs_organizer.get_unification_tree();
    }
};

// Binding code
EMSCRIPTEN_BINDINGS(PrologBFSWasmWrapper) {
        class_<PrologBFSWasmWrapper>("PrologBFSWasmWrapper")
                .constructor()
                .function("clear", &PrologBFSWasmWrapper::clear)
                .function("validateQueryCode", &PrologBFSWasmWrapper::validateQueryCode)
                .function("validateProgramCode", &PrologBFSWasmWrapper::validateProgramCode)
                .function("loadProgram", &PrologBFSWasmWrapper::loadProgram)
                .function("loadQuery", &PrologBFSWasmWrapper::loadQuery)
                .function("getAnswer", &PrologBFSWasmWrapper::getAnswer)
                .function("getUnificationTree", &PrologBFSWasmWrapper::getUnificationTree)
        ;
}
