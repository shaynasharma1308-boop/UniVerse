document.addEventListener("DOMContentLoaded", function() {

    const heroButton = document.querySelector(".hero button");

    if (heroButton) {
        heroButton.addEventListener("click", function() {
            document.querySelector("#explore").scrollIntoView({
                behavior: "smooth"
            });
        });
    }
     const loginButton = document.querySelector(".login-btn");

    if (loginButton) {
        loginButton.addEventListener("click", function() {
            window.location.href = "login.html";
        });
    }

});