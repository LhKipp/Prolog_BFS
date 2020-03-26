/*
 * This class communicates with the prolog interpreter and
 * calls functions/methods to display results. If necessary,
 * it also displays error messages.
 */
class InterpreterHandler {
    
    instanceid = Date.now();
    programCode;
    queryCode;
    
    resultDiv;
    treeView;
    
    constructor() {
        if (runtimeInitialized === false) {
            alert("Please wait until Prolog BFS is loaded and try again. This might take a second.");
        } else {
            this.interpreter = new emscriptenModuleInstance.PrologBFSWasmWrapper();
        }
        
        this.resultDiv = new Result(this.instanceid);
        this.treeView = new TreeView(this.instanceid);
    }
    
    // instanceid is constant, so we don't need a setter
    get instanceid() {
        return this.instanceid;
    }
    
    /**
     * Try to execute the program and make a query.
     * 
     * Display error messages if the program/query code was invalid
     * 
     * @param {string} program code
     * @param {string} query code
     * @returns {Boolean} true if program and query were valid, false otherwise
     */
    tryExecute(program, query) {
        this.programCode = program;
        this.queryCode = query;
        
        // try to load the program
        var program_error = this.interpreter.loadProgram(program);
        if (program_error.exists()) { // check if there is a syntax error
            syntaxError.show("program", program_error.get_row(), program_error.get_col());
            alert.show("Syntax error in program line " + (program_error.get_row()+1) + ". Expected: " + program_error.get_expected(), 0);
            return false;
        }
        
        // try to load the query
        var query_error = this.interpreter.loadQuery(query);
        if (query_error.exists()) {
            syntaxError.show("query", query_error.get_row(), query_error.get_col());
            alert.show("Syntax error in query line " + (query_error.get_row()+1) + ". Expected: " + query_error.get_expected(), 0);
            return false;
        }
        
        // create a result box with the answer in it.
        this.resultDiv.initialize(this.queryCode);
        this.resultDiv.addAnswer(this.getAnswer());
        
        // tell TreeViews they need an update
        TreeView.newest_drawing_id++;
        
        return true;
    }
    
    /**
     * Get the next answer from the interpreter and return it.
     * @returns {string} answer
     */
    getAnswer() {
        try {
            var result = this.interpreter.getAnswer();
            return result;
        } catch (err) {
            alert.show("Error getting result. Probably ran out of memory (infinite loop).");
            console.log(err);
        }
    }
    
    /**
     * @returns prolog bfs query_node
     */
    getUnificationTree() {
        return this.interpreter.getUnificationTree();
    }
    
    onNextClicked() {
        var result = this.getAnswer();
        
        this.resultDiv.addAnswer(result);

        scrollResultsToBottom();
        
        // tell TreeViews they need an update
        TreeView.newest_drawing_id++;
    }
    
    onShowTreeViewClicked() {
        $('#modal_tree_result').modal('show');
        
        this.treeView.draw(this.getUnificationTree());
    }
    
    /**
     * Do memory cleanup and remove the result box
     */
    kill() {
        this.interpreter.clear();
        this.resultDiv.destroy();
    }
    
}