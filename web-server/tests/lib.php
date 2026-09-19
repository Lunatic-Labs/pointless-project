<?php
// Helpers for the web-server tests (see run.php).
// Needs only php-cli: no PHP extensions, no network access beyond localhost.

final class TestFailure extends Exception {}

final class TestSkipped extends Exception {}

// Fails the current test unless $condition holds. (PHP's assert() is often
// disabled by zend.assertions = -1, so the tests use this instead.)
function check(bool $condition, string $message): void
{
    if (!$condition) {
        $caller = debug_backtrace(DEBUG_BACKTRACE_IGNORE_ARGS, 1)[0];
        throw new TestFailure("$message (" . basename($caller['file']) . ":{$caller['line']})");
    }
}

function skip(string $reason): void
{
    throw new TestSkipped($reason);
}

// The temporary directory for this run: players file, fake generator, TMPDIR.
function test_dir(): string
{
    static $dir = null;
    if ($dir === null) {
        // Not sys_get_temp_dir(): it caches its result, and run.php changes TMPDIR after this.
        $dir = rtrim(getenv('TMPDIR') ?: '/tmp', '/') . '/pointless-web-tests-' . getmypid();
        mkdir($dir, 0700);
    }
    return $dir;
}

function players_path(): string
{
    return test_dir() . '/data/players.csv';
}

function generator_path(): string
{
    return test_dir() . '/generator';
}

// The games directory pointless_games_dir() uses: games/ next to the players file.
function games_path(): string
{
    return test_dir() . '/data/games';
}

// TMPDIR for the tests and the server, so tests can check that nothing is left behind.
function tmp_path(): string
{
    return test_dir() . '/tmp';
}

// Where the fake generator records each run (outside its work directory, which is deleted).
function generator_log_path(): string
{
    return test_dir() . '/generator-runs.log';
}

// Deletes $path recursively.
function remove_tree(string $path): void
{
    if (is_link($path) || is_file($path)) {
        unlink($path);
        return;
    }
    if (!is_dir($path)) {
        return;
    }
    foreach (scandir($path) as $name) {
        if ($name !== '.' && $name !== '..') {
            remove_tree("$path/$name");
        }
    }
    rmdir($path);
}

// Names of everything under $dir (relative paths), sorted.
function list_tree(string $dir): array
{
    $names = [];
    $it = new RecursiveIteratorIterator(
        new RecursiveDirectoryIterator($dir, FilesystemIterator::SKIP_DOTS),
        RecursiveIteratorIterator::SELF_FIRST
    );
    foreach ($it as $file) {
        $names[] = substr($file->getPathname(), strlen($dir) + 1);
    }
    sort($names);
    return $names;
}

// Creates a fake production tree (src/main and resources/) laid out like
// `make production`. Like the real one, src/main takes `-s <seed>` or `-e <email>`
// and prints "Seed: <seed>" and a password line; its seed for an email is crc32(email).
// $mode selects what else it does:
//   'ok'      writes zipfiles/puzzle1.zip containing "seed=<seed>"
//   'fail'    exits with status 1
//   'no-zip'  exits with status 0 without writing a zip
//   'big'     writes a 64 MB zipfiles/puzzle1.zip, too big to send before a client hangs up
//   'missing' no src/main at all
function fake_generator(string $mode = 'ok'): void
{
    $gen = generator_path();
    remove_tree($gen);
    mkdir("$gen/src/zipfiles", 0700, true);
    mkdir("$gen/resources/files-test", 0700, true);
    file_put_contents("$gen/resources/files-test/.desc.txt", "resource marker\n");
    if ($mode === 'missing') {
        return;
    }
    $log = var_export(generator_log_path(), true);
    $mode = var_export($mode, true);
    $php = PHP_BINARY;
    $script = <<<PHP
        #!$php
        <?php
        // Fake puzzle generator written by web-server/tests/lib.php.
        file_put_contents($log, json_encode([
            'args' => array_slice(\$argv, 1),
            'cwd' => getcwd(),
            'has_resources' => is_file('../resources/files-test/.desc.txt'),
        ]) . "\\n", FILE_APPEND);
        \$seed = \$argv[1] === '-e' ? (string)crc32(\$argv[2]) : \$argv[2];
        echo "Seed: \$seed\nFake       Password: pw\$seed\n";
        if ($mode === 'fail') {
            fwrite(STDERR, "fake generator failure\\n");
            exit(1);
        }
        if ($mode === 'ok') {
            file_put_contents('zipfiles/puzzle1.zip', "seed=\$seed");
        }
        if ($mode === 'big') {
            file_put_contents('zipfiles/puzzle1.zip', str_repeat('x', 64 << 20));
        }
        PHP;
    file_put_contents("$gen/src/main", $script);
    chmod("$gen/src/main", 0700);
}

// Decoded runs recorded by the fake generator, oldest first.
function generator_runs(): array
{
    if (!is_file(generator_log_path())) {
        return [];
    }
    $lines = file(generator_log_path(), FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);
    return array_map(fn ($line) => json_decode($line, true), $lines);
}

// Resets the state shared by all tests: no players, a working fake generator,
// empty TMPDIR and generator log.
function reset_state(): void
{
    remove_tree(dirname(players_path()));
    remove_tree(tmp_path());
    mkdir(tmp_path(), 0700);
    @unlink(generator_log_path());
    fake_generator('ok');
}

