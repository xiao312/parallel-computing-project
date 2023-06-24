#include "Distribution.h"
#include <fstream>
#include <sstream>
#include <iostream>

Distribution::Distribution(const std::string& filename) {
    std::ifstream inputFile(filename);
    std::string line;
    
    if (inputFile.is_open()) {
        getline(inputFile, line);
        cutoff_ = stod(line.substr(line.find(' ')+1));
        getline(inputFile, line);
        dr_ = stod(line.substr(line.find(' ')+1));
        getline(inputFile, line);
        mesh_ = stod(line.substr(line.find(' ')+1));
        getline(inputFile, line);
        l_ = stod(line.substr(line.find(' ')+1));

        getline(inputFile, line);
        if (line == "f:") {
            getline(inputFile, line);

            r_ = new double[mesh_];     // Dynamically allocate memory for r_
            fval_ = new double[mesh_];

            size_t pos = 0;
            std::string token;
            int index = 0;
            while ((pos = line.find(',')) != std::string::npos) {
                token = line.substr(0, pos);
                double f_value = stod(token);

                r_[index] = index * dr_;
                fval_[index] = stod(token);

                index++;

                line.erase(0, pos + 1);
            }
        }

        inputFile.close();
    }

}

Distribution::~Distribution() {
    delete[] r_;
    delete[] fval_;
}


double Distribution::getCutoff() const {
    return cutoff_;
}

double Distribution::getDr() const {
    return dr_;
}

int Distribution::getMesh() const {
    return mesh_;
}

double Distribution::getL() const {
    return l_;
}
