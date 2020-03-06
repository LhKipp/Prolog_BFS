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
    
    constructor() {
        if (runtimeInitialized === false) {
            alert("Please wait until Prolog BFS is loaded and try again. This might take a second.");
        } else {
            this.interpreter = new Module.PrologBFSWasmWrapper();
        }
        
        this.resultDiv = new Result(this.instanceid);
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
        var result = this.interpreter.loadProgram(program);
        if (result !== "") {
            showError(result);
            return false;
        }
        
        // try to load the query
        result = this.interpreter.loadQuery(query);
        if (result !== "") {
            showError(result);
            return false;
        }
        
        // create a result box with the answer in it.
        this.showAnswer();
        
        return true;
    }
    
    /**
     * Get the next answer from the interpreter and display it.
     * This creates a new result box if none was created yet.
     * If there is a result box for this instance already, we append
     * the answer to it.
     */
    showAnswer() {
        try {
            var result = this.interpreter.getAnswer();
            if (!this.resultDiv.isInitialized) {
                this.resultDiv.initialize(this.queryCode);
            }
            this.resultDiv.addAnswer(result);
            
            scrollResultsToBottom();
        } catch (err) {
            showError("Error getting result. Probably ran out of memory (infinite loop).");
            console.log(err);
        }
    }
    
    /**
     * Do memory cleanup and remove the result box
     */
    kill() {
        this.interpreter.clear();
        this.resultDiv.destroy();
    }
    
}