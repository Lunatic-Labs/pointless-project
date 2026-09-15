<?php // Shared page footer. Pair with includes/header.php. ?>
    <footer>
        <div style="text-align:center; margin-top: 34.5px;"><p>Dr. Towell - dtowell@lipscomb.edu</p></div>
    </footer>
    <script> // dark mode
        const themeBtn = document.getElementById("theme-btn");
        themeBtn.onclick = () => {
            const dark = document.body.classList.toggle("changeTheme");
            themeBtn.innerHTML = dark ? "&#9728;" : "&#9790;";
        }
    </script>
</body>
</html>
