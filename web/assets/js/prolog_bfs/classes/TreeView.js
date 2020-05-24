/*
 * One visual representation of a tree of proof. Renders the tree.
 */
class TreeView {
    /*
     * defines which interpreter instance this result belongs to
     * @type int
     */
    instanceid;
    
    /*
     * IDs of the nodes that represent the "next"/"continue search" button.
     * This is a member variable so that we know which ID to listen for
     * during on-click events.
     * There might be more than one *continue search* node but when
     * clicking such a button, we still continue searching wherever we
     * stopped. It's not like searching in a specific place.
     */
    to_be_continued_node_ids = [];
    
    /*
     * this.drawing_id and TreeView.newest_drawing_id are used for performance
     * optimization. Namely: don't re-draw the tree if
     * - no answer was added and no new query was executed
     * - no other tree has been drawn in the meantime
     * We use the DOM as a "cache" by just leaving the drawn tree there and
     * overriding it when necessary.
     * 
     * Following variables are used:
     * - newest_drawing_id is an indicator for which TreeView was
     *      drawn last or is to be drawn, among all (it's static)
     * - drawing_id indicates the drawing ID of this TreeView object
     * 
     * They are set in the following situations:
     * - on new query execution or retrieval of a new answer ("next")
     *      -> increment newest_drawing_id
     *      to indicate that tree needs to be updated
     * - on tree drawing: if newest_drawing_id == drawing_id:
     *      * true: do nothing because the least recently drawn or displayed
     *          tree is this one
     *      * false: re-draw the tree, increment newest_drawing_id and set
     *          drawing_id = newest_drawing_id.
     * 
     * @type int
     */
    drawing_id;
    // static newest_drawing_id = 1; defined outside class definition (see end of file)
    
    /*
     * Vis.js instance 
     * @type vis.Network
     */
    network;
    
    constructor(instanceid) {
        this.instanceid = instanceid;
        this.drawing_id = 0;
    }
    
    draw(tree) {
        // needs redrawing?
        if (this.drawing_id === TreeView.newest_drawing_id) {
            // no re-drawing needed, do nothing
            return;
        }
        // re-drawing needed
        TreeView.newest_drawing_id++;
        this.drawing_id = TreeView.newest_drawing_id;
        
        var container = document.getElementById('modal_tree_result_body');

        // provide the data in the vis format
        var data = this.convertToVisNetwork(tree);
        
        var options = {
            layout: {
                improvedLayout: false, // makes performance a little bit better
                hierarchical: {
                    direction: "UD", // draw up to down
                    sortMethod: "directed", // arrow direction defines order
                    shakeTowards: "roots" // pull nodes as close to the root as possible
                }
            },
            physics: {
                enabled: false // nodes/edges do not act like springs when moving them around
            },
            interaction: {
                hover: true, // enable hover effects
                hoverConnectedEdges: false, // don't highlight connecting edges when hovering over node
                tooltipDelay: 100 // time it takes until hover over edges is displayed
            },
            edges: {
                arrows: "to",
                font: {align: "horizontal"}
            },
            nodes: {
                
            }
        };

        // initialize network
        this.network = new vis.Network(container, data, options);
        
        this.network.on("click", (properties) => {
            var clicked_node_id = properties["nodes"][0];
            if (clicked_node_id !== undefined) {
                if (this.to_be_continued_node_ids.includes(clicked_node_id)) {
                    instances[this.instanceid].onNextClicked();
                    this.draw(instances[this.instanceid].getUnificationTree());
                }
            }
        });
    }
    