// Starts `php -S` on a free port serving web-server/ with the test environment.
// Returns the base URL. The server is stopped when the runner exits.
function start_server(): string
{
    static $url = null;
    if ($url !== null) {
        return $url;
    }
    $log = test_dir() . '/server.log';
    $env = array_merge(getenv(), [
        'POINTLESS_PLAYERS_FILE' => players_path(),
        'POINTLESS_GENERATOR_DIR' => generator_path(),
        'TMPDIR' => tmp_path(),
    ]);
    $sessions = test_dir() . '/sessions';
    @mkdir($sessions, 0700);
    $cmd = [PHP_BINARY, '-d', "session.save_path=$sessions", '-S', '127.0.0.1:0', '-t', dirname(__DIR__)];
    $proc = proc_open($cmd, [0 => ['file', '/dev/null', 'r'], 1 => ['file', $log, 'w'], 2 => ['redirect', 1]], $pipes, null, $env);
    if (!is_resource($proc)) {
        throw new RuntimeException('could not start php -S');
    }
    register_shutdown_function(function () use ($proc) {
        proc_terminate($proc);
        proc_close($proc);
    });
    for ($i = 0; $i < 100; $i++) {
        if (preg_match('#\(http://(127\.0\.0\.1:\d+)\) started#', (string)@file_get_contents($log), $m)) {
            return $url = "http://$m[1]";
        }
        if (!proc_get_status($proc)['running']) {
            break;
        }
        usleep(50000);
    }
    throw new RuntimeException("php -S did not start; see $log");
}

// A browser: sends requests to the test server and keeps the session cookie.
final class Client
{
    private array $cookies = [];

    public function get(string $path): Response
    {
        return $this->request('GET', $path, null);
    }

    public function post(string $path, array $fields = []): Response
    {
        return $this->request('POST', $path, $fields);
    }

    // Sends an empty POST, reads the start of the response, and disconnects,
    // like a player who cancels a download.
    public function postAndHangUp(string $path): void
    {
        $host = parse_url(start_server(), PHP_URL_HOST) . ':' . parse_url(start_server(), PHP_URL_PORT);
        $socket = stream_socket_client("tcp://$host", $errno, $errstr, 30);
        if ($socket === false) {
            throw new TestFailure("POST $path: could not connect: $errstr");
        }
        $headers = array_merge(["POST /$path HTTP/1.1", "Host: $host", 'Content-Length: 0', 'Connection: close'],
                               $this->cookieHeaders());
        fwrite($socket, implode("\r\n", $headers) . "\r\n\r\n");
        fread($socket, 1024);
        fclose($socket);
    }

    private function cookieHeaders(): array
    {
        if (!$this->cookies) {
            return [];
        }
        $pairs = [];
        foreach ($this->cookies as $name => $value) {
            $pairs[] = "$name=$value";
        }
        return ['Cookie: ' . implode('; ', $pairs)];
    }

    private function request(string $method, string $path, ?array $fields): Response
    {
        $headers = $this->cookieHeaders();
        $options = ['method' => $method, 'follow_location' => 0, 'ignore_errors' => true, 'timeout' => 30];
        if ($fields !== null) {
            $headers[] = 'Content-Type: application/x-www-form-urlencoded';
            $options['content'] = http_build_query($fields);
        }
        $options['header'] = implode("\r\n", $headers);

        $body = file_get_contents(start_server() . "/$path", false, stream_context_create(['http' => $options]));
        if ($body === false) {
            throw new TestFailure("$method $path: request failed");
        }
        $response = new Response($http_response_header, $body);
        foreach ($response->headers('Set-Cookie') as $cookie) {
            [$pair] = explode(';', $cookie, 2);
            [$name, $value] = explode('=', $pair, 2);
            $this->cookies[trim($name)] = trim($value);
        }
        return $response;
    }
}

final class Response
{
    public int $status;
    public string $body;
    private array $headerLines;

    public function __construct(array $headerLines, string $body)
    {
        preg_match('#^HTTP/\S+ (\d{3})#', $headerLines[0], $m);
        $this->status = (int)$m[1];
        $this->headerLines = array_slice($headerLines, 1);
        $this->body = $body;
    }

    // All values of header $name (case-insensitive).
    public function headers(string $name): array
    {
        $values = [];
        foreach ($this->headerLines as $line) {
            [$key, $value] = array_pad(explode(':', $line, 2), 2, '');
            if (strcasecmp(trim($key), $name) === 0) {
                $values[] = trim($value);
            }
        }
        return $values;
    }

    public function header(string $name): ?string
    {
        return $this->headers($name)[0] ?? null;
    }

    public function contains(string $text): bool
    {
        return str_contains($this->body, $text);
    }
}

// Registers a player through index.php with $client.
function register(Client $client, string $email, string $fname = 'Test', string $lname = 'Player'): Response
{
    return $client->post('index.php', ['fname' => $fname, 'lname' => $lname, 'email' => $email]);
}

// Rows of the players file, including the header; [] if it doesn't exist.
function player_rows(): array
{
    if (!is_file(players_path())) {
        return [];
    }
    $rows = [];
    $file = fopen(players_path(), 'r');
    while (($row = fgetcsv($file)) !== false) {
        $rows[] = $row;
    }
    fclose($file);
    return $rows;
}

// The Seed column of $email's row in the players file, or null if $email is not there.
function player_seed(string $email): ?string
{
    foreach (array_slice(player_rows(), 1) as $row) {
        if ($row[2] === $email) {
            return $row[3] ?? '';
        }
    }
    return null;
}

// Appends a player row without a seed, like those written before seeds were stored.
function add_legacy_player(string $fname, string $lname, string $email): void
{
    $path = players_path();
    @mkdir(dirname($path), 0700, true);
    $new = !is_file($path);
    $file = fopen($path, 'a');
    if ($new) {
        fputcsv($file, ['FName', 'LName', 'Email']);
    }
    fputcsv($file, [$fname, $lname, $email]);
    fclose($file);
}
