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

            size_t pos = 0;
            std::string token;
            int index = 0;
            while ((pos = line.find(',')) != std::string::npos) {
                token = line.substr(0, pos);
                double f_value = stod(token);
                std::string tag = std::to_string(index *dr_);

                f_.push_back(stod(token));

                fmap_.insert(std::make_pair(tag, f_value));
                index++;

                line.erase(0, pos + 1);
            }
        }

        inputFile.close();
    }

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

const std::vector<double>& Distribution::getF() const {
    return f_;
}

const std::map<std::string, double>& Distribution::getFMap() const {
    return fmap_;
}
