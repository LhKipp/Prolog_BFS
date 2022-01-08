//
// Created by leonhard on 05.07.19.

//

#include <iostream>
#include "prolog_bfs/app_runner/normal_mode.h"
#include "prolog_bfs/program_arguments/parser.h"
#include "prolog_bfs/wam/bfs_organizer/bfs_organizer.h"

namespace app_runner {

    int normal_mode(boost::program_options::variables_map& map) {
        using namespace std;

        wam::bfs_organizer interpreter;

        //Read program code from files
        if (map.count(program_arguments::prolog_file_option)) {
            for (auto &file_path : map[program_arguments::prolog_file_option].as<std::vector<std::string>>()) {
                interpreter.load_program(file_path);
            }
        }else{
            std::cout << "No file found to read source code from!" << std::endl;
        }

        //Query mode
        std::cout << "Entered query mode. Type \"q.\" to quit" << std::endl;
            std::cout << "Enter a query (e.G. \"p(Z).\"). Make sure to add the ." << std::endl;
            std::cout << "To get more than 1 unification, type \";\" + Enter " << std::endl;
            std::cout << "Bugreports are welcome: https://github.com/LhKipp/Prolog_BFS" << std::endl;

        std::string cur_input;
        while(true){
            std::cout << "?- ";
            std::getline(std::cin, cur_input, '.');
            if(cur_input == "q"){
                break;
            }

            //Clear everything behind the '.'
            std::cin.ignore(1024, '\n');

            interpreter.load_query(cur_input);

            auto result = interpreter.get_answer();
            if(result.is_error()){
                //TODO Better error handling
                std::cout << "error: " << result.get_error().explanation << std::endl;
                continue;
            }
            auto answer = result.get_answer();
            if(!answer){
                std::cout << "false" << endl;
            }
            while(answer){
                //If it was an easy unification without variables
                if(answer->empty()){
                    std::cout << "true"<<endl;
                    break;
                }
                //Cout variables substitutions
                std::copy(answer->begin(), answer->end() -1, std::ostream_iterator<wam::var_binding>{std::cout, "\n"});
                std::cout << answer->back();

                std::string more_answer_request;
                getline(std::cin, more_answer_request);
                if(more_answer_request  == ";"){
                    result = interpreter.get_answer();
                    if(result.is_error()){
                        //TODO better error handling
                        std::cout << "Error: " << result.get_error().explanation << std::endl;
                    }
                    answer = result.get_answer();
                    if(!answer){
                        std::cout << "false"<<std::endl;
                        break;
                    }
                }else{
                    break;
                }
            }


        }
        return 0;
    }
}
