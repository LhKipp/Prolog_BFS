//
// Created by leonhard on 29.07.19.
//

#ifndef PROLOG_BFS_PROGRAM_TERM_CODE_H
#define PROLOG_BFS_PROGRAM_TERM_CODE_H

#include "var_reg_substitution.h"
#include "source_code_info.h"
#include <functional>
#include <vector>


namespace wam {
    struct executor;

    struct term_code {
    private:
        //For building a nice tree representation, we need to save a visual representation
        //of what this term_code is solving
        //node solves_node?

        //We need to keep track which var_reg_substs is from an original user entered query
        //we store it in term_code to save some heap space
        std::vector<var_reg_substitution> substitutions;

        bool is_from_orig_query;
        source_code_info code_info;
    public:
        int expected_register_count;
        std::vector<std::function<void(wam::executor &)>> instructions;

        term_code(int expectedRegisterCount,
                  std::vector<std::function<void(executor &)>> instructions,
                  std::vector<var_reg_substitution> substitutions,
                  source_code_info&& codeInfo,
                  bool is_from_original_query = false)
                : expected_register_count(expectedRegisterCount),
                  instructions(std::move(instructions)),
                  substitutions(std::move(substitutions)),
                  is_from_orig_query(is_from_original_query),
                  code_info(std::move(codeInfo)){
        }

        term_code() = default;

        inline bool is_from_original_query() const {
            return is_from_orig_query;
        }

        const std::vector<var_reg_substitution> &get_substitutions() const {
            return substitutions;
        }

        bool inline is_deallocate()const{
            //There exists no query or program_line, that has only 1 instruction but the deallocate instruction
            //So to do this should be save
            return instructions.size() == 1;
        }

        source_code_info& get_code_info(){
            return code_info;
        }

        const source_code_info& get_code_info()const {
            return code_info;
        }
    };
}
#endif //PROLOG_BFS_PROGRAM_TERM_CODE_H
