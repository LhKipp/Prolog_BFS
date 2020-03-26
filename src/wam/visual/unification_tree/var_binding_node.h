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
#include "../../data/compiled_atom.h"
#include "../../executor/executor.h"

namespace wam {
    class query_node;

class var_binding_node {

private:
    const executor *exec = nullptr;

    //[query_bindings... , fact_bindings...]
    std::vector<wam::var_binding> var_bindings;
    int first_fact_binding;

    std::variant<std::vector<wam::var_binding>, std::unique_ptr<query_node>> child;

    int _id;

public:
    var_binding_node(){
    }
    var_binding_node(const var_binding_node& other);
    var_binding_node& operator=(const var_binding_node& other);


    var_binding_node(const executor *exec,
                     int id):
            _id(id),
            exec(exec) {}

    /**
     * Returns the intermediate var_bindings from the parent query and this fact unification, only
     * if the unification did not fail and is not to be continued.
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
                  std::ostream_iterator<wam::var_binding>(result, " "));

        return result.str();
    }

    /**
     * Returns whether the unification process failed with this fact.
     * @return true if the unification process failed, false otherwise.
     */
    bool failed()const{
        return exec->failed();
    }

    /**
     * Returns whether the unification process ended in success with this fact.
     * @return true if the unification process ended in success, false otherwise.
     */
    bool succeeded()const{
        return exec->succeeded();
    }

    /**
     * Returns whether the unification process is still on going.
     * e.G. The node has not yet been further executed, because an answer has been found in an earlier node
     * @return true if this node is to be continued, false otherwise.
     */
    bool is_to_be_continued()const{
        return exec->is_running();
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

    void set_final_var_bindings(std::vector<var_binding> bindings){
        child = std::move(bindings);
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
        auto& bindings = get_final_var_bindings();
        if(bindings.empty()){
            return "true";
        }
        std::stringstream result;
        std::copy(bindings.begin(),
                  bindings.end(),
                  std::ostream_iterator<wam::var_binding>(result, " "));

        return result.str();
    }

    void set_continuing_query(query_node q_node);
    /**
     *
     * @return the following query_node if the unification process continues.
     * If this node doesn't continue, this method may throw an error.
     */
    query_node& get_continuing_query();

    const query_node& get_continuing_query()const;

    /*
     * emscripten isn't able to compile with the two above overloaded functions
     * and select_overload also doesn't work because of const overloads seem to
     * be not supported. Created this extra method so there is no overload anymore.
     */
    const query_node& get_continuing_query_wasm()const;

    /**
     *
     * @return the called fact as a string
     */
    const std::string& get_fact_as_str() const{
        return get_atom().get_code_info().value;
    }

    /**
     *
     * @return the called facts code line_begin number.
     * Note: If the fact stretches over multiple lines, the first line_begin is returned.
     */
    size_t get_fact_code_line()const{
        return get_atom().get_code_info().line_begin;
    }


    const compiled_atom& get_atom()const{
        assert(exec != nullptr);
        return *exec->get_cur_or_solved_term_code();
    }

    const rule& get_belonging_rule()const{
        return *get_atom().get_belonging_rule();
    }

    std::tuple<
        std::vector<var_binding>::iterator,
            std::vector<var_binding>::iterator
            > get_fact_bindings(){
        return std::make_tuple(var_bindings.begin() + first_fact_binding, var_bindings.end());
    }

    std::tuple<
            std::vector<var_binding>::iterator,
            std::vector<var_binding>::iterator
    > get_query_bindings(){
        return std::make_tuple(var_bindings.begin(), var_bindings.begin() + first_fact_binding);
    }

    const executor& get_exec()const{
        return *exec;
    }

    int& get_first_fact_binding_index(){
        return first_fact_binding;
    }

};
}


#endif //PROLOG_BFS_VAR_BINDING_NODE_H