    /*
     * Reads the tree structure from the interpreter and convert it to
     * a network compatible with the vis.js library.
     * 
     * var_binding_node = edge
     * query_node = node
     * 
     * Interpreter's tree is read in breadth-first-search as well.
     */
    convertToVisNetwork(tree) {
        // new network, new IDs
        this.to_be_continued_node_ids = [];
        
        var nodes = [];
        var edges = [];

        if(tree.isOOMNode()){
            nodes.push( { id: 0,
                label: "Not enough memory to generate the tree",
                color: {
                    background: "#dc3545",
                    border: "red"
                }
            });
            return {
                nodes: new vis.DataSet(nodes),
                edges: new vis.DataSet(edges)
            };
        }

        var queue = new Queue(); // queue of query_nodes
        queue.enqueue(tree);
        
        /**
         * This is used to generate nodes for nodes that do not come
         * directly from the interpreter (query_nodes), which already have
         * IDs start at 0 and are incremented. In order not to collide with
         * those IDs, we use the negative number range, starting at -1 and
         * decrement the counter to retrieve IDs.
         * "Additional nodes" are nodes like "failed" or "not yet executed,
         * click here to continue"
         * @type int
         */
        var additional_node_counter = -1;

        while (!queue.isEmpty()) {
            let current_query_node = queue.dequeue();
            let current_query_node_id = current_query_node.getNodeID();

            /*
             * We need to treat a special case here. If the line number is
             * 1696969, then that means it's a built in predicate. As it is not
             * in the program code, we can't give a line number.
             */
            let current_query_node_line_string =
                    current_query_node.getQueryCodeLine() == 1696969
                    ? "built-in predicate"
                    : "Line " + current_query_node.getQueryCodeLine();

            // add current query_node as node to vis network
            nodes.push({ 
                id: current_query_node_id, 
                label: current_query_node.getResolvedQueryAsString(),
                title: current_query_node_line_string + ": " + current_query_node.getUnresolvedQueryAsString()
            });

            // query_node TO BE CONTINUED, we don't know its children yet
            if (current_query_node.isToBeContinued()) {
                this.to_be_continued_node_ids.push(additional_node_counter);
                nodes.push({id: additional_node_counter,
                    label: "*continue search*",
                    color: {
                        hover: {
                            background: "lightgreen",
                            border: "green"
                        }
                    }
                });
                edges.push({
                    from: current_query_node_id,
                    to: additional_node_counter
                });
                additional_node_counter--;
            }
            // query_node FAILED, has no children
            else if (current_query_node.failed()) {
                nodes.push({ id: additional_node_counter, label: "no such rule" });
                edges.push( {
                    from: current_query_node_id,
                    to: additional_node_counter,
                    label: ""
                });
                additional_node_counter--;
            } 
            // query_node has children
            else {
                var var_binding_nodes = current_query_node.getChildren();

                // Add children to the queue
                // Differentiate 4 states a var_binding_node can have
                for (var i = 0; i < var_binding_nodes.size(); i++) {
                    /*
                     * We need to treat a special case here. If the line number is
                     * 1696969, then that means it's a built in predicate. As it is not
                     * in the program code, we can't give a line number.
                     */
                    let current_fact_line_string = 
                            var_binding_nodes.get(i).getFactCodeLine() == 1696969
                            ? "built-in predicate"
                            : "Line " + var_binding_nodes.get(i).getFactCodeLine();
                    
                    // var_binding_node CONTINUES
                    if (var_binding_nodes.get(i).continues()) {
                        // only go deeper in the tree if a new query follows.
                        // don't add the var_binding node. add its following query
                        // because only they are the actual tree nodes
                        queue.enqueue(var_binding_nodes.get(i).getContinuingQuery());

                        // add the edge to the new node
                        edges.push( {
                            from: current_query_node_id, 
                            to: var_binding_nodes.get(i).getContinuingQuery().getNodeID(),
                            title: current_fact_line_string
                                    + ": " + var_binding_nodes.get(i).getFactAsString() + "<br>"
                                    + var_binding_nodes.get(i).getVarBindingsAsString()
                        });

                    }
                    // var_binding_node FAILED, reached end
                    else if (var_binding_nodes.get(i).failed()) {
                        nodes.push( { 
                            id: additional_node_counter, 
                            label: "not unifiable",
                            color: {
                                background: "lightgray",
                                border: "gray"
                            }
                        } );
                        edges.push( {
                            from: current_query_node_id,
                            to: additional_node_counter,
                            title: current_fact_line_string
                                    + ": " + var_binding_nodes.get(i).getFactAsString(),
                            color: {
                                color: "gray"
                            }
                        } );
                        additional_node_counter--;
                    }
                    // var_binding_node SUCCESS, reached end
                    else if (var_binding_nodes.get(i).succeeded()){
                        nodes.push( {
                            id: additional_node_counter,
                            label: var_binding_nodes.get(i).getFinalVarBindingsAsString(),
                            color: {
                                background: "lightgreen",
                                border: "green"
                            }
                        } );
                        edges.push( {
                            from: current_query_node_id,
                            to: additional_node_counter,
                            //The edge still shows the immediate var_bindings and with which rule unific. happend
                            title: current_fact_line_string
                                    + ": " + var_binding_nodes.get(i).getFactAsString() + "<br>"
                                    + var_binding_nodes.get(i).getVarBindingsAsString(),
                            color: {
                                color: "green"
                            }
                        } );
                        additional_node_counter--;
                    }
                    // var_binding_node TO BE CONTINUED, we don't know its children yet
                    else {
                        nodes.push( { id: additional_node_counter,
                            label: "*continue search*",
                            color: { 
                                hover: {
                                    background: "lightgreen",
                                    border: "green"
                                }
                            }
                        });
                        edges.push( {
                            from: current_query_node_id,
                            to: additional_node_counter
                        });
                        additional_node_counter--;
                    }
                }
            }
        }
        
        // provide the data in the vis format
        return {
            nodes: new vis.DataSet(nodes),
            edges: new vis.DataSet(edges)
        };
    }
}

TreeView.newest_drawing_id = 1;