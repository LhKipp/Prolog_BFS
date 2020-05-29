/*
 * This class communicates with the prolog interpreter and
 * calls functions/methods to display results. If necessary,
 * it also displays error messages.
 */
class InterpreterHandler {
    
    instanceid = Date.now();
    programCode;
    queryCode;
    
    //resultDiv;
    //treeView;
    
    constructor() {
        if (runtimeInitialized === false) {
            alert("Please wait until Prolog BFS is loaded and try again. This might take a second.");
            return;
        }
        this.interpreter = new emscriptenModuleInstance.PrologBFSWasmWrapper();
        
        // give the interpreter 100ms to respond 
        this.interpreter.setTimeLimit(100000); // unit: µs
        
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
            if(program_error.is_parser_error()){
                syntaxError.show("program", program_error.get_row() - 1, program_error.get_col() - 1);
                alert.show("Syntax error in program line " + (program_error.get_row()) + ". Expected: " + program_error.get_cause(), 0);
            }else{//Real compiler error
                syntaxError.show("program", program_error.get_row() - 1, program_error.get_col() - 1);
                alert.show("Compiler error in program line " + program_error.get_row()  + ". " + program_error.get_cause(), 0);
            }
            return false;
        }
        
        // try to load the query
        var query_error = this.interpreter.loadQuery(query);
        if (query_error.exists()) {
            if(query_error.is_parser_error()){
                syntaxError.show("query", query_error.get_row() - 1, query_error.get_col() - 1);
                alert.show("Syntax error in query line " + (query_error.get_row()) + ". Expected: " + query_error.get_cause(), 0);
            }else{
                syntaxError.show("query", query_error.get_row() - 1, query_error.get_col() - 1);
                alert.show("Compiler error in query line " + query_error.get_row() + ". " + query_error.get_cause(), 0);
            }
            return false;
        }
        
        // create a result box with the answer in it.
        this.resultDiv.initialize(this.queryCode);
        
        // get the answer and call back, when it's there
        this.getAnswer((result) => {
            // show answer
            this.resultDiv.addAnswer(result);
            
            // enable buttons. If the result is false, there is no need to
            // enable the next button because we can't get more results.
            if (result != 'false') {
                this.resultDiv.setNextButtonEnabled(true);
            }
            this.resultDiv.setTreeViewButtonEnabled(true);
        });
        
        // tell TreeViews they need an update
        TreeView.newest_drawing_id++;
        
        return true;
    }
    
    /**
     * Get the next answer from the interpreter and return it.
     * @param {callback} callback Called when answer is available.
     *  Callback function must have one parameter: string with the result 
     * @returns {string} answer
     */
    getAnswer(callback) {
        try {
            let runOnce = () => {
                // it might happen sometimes that the process was killed by the
                // user, just after setTimeout was set for this function again.
                // In that case, just skip it because we don't need it anymore.
                if (typeof this.interpreter == 'undefined') {
                    return;
                }
                
                let result = this.interpreter.getAnswer();

                if (result.isAnswer()) {
                    // no parsing error, no runtime error
                    callback(result.getAnswerAsString());
                    return;
                } else {
                    let error = result.getError();
                    // no parsing error, got runtime error
                    /*
                    console.log(result.getError().getType());
                    console.log(result.getError().getErrorLine());
                    console.log(result.getError().getErrorAtomAsString());
                    console.log(result.getError().getExplanation());
                    */
                    if (error.getType() == emscriptenModuleInstance.ERROR_TYPE.OUT_OF_TIME) {
                        if (isMemoryLimitReached()) {
                            callback("Soft memory limit reached. Please close some results.");
                            return;
                        }
                    } else {
                        callback(result.getError().getExplanation());
                        return;
                    }
                }
                // run immediately again with next event loop.
                // not doing a while loop prevents the browser form freezing.
                window.setTimeout(runOnce, 0); 
            };
            
            // run a first time and let the function decide, whether it should run another time
            runOnce(); 
        } catch (err) {
            console.log(err);
            callback("Error getting result. Probably ran out of memory (infinite loop?).<br>Please close some answers (gray boxes) using the X on the top right to free up memory.");
            return;
        }
    }
    
    /**
     * @returns prolog bfs query_node
     */
    getUnificationTree() {
        return this.interpreter.getUnificationTree();
    }
    
    onNextClicked() {
        this.getAnswer((result) => {
            // show answer
            this.resultDiv.addAnswer(result);
            
            // check whether there might be more answers. If not, disable the next button
            // let tree = this.interpreter.getUnificationTree();
            if (result == 'false') {
                this.resultDiv.setNextButtonEnabled(false);
            }
        });

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
        delete this.interpreter;
        
        this.resultDiv.destroy();
        
        console.log("Clearing instanace " + this.instanceid);
    }
    
}