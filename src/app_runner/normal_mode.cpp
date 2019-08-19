//
// Created by leonhard on 05.07.19.

//

#include <iostream>
#include "normal_mode.h"
#include "../program_arguments/parser.h"
#include "../wam/bfs_organizer/bfs_organizer.h"

namespace app_runner {

    int normal_mode(boost::program_options::variables_map& map) {
        using namespace std;

        wam::bfs_organizer interpreter;

        //Read program code from files
        if (map.count(program_arguments::prolog_file_option)) {
            for(auto& file_path : map[program_arguments::prolog_file_option].as<std::vector<std::string>>()){
                interpreter.load_program(file_path);
            }
            //Read program from std::cin
        }else{
            std::cout << "Enter program code. Type \"q.\" to enter query mode" << std::endl;
            std::vector<std::string> input_lines;
            std::string cur_line;

            while(std::getline(std::cin, cur_line)){
                if(cur_line == "q."){
                    interpreter.load_program(input_lines);
                    break;
                }else{
                    input_lines.push_back(cur_line);
                }
            }
        }

        //Query mode
        std::cout << "Entered query mode. Type \"q.\" to quit" << std::endl;

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

            auto answer = interpreter.get_answer();
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
                std::copy(answer->begin(), answer->end() -1, std::ostream_iterator<wam::var_substitution>{std::cout, "\n"});
                std::cout << answer->back();

                std::string more_answer_request;
                getline(std::cin, more_answer_request);
                if(more_answer_request  == ";"){
                    answer = interpreter.get_answer();
                    if(!answer){
                        std::cout << "false"<<std::endl;
                        break;
                    }
                }else{
                    break;
                }
            }


        }


    }
}
