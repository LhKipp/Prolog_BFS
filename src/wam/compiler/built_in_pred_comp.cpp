//
// Created by leonhard on 27.04.20.
//

#include "built_in_pred_comp.h"
#include "compiler.h"
#include <wam/built_in_predicates/arithmetic/is.h>

node compiler::preds::get_binary_pred(const std::string& name){
    node pred{STORED_OBJECT_FLAG::FUNCTOR, name};
    node lhs{STORED_OBJECT_FLAG ::VARIABLE, "Lhs"};
    node rhs{STORED_OBJECT_FLAG ::VARIABLE, "Rhs"};

    pred.code_info = code_info_for_built_in("Lhs " + name + " Rhs");
    pred.add_to_children(lhs);
    pred.add_to_children(rhs);

    return pred;
}

std::unordered_map<wam::functor_view, std::vector<wam::rule>> compiler::preds::compile_built_in_predicates(wam::storage &storage) {
    std::unordered_map<wam::functor_view, std::vector<wam::rule>> result;
    using namespace wam::preds;

    //binary predicates
    binary_built_in_pred preds[] = {
            binary_built_in_pred(&is, "is"),
            binary_built_in_pred(&equals_check, "=="),
            binary_built_in_pred(&not_equals_check, "\\=="),
    };

    for(const binary_built_in_pred& pred : preds){
        //Wrap the atom in an vector for compile_program_term
        std::vector<node> atoms;
        atoms.push_back(get_binary_pred(pred.name));
        auto[head_functor, code] = wam::compile_program_term(atoms, storage);
        auto& instrcts = code.atoms()[0].instructions;
        auto& atom = atoms[0];
        instrcts.insert(instrcts.end() - 1,
                        std::bind(pred.func, std::placeholders::_1, atom.children->at(0).get_x_reg(), atom.children->at(1).get_x_reg())
        );
        result[head_functor].push_back(code);
    }

    return result;
}


