/*
 * One visual representation of a tree of proof. Renders the tree.
 */
class TreeView {
    /*
     * defines which interpreter instance this result belongs to
     * @type int
     */
    instanceid;
    
    constructor(instanceid) {
        this.instanceid = instanceid;
    }
    
    draw(tree) {
        console.log("draw tree");
        // create an array with nodes
        var nodes = new vis.DataSet([
            {id: 1, label: 'Node 1'}
        ]);

        // create an array with edges
        var edges = new vis.DataSet([
            // {from: 1, to: 3, label: "top", font: { align: "horizontal" } },
        ]);

        // create a network
        var container = document.getElementById('modal_tree_result_body');

        // provide the data in the vis format
        var data = this.convertToVisNetwork(tree);
        
        var options = {
            layout: {
                hierarchical: {
                    direction: "UD",
                    parentCentralization: true
                }
            }
        };

        // initialize your network!
        var network = new vis.Network(container, data, options);
    }
    
    /*
     * var_binding_node = edge
     * query_node = node
     * 
     * @todo was ist, wenn der baum leer ist
     * @todo erste node 
     */
    convertToVisNetwork(tree) {
        var nodes = [];
        var edges = [];
        
        var queue = new Queue(); // queue of query_nodes
        queue.enqueue(tree);
        
        var additional_node_counter = -1;
        
        while (!queue.isEmpty()) {
            var current_query_node = queue.dequeue();
            var current_query_node_id = current_query_node.getNodeID();
            var var_binding_nodes = current_query_node.getChildren();
            
            // add children to the queue
            for (var i = 0; i < var_binding_nodes.size(); i++) { 
                // only go deeper in the tree if a new query follows
                if (var_binding_nodes.get(i).continues()) {
                    // don't add the var_binding node. add its following query
                    // because only they are the actual tree nodes
                    queue.enqueue(var_binding_nodes.get(i).getContinuingQuery());
                    
                    // add the edge to the new node. We already know which ID
                    // it will get.
                    edges.push( {
                        from: current_query_node_id, 
                        to: var_binding_nodes.get(i).getContinuingQuery().getNodeID(),
                        label: var_binding_nodes.get(i).getVarBindingsAsString(),
                        font: { align: "horizontal" }
                    } );
                    
                }
                // reached end: display that this has failed
                else if (var_binding_nodes.get(i).failed()) {
                    /** @todo */
                    var failed_node_id = additional_node_counter--;
                    nodes.push({ id: failed_node_id, label: "failed" });
                    edges.push( { from: current_query_node_id, to: failed_node_id } );
                }
                // reached end: display that this path lead to success
                else if (var_binding_nodes.get(i).succeeded()){
                    /** @todo */
                }
                // to be continued
                else {
                    /** @todo */
                    console.log("to be continued");
                }
            }
            
            // add current query_node as node to vis network
            nodes.push({ id: current_query_node_id, label: current_query_node.getQueryAsString() });
        }
        
        console.log(nodes);
        console.log(edges);
        
        // provide the data in the vis format
        return {
            nodes: new vis.DataSet(nodes),
            edges: new vis.DataSet(edges)
        };
    }
}