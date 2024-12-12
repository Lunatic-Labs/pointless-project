#include <string>
#include <iostream>
#include <sqlite3.h>

// g++ dbtest.cpp -lsqlite3

int main(){
    sqlite3* db;
    char* stmt;

    std::string userText;
    std::cout << "Enter Email: ";
    std::cin >> userText;

    int rc = sqlite3_open("STUDENTS.db", &db);
    if(rc == SQLITE_OK) {
        std::string query = "INSERT INTO STUDENTS (email, level) VALUES ('" + userText + "', 0);";
        sqlite3_exec(db, query.c_str(), 0, 0, &stmt);
    }

    sqlite3_close(db);
}