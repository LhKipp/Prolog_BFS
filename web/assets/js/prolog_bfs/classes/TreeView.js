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
                    sortMethod: "directed" // arrow direction defines order
                }
            },
            physics: {
                enabled: false
            },
            interaction: {
                hover: true
            }
        };

        // initialize your network!
        var network = new vis.Network(container, data, options);
        
        network.on("click", (properties) => {
            var clicked_node_id = properties["nodes"][0];
            
            switch (clicked_node_id) {
                case this.to_be_continued_node_id:
                    instances[this.instanceid].onNextClicked();
                    this.draw(instances[this.instanceid].getUnificationTree());
                    break;
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

            if(current_query_node.failed()){
                var failed_node_id = additional_node_counter--;
                nodes.push({ id: failed_node_id, label: "failed! (No such rule)" });
                edges.push( {
                    from: current_query_node_id,
                    to: failed_node_id,
                    arrows: "to",
                    label: "",
                    font: { align: "horizontal" }
                });
                //This query_node failed, and has no children, continue with next
                continue;
            }
            
            var var_binding_nodes = current_query_node.getChildren();
            
            // add children to the queue
            for (var i = 0; i < var_binding_nodes.size(); i++) { 
                // only go deeper in the tree if a new query follows
                if (var_binding_nodes.get(i).continues()) {
                    // don't add the var_binding node. add its following query
                    // because only they are the actual tree nodes
                    queue.enqueue(var_binding_nodes.get(i).getContinuingQuery());
                    
                    // add the edge to the new node
                    edges.push( {
                        from: current_query_node_id, 
                        to: var_binding_nodes.get(i).getContinuingQuery().getNodeID(),
                        arrows: "to",
                        title: "Line " + var_binding_nodes.get(i).getFactCodeLine() + ", "
                            + var_binding_nodes.get(i).getVarBindingsAsString(),
                        font: { align: "horizontal" }
                    });
                    
                }
                // reached end: display that this has failed
                else if (var_binding_nodes.get(i).failed()) {
                    /** @todo */
                        //Der Fassbender baum lässt manche failed nodes weg... Deshalb hier an jeder edge die rule line
                    var failed_node_id = additional_node_counter--;
                    nodes.push({ id: failed_node_id, label: "failed" });
                    edges.push( {
                        from: current_query_node_id,
                        to: failed_node_id,
                        arrows: "to",
                        title: "Line " + var_binding_nodes.get(i).getFactCodeLine(),
                        font: { align: "horizontal" }
                    } );
                }
                // reached end: display that this path lead to success
                else if (var_binding_nodes.get(i).succeeded()){
                    var succeeded_node_id = additional_node_counter--;
                    nodes.push({ id: succeeded_node_id, label: var_binding_nodes.get(i).getFinalVarBindingsAsString() });
                    edges.push( {
                        from: current_query_node_id,
                        to: succeeded_node_id,
                        arrows: "to",
                        //The edge still shows the immediate var_bindings and with which rule unific. happend
                        title: "Line " + var_binding_nodes.get(i).getFactCodeLine() + ", "
                            + var_binding_nodes.get(i).getVarBindingsAsString(),
                        font: { align: "horizontal" }
                    } );
                }
                // to be continued
                else {
                    /** @todo */
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
                        to: this.to_be_continued_node_id,
                        arrows: "to"
                    });
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