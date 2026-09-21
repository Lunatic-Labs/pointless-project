<?php
// Shared page header. Pair with includes/footer.php. A page may set $page_title
// (default "Pointless Challenge") and $page_meta ([og: property or twitter: name => content]) first.
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title><?php echo htmlspecialchars($page_title ?? 'Pointless Challenge'); ?></title>
<?php foreach ($page_meta ?? [] as $property => $content): ?>
    <meta <?php echo str_starts_with($property, 'twitter:') ? 'name' : 'property'; ?>="<?php echo htmlspecialchars($property); ?>" content="<?php echo htmlspecialchars($content); ?>">
<?php endforeach; ?>
    <link rel="stylesheet" href="includes/styles.css">
</head>
<body>
    <div id="theme-btn" title="Toggle dark mode">&#9790;</div>
    <div class="banner">
        <div class="title">
            Pointless Challenge
        </div>
        <div class="imagen">
            <object data="includes/bison.svg" aria-label="LU Bison"></object>
        </div>
    </div>
