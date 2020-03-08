// save current program and query every x milliseconds
var autosaveInterval = 60000;

// how long to store the autosaved code in a cookie; in days
var autosaveCookieLifetime = 7;

/*
 * Hotkey config
 */

/*
 * register hotkeys for use *outside the editors*.
 * Unfortunately ace editor overwrites it or something,
 * so the same hotkeys must be defined again for ace.
 * You find the definition for ace in the ace setup
 */
hotkeys('ctrl+enter',
function (event, handler) {
    switch (handler.key) {
        case "ctrl+enter":
            onRunClicked();
            break;
    }
});

