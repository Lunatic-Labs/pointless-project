#include "crow.h"
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <stdexcept>

// Function to generate the file
void generateFile(const std::string& filename) {
    std::ofstream outfile(filename);
    if (!outfile.is_open()) {
        throw std::runtime_error("Could not open the file to write");
    }
    outfile << "This is a dynamically generated file.\n";
    outfile << "Timestamp: " << time(nullptr) << "\n";
    outfile.close();
}

// Function to read the contents of the file
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

    // Route to serve the HTML page
    CROW_ROUTE(app, "/")([]() {
        return readFile("static/index.html");
    });

    // Route to generate and download the file
    CROW_ROUTE(app, "/download")([]() {
        std::string filename = "static/output.txt";

        // Generate the file
        try {
            generateFile(filename);
        } catch (const std::exception& e) {
            return crow::response(500, "Error generating file: " + std::string(e.what()));
        }

        // Serve the generated file
        auto file_content = readFile(filename);
        crow::response res;
        res.set_header("Content-Disposition", "attachment; filename=output.txt");
        res.write(file_content);
        return res;
    });

    // Run the application on localhost, port 8080
    app.port(8080).multithreaded().run();
}
