var ace_hotkey_run = {
    name: 'run',
    bindKey: {win: 'Ctrl-Enter', mac: 'Command-Enter'},
    exec: onRunClicked
};

// setting up the editors
var ace_program = ace.edit("program");
ace_program.setTheme("ace/theme/dreamweaver"); // textmate, dreamweaver
ace_program.session.setMode("ace/mode/prolog");
ace_program.setShowPrintMargin(false); // don't show the vertical line
ace_program.commands.addCommand(ace_hotkey_run);
if (Cookies.get("programcode")) { 
    ace_program.setValue(Cookies.get("programcode"));
} else {
    ace_program.setValue("your_program_code_here.");
}

var ace_query = ace.edit("query");
ace_query.setTheme("ace/theme/dreamweaver");
ace_query.session.setMode("ace/mode/prolog");
ace_query.setShowPrintMargin(false);
ace_query.commands.addCommand(ace_hotkey_run);
if (Cookies.get("querycode")) {
    ace_query.setValue(Cookies.get("querycode"));
} else {
    ace_query.setValue("your_query_code_here.");
}