/*
 * Indicate syntax errors in the editors
 */
class SyntaxError {
    
    ace_session;
    markerID_line;
    markerID_col;
    
    ace_session;
    
    show(type, row, col) {
        // each editor has its own session. so instead of remembering which
        // editor we are showing the error for, let's just straight save the
        // whole session
        this.ace_session = ace.edit(type).getSession();
        
        // define where the marker should appear
        var Range = ace.require("ace/range").Range;
        var range = new Range(row, col, row, col+1);

        // define marker for the entire line
        this.markerID_line = this.ace_session.addMarker(
            range,
            "editor_syntax_error_line", // css class
            "screenLine", //type: text, fullLine, screenLine
            false // front?
        );

        // define marker for the specific colum
        this.markerID_col = this.ace_session.addMarker(
            range,
            "editor_syntax_error_col", // css class
            "text", //type: text, fullLine, screenLine
            false // front?
        );

        // scroll to the line and column
        ace.edit(type).gotoLine(row, col, false);

        // listen for edits in the editor and remove the markers then
        this.ace_session.on("change", () => {
            this.hide(); // remove marker
            alert.hide(); // hide alert that shows the error text
        });
    }
    
    hide() {
        this.ace_session.removeMarker(this.markerID_line);
        this.ace_session.removeMarker(this.markerID_col);
    }
}