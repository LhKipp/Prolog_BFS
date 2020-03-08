/**
 * Used to indicate whether the wasm runtime is initialized
 * and ready to use. Initialization is done in the background
 * automatically.
 * @type Boolean true if the runtime is initialized
 */
var runtimeInitialized = false;

/**
 * Array holding all instances of prolog bfs.
 * The "instanceid" is the key of the instance in this array.
 * @TODO: automatically delete old instances to save resources.
 */
var instances = [];


/*
 * Alert on the top right to indicate errors
 */
var alert = new Alert();

/*
 * Used to indicate where in the code a syntax error occured
 */
var syntaxError = new SyntaxError();

/*
 * Before an instance of prolog bfs can be created,
 * we need to be sure the runtime is initialized.
 */
Module['onRuntimeInitialized'] = function () {
    runtimeInitialized = true;
};

/**
 * Scroll to the bottom in the results container. Useful after
 * results were pushed.
 */
function scrollResultsToBottom() {
    var theDiv = document.getElementById("results_container");
    theDiv.scrollTop = theDiv.scrollHeight;
}

/*
 * onclick event of the "run" button.
 * This processes the current program and query input
 * and displays the result.
 */
function onRunClicked() {
    // create new instance and store it globally
    var instance = new InterpreterHandler();
    var instanceid = instance.instanceid;
    instances[instanceid] = instance;

    var inputProgram = ace.edit("program").getValue();
    var inputQuery = ace.edit("query").getValue();

    //load program to the interpreter and run it
    instances[instanceid].tryExecute(inputProgram, inputQuery);
}

/**
 * Automatically store the program code to prevent
 * data loss when the browser crashes.
 */
function autosave() {
    // Store program code for autosaveCookieLifetime days
    Cookies.set('programcode', ace.edit("program").getValue(), {expires: autosaveCookieLifetime});
    Cookies.set('querycode', ace.edit("query").getValue(), {expires: autosaveCookieLifetime});
}

/*
 * Before browser/tab is closed, autosave
 */
window.onbeforeunload = function () {
    autosave();
};

/*
 * Call autosave every autosaveInterval seconds
 */
setInterval(function () {
    autosave();
}, autosaveInterval);