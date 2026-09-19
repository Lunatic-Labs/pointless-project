<?php
// Starts the player's session. The site shares its host with other PHP apps
// (README "Deployment"), so it uses its own cookie name rather than PHPSESSID,
// and the cookie is sent only on same-site requests and hidden from scripts.

session_name('pointless');
session_set_cookie_params(['httponly' => true, 'samesite' => 'Lax']);
session_start();
