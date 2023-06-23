#include "Distribution.h"
#include <fstream>
#include <sstream>

Distribution::Distribution(const std::string& filename) {
    std::ifstream inputFile(filename);
    std::string line;
    std::string f_values; // added variable to store all "f:" values
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::string parameterName;
        iss >> parameterName;
        if (parameterName == "cutoff") {
            iss >> cutoff_;
        } else if (parameterName == "dr") {
            iss >> dr_;
        } else if (parameterName == "mesh") {
            iss >> mesh_;
        } else if (parameterName == "l") {
            iss >> l_;
        } else if (parameterName == "f:") {
            continue;
        } else {
            double value;
            while (iss >> value) {
                f_.push_back(value);
                // f_values += std::to_string(value) + " "; // append each value to the f_values string
            }
        }
    }

    // // parse the concatenated f values at the end
    // std::istringstream iss_f(f_values);
    // double value;
    // while (iss_f >> value) {
    //     f_.push_back(value);
    // }
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
