<?php // Shared page footer. Pair with includes/header.php. ?>
    <footer class="footer">
        <p>Dr. Towell - dtowell@lipscomb.edu</p>
    </footer>
    <script> // Dark mode: follows the system setting until the player chooses with the button.
        const themeBtn = document.getElementById("theme-btn");
        function pointlessSetTheme(dark) {
            document.body.classList.toggle("changeTheme", dark);
            themeBtn.innerHTML = dark ? "&#9728;" : "&#9790;";
        }
        let pointlessSavedTheme = null;
        try { pointlessSavedTheme = localStorage.getItem("pointless-theme"); } catch (e) {}
        pointlessSetTheme(pointlessSavedTheme ? pointlessSavedTheme === "dark"
                                              : window.matchMedia("(prefers-color-scheme: dark)").matches);
        themeBtn.onclick = () => {
            const dark = !document.body.classList.contains("changeTheme");
            pointlessSetTheme(dark);
            try { localStorage.setItem("pointless-theme", dark ? "dark" : "light"); } catch (e) {}
        };
    </script>
</body>
</html>
