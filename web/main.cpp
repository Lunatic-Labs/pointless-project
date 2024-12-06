#include "crow.h"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <stdexcept>

std::string readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return "File not found!";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main() {
    crow::SimpleApp app;
    // app.bindaddr("0.0.0.0").port(8080).multithreaded().run();
    // app.bindaddr("127.0.0.1").port(8080).multithreaded().run();

    CROW_ROUTE(app, "/")([]() {
        return readFile("static/index.html");
    });

    CROW_ROUTE(app, "/generateFile").methods("POST"_method)([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body)
            return crow::response(400, "Invalid EMAIL");

        std::string userText = body["text"].s();

        int seed = 1;
        for(int i = 0; i < userText.length(); i++) {
            seed += ((int)userText[i] - 30)*i;
            seed %= 10000000;
        }

        std::string precommand = "cd ../../src && make cleanzip"; 
        std::string command = "cd ../../src && ./main -s " + std::to_string(seed);

        std::system(precommand.c_str());
        int result = std::system(command.c_str());
        if (result != 0) {
            return crow::response(500, "Failed to generate file");
        }

        std::ifstream file("../../src/zipfiles/puzzle1.zip", std::ios::binary);
        if (!file.is_open()) {
            return crow::response(500, "File not found");
        }

        std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        crow::response res(fileContent);
        res.add_header("Content-Type", "application/octet-stream");
        res.add_header("Content-Disposition", "attachment; filename=\"pointless.zip\"");
        
        std::system(precommand.c_str());
        
        return res;
    });

    std::vector<std::string> validTokens = {"1", "2", "3"};

    CROW_ROUTE(app, "/validateToken").methods("POST"_method)([&validTokens](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body) return crow::response(400, "Invalid JSON");

        std::string token = body["token"].s();
        bool isValid = std::find(validTokens.begin(), validTokens.end(), token) != validTokens.end();

        crow::json::wvalue result;
        result["message"] = isValid ? "Token is valid!" : "Token is invalid.";
        return crow::response(result);
    });
    app.port(8080).multithreaded().run();
}
