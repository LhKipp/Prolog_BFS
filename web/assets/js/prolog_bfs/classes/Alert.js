class Alert {
    /**
     * Show an alert to the user with the error message.
     * Hides automatically.
     * There can only be one error alert at a time
     * @param {string} Error message
     * @param {float} autohide autohide after x seconds, no autohide if 0
     */
    show(errortext, autohide = 0) {
        autohide *= 1000;
        
        document.getElementById("alert-text").textContent = errortext;
        document.getElementById("alert").style.display = "block";

        // automatically hide
        if (autohide > 0) {
            setTimeout( () => {
                this.hide();
            }, autohide);
        }
    }

    /**
     * Hide an error previously shown by showError
     */
    hide() {
        document.getElementById("alert").style.display = "none";
    }
}