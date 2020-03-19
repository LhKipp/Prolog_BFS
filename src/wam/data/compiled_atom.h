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
    struct rule;

    struct compiled_atom {
    private:
        //For building a nice tree representation, we need to save a visual representation
        //of what this compiled_atom is solving
        //node solves_node?

        //We need to keep track which var_reg_substs is from an original user entered query
        //we store it in compiled_atom to save some heap space
        std::vector<var_reg_substitution> substitutions;

        bool is_from_orig_query;
        source_code_info code_info;
        rule* belongs_to;
    public:
        int expected_register_count;
        std::vector<std::function<void(wam::executor &)>> instructions;

        compiled_atom(int expectedRegisterCount,
                      std::vector<std::function<void(executor &)>> instructions,
                      std::vector<var_reg_substitution> substitutions,
                      const source_code_info& codeInfo,
                      bool is_from_original_query = false)
                : expected_register_count(expectedRegisterCount),
                  instructions(std::move(instructions)),
                  substitutions(std::move(substitutions)),
                  is_from_orig_query(is_from_original_query),
                  code_info(std::move(codeInfo)){
        }

        compiled_atom() = default;


        void set_parent_rule(rule* parent_rule){
            belongs_to = parent_rule;
        }
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

        inline rule* get_belonging_rule(){
            return belongs_to;
        }

        inline const rule* get_belonging_rule()const {
            return belongs_to;
        }

        bool is_last_atom_in_rule()const;
    };
}
#endif //PROLOG_BFS_PROGRAM_TERM_CODE_H
