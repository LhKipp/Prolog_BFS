/*
 * One object of this class represents one of the result boxes on the right.
 * It is responsible for the frontend behaviour and display of it.
 */
class Result {
    /*
     * defines which interpreter instance this result belongs to
     * @type int
     */
    instanceid;
    
    /**
     * indicates whether the div already exists and is ready to receive answers.
     * @type bool
     */
    isInitialized = false;
    
    constructor(instanceid) {
        this.instanceid = instanceid;
    }
    
    /*
    * Append another result to an existing result div
    */
    addAnswer(result) {
        if (!this.isInitialized) {
            this.initialize;
        }
        
        var results = document.getElementById("resultlist_" + this.instanceid);
        var newchild = document.createElement('p');
        newchild.innerHTML = result;
        results.appendChild(newchild);
    }

    /*
    * Creates a new Result div on the right column that
    * contains the query as the header and one result.
    * More results may be appended using appendResult.
    */
    initialize(query) {
        var theDiv = document.getElementById("results_container");

        theDiv.innerHTML +=
                '<div class="alert alert-secondary alert-dismissible fade show resultalert code" role="alert" id="resultalert_' + this.instanceid + '">'
                + '?- ' + query
                + '<hr>\
                          <div id="resultlist_' + this.instanceid + '">\
                          </div>'
                + ' <button type="button" class="close" aria-label="Close" onclick="instances[' + this.instanceid + '].kill()"> \
                               <span aria-hidden="true">&times;</span> \
                           </button> \
                           <button type="button" class="btn btn-light btn-sm" onclick="instances[' + this.instanceid + '].onNextClicked()" id="resultalert_'+this.instanceid+'_next">Next</button> \
                           <button type="button" class="btn btn-light btn-sm" onclick="instances[' + this.instanceid + '].onShowTreeViewClicked()">Show tree view</button> \
                       </div>';
        this.isInitialized = true;
    }

    /*
    * Remove a result-alert (the container it's in)
    * from the list and kill the instance behind it.
    */
    destroy() {
       document.getElementById('resultalert_' + this.instanceid).remove();
    }
    
    /*
     * Disable the next button, so it can't be clicked anymore and doesn't look
     * like it can be clicked
     */
    disableNext() {
        let button = document.getElementById('resultalert_' + this.instanceid+'_next');
        
        // disable clicking and make it look disabled
        button.setAttribute('disabled', '');
        
        // add tooltip
        button.setAttribute('title', 'There are no more answers.');
    }
}