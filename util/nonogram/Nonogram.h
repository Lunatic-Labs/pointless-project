#include "../../lib/cpp/qrcodegen.hpp"
#include <map>
#include <list>

#ifndef NONOGRAM_H
#define NONOGRAM_H
class Nonogram {

public:
    Nonogram(qrcodegen::QrCode secret);                    // Construct Nonogram from QR Code
    std::vector<std::list<int>> Nonogram::getColumnRuns(); // Get runs for column headers
    std::vector<std::list<int>> Nonogram::getRowRuns();    // Get runs for row headers
    std::map<std::pair<int, int>, bool> getHints();        // Get hints for solved positions
};
#endif