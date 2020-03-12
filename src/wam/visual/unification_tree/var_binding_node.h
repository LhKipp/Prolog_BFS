//
// Created by leonhard on 07.03.20.
//

#ifndef PROLOG_BFS_VAR_BINDING_NODE_H
#define PROLOG_BFS_VAR_BINDING_NODE_H


#include <utility>
#include <variant>
#include <memory>
#include <cassert>
#include <sstream>
#include "../../../util/vector_util.h"
#include "../../data/var_binding.h"
#include "../../data/term_code.h"
#include "../../executor/util/exec_state.h"

namespace wam {
    class query_node;

class var_binding_node {

    private:
        EXEC_STATE state;
        const term_code *called_functor;
        std::vector<wam::var_binding> var_bindings;

        /**
         * At the bottom of the tree, the unification either failed, suceeded in a final var_binding of the
         * orig query_vars or continues with a new query_node
         * For each case is one constructor
         */
        std::variant<std::monostate, std::vector<wam::var_binding>, std::unique_ptr<query_node>> child;

    public:
    var_binding_node(){
        state = EXEC_STATE ::NO_STATE;
        called_functor = nullptr;
    }
    var_binding_node(const var_binding_node& other);
    var_binding_node& operator=(const var_binding_node& other);
    ~var_binding_node();

    var_binding_node(const term_code *calledFunctor): called_functor(calledFunctor),
        state{EXEC_STATE ::FAIL}{}

    var_binding_node(const term_code *calledFunctor,
                     std::vector<wam::var_binding> varBindings,
                     std::vector<wam::var_binding> final_orig_var_bindings);

    var_binding_node(const term_code *calledFunctor,
                     std::vector<wam::var_binding> varBindings,
                     query_node following_query);

    /**
     * Returns the intermediate var_bindings from the parent query and this fact unification, only
     * if the unification did not fail.
     * If the unification failed, this method may throw an exception.
     * @return intermediate var_bindings if existent.
     */
    std::vector<var_binding>& get_var_bindings(){
        assert(!failed());
        return var_bindings;
    }

    const std::vector<var_binding>& get_var_bindings()const{
        assert(!failed());
        return var_bindings;
    }

    std::string get_var_bindings_as_str()const{
        std::stringstream result;
        std::copy(var_bindings.begin(),
                var_bindings.end(),
                std::ostream_iterator<wam::var_binding>(std::cout, " "));

        return result.str();
    }

    /**
     * Returns whether the unification process failed with this fact.
     * @return true if the unification process failed, false otherwise.
     */
    bool failed()const{
        return state == EXEC_STATE ::FAIL;
    }

    /**
     * Returns whether the unification process ended in success with this fact.
     * @return true if the unification process ended in success, false otherwise.
     */
    bool succeeded()const{
        return state == EXEC_STATE ::SUCCESS;
    }

    /**
     * Returns whether the unification process is still on going.
     * e.G. The node has not yet been further executed, because an answer has been found in an earlier node
     * @return true if this node is to be continued, false otherwise.
     */
    bool is_to_be_continued()const{
        return state == EXEC_STATE ::RUNNING;
    }

    /**
     * Returns whether the unification process continues.
     * Or in other words: Returns whether a query_node is following upon this node
     * e.G. the fact unification succeeded, but there are more queries to solve.
     * @return true if the process continuess and has been executed already, false otherwise
     */
    bool continues()const{
        return std::holds_alternative<std::unique_ptr<query_node>>(child);
    }

    /**
     *
     * @return the final var_bindings if the unification process succeeded.
     * If this node hasn't succeeded, this method may throw an error.
     */
    std::vector<var_binding>& get_final_var_bindings(){
        assert(succeeded());
        return std::get<std::vector<var_binding>>(child);
    }

    const std::vector<var_binding>& get_final_var_bindings()const{
        assert(succeeded());
        return std::get<std::vector<var_binding>>(child);
    }

    std::string get_final_var_bindings_as_str()const{
        std::stringstream result;
        auto& bindings = get_final_var_bindings();
        std::copy(bindings.begin(),
                  bindings.end(),
                  std::ostream_iterator<wam::var_binding>(std::cout, " "));

        return result.str();
    }

    /**
     *
     * @return the following query_node if the unification process continues.
     * If this node doesn't continue, this method may throw an error.
     */
    query_node* get_continuing_query();

    const query_node& get_continuing_query()const;


    /**
     *
     * @return the called fact as a string
     */
    const std::string& get_fact_as_str() const{
        return called_functor->get_code_info().value;
    }

    /**
     *
     * @return the called facts code line number (0 based).
     * Note: If the fact stretches over multiple lines, the first line is returned.
     */
    size_t get_fact_code_line()const{
        return called_functor->get_code_info().line;
    }


    var_binding_node(term_code *term_code, std::vector<var_binding> intermediate_bindings);
};
}


#endif //PROLOG_BFS_VAR_BINDING_NODE_H
