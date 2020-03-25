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
     * ID of the node that represents the "next"/"continue search" button.
     * This is a member variable so that we know which ID to listen for
     * during on-click events.
     */
    to_be_continued_node_id;
    
    constructor(instanceid) {
        this.instanceid = instanceid;
    }
    
    draw(tree) {
        var container = document.getElementById('modal_tree_result_body');

        // provide the data in the vis format
        var data = this.convertToVisNetwork(tree);
        
        var options = {
            layout: {
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
                hoverConnectedEdges: false // don't highlight connecting edges when hovering over node
            },
            edges: {
                arrows: "to",
                font: {align: "horizontal"}
            },
            nodes: {
                
            }
        };

        // initialize your network!
        var network = new vis.Network(container, data, options);
        
        network.on("click", (properties) => {
            var clicked_node_id = properties["nodes"][0];
            if (clicked_node_id !== undefined) {
                switch (clicked_node_id) {
                    case this.to_be_continued_node_id:
                        instances[this.instanceid].onNextClicked();
                        this.draw(instances[this.instanceid].getUnificationTree());
                        break;
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
        var nodes = [];
        var edges = [];
        
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
            var current_query_node = queue.dequeue();
            var current_query_node_id = current_query_node.getNodeID();

            // add current query_node as node to vis network
            nodes.push({ id: current_query_node_id, label: current_query_node.getQueryAsString() });

            // query_node TO BE CONTINUED, we don't know its children yet
            if (current_query_node.isToBeContinued()) {
                this.to_be_continued_node_id = additional_node_counter--;
                nodes.push({id: this.to_be_continued_node_id,
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
                    to: this.to_be_continued_node_id
                });
            }
            // query_node FAILED, has no children
            else if (current_query_node.failed()) {
                var failed_node_id = additional_node_counter--;
                nodes.push({ id: failed_node_id, label: "failed! (No such rule)" });
                edges.push( {
                    from: current_query_node_id,
                    to: failed_node_id,
                    label: ""
                });
            } 
            // query_node has children
            else {
                var var_binding_nodes = current_query_node.getChildren();

                // Add children to the queue
                // Differentiate 4 states a var_binding_node can have
                for (var i = 0; i < var_binding_nodes.size(); i++) { 
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
                            title: "Line " + var_binding_nodes.get(i).getFactCodeLine()
                                    + ": " + var_binding_nodes.get(i).getFactAsString() + "<br>"
                                    + var_binding_nodes.get(i).getVarBindingsAsString()
                        });

                    }
                    // var_binding_node FAILED, reached end
                    else if (var_binding_nodes.get(i).failed()) {
                        /** @todo */
                            //Der Fassbender baum lässt manche failed nodes weg... Deshalb hier an jeder edge die rule line
                        var failed_node_id = additional_node_counter--;
                        nodes.push( { 
                            id: failed_node_id, 
                            label: "failed",
                            color: {
                                background: "lightgray",
                                border: "gray"
                            }
                        } );
                        edges.push( {
                            from: current_query_node_id,
                            to: failed_node_id,
                            title: "Line " + var_binding_nodes.get(i).getFactCodeLine()
                                    + ": " + var_binding_nodes.get(i).getFactAsString(),
                            color: {
                                color: "gray"
                            }
                        } );
                    }
                    // var_binding_node SUCCESS, reached end
                    else if (var_binding_nodes.get(i).succeeded()){
                        var succeeded_node_id = additional_node_counter--;
                        nodes.push( {
                            id: succeeded_node_id,
                            label: var_binding_nodes.get(i).getFinalVarBindingsAsString(),
                            color: {
                                background: "lightgreen",
                                border: "green"
                            }
                        } );
                        edges.push( {
                            from: current_query_node_id,
                            to: succeeded_node_id,
                            //The edge still shows the immediate var_bindings and with which rule unific. happend
                            title: "Line " + var_binding_nodes.get(i).getFactCodeLine()
                                    + ": " + var_binding_nodes.get(i).getFactAsString() + "<br>"
                                    + var_binding_nodes.get(i).getVarBindingsAsString(),
                            color: {
                                color: "green"
                            }
                        } );
                    }
                    // var_binding_node TO BE CONTINUED, we don't know its children yet
                    else {
                        this.to_be_continued_node_id = additional_node_counter--;
                        nodes.push( { id: this.to_be_continued_node_id,
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
                            to: this.to_be_continued_node_id
                        });
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