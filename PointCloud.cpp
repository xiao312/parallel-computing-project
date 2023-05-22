#include "PointCloud.h"

void PointCloud::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file " + filename);
    }

    Point point;
    char c; // used to read the commas and parentheses
    while (file >> c) {
        if (c == '(' || c == '[') { // Skip opening brackets
            continue;
        } else if (c == ')' || c == ']') { // Skip closing brackets
            continue;
        }

        file.putback(c); // Put back the character that was read
        if (!(file >> point.x >> c >> point.y >> c >> point.z >> c)) {
            break; // Stop reading if there's a problem with the input file
        }

        points.push_back(point);
    }

}
