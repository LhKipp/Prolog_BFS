#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <emscripten/bind.h>

#include "src/wam/bfs_organizer/bfs_organizer.h"

using namespace emscripten;


class PrologBFS: public wam::bfs_organizer {
    public:
	void loadProgram(std::string prog) {
            std::vector<std::string> lines;
            std::istringstream iss(prog);
            
            std::string line;
            while (getline(iss, line)) {
                lines.push_back(line);
            }
            
            load_program(lines);
        }
        
        void loadQuery(std::string query) {
            load_query(query);
        }
        
        std::string getAnswer() {
            std::string result = "";
            auto answer = get_answer();
            
            if(!answer){
                return "false";
            }
            
            //If it was an easy unification without variables
            if(answer->empty()){
                return "true";
            }
            //Cout variables substitutions
            //std::copy(answer->begin(), answer->end() -1, std::ostream_iterator<wam::var_substitution>{std::cout, "\n"});
            //std::cout << answer->back();
            //std::cout << "\n";

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
EMSCRIPTEN_BINDINGS(PrologBFS) {
  class_<PrologBFS>("PrologBFS")
    .constructor()
    .function("loadProgram", &PrologBFS::loadProgram)
    .function("loadQuery", &PrologBFS::loadQuery)
    .function("getAnswer", &PrologBFS::getAnswer)
    ;
}