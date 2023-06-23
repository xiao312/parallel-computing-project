#include "Distribution.h"
#include <fstream>
#include <sstream>
#include <iostream>

Distribution::Distribution(const std::string& filename) {
    std::ifstream inputFile(filename);
    std::string line;
    std::string f_values; // added variable to store all "f:" values
    
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
            while ((pos = line.find(',')) != std::string::npos) {
                token = line.substr(0, pos);
                f_.push_back(stod(token));
                line.erase(0, pos + 1);
            }
            // f_.push_back(stod(line));

        }
        
        // int count = 0;
        // for(double f : f_) {
        //     std::cout << f << " ";
        //     count++;
        // }
        // std::cout << "\nCount = " << count << std::endl;

        inputFile.close();
    }

    // while (std::getline(inputFile, line)) {
    //     std::istringstream iss(line);
    //     std::string parameterName;
    //     iss >> parameterName;
    //     if (parameterName == "cutoff") {
    //         iss >> cutoff_;
    //     } else if (parameterName == "dr") {
    //         iss >> dr_;
    //     } else if (parameterName == "mesh") {
    //         iss >> mesh_;
    //     } else if (parameterName == "l") {
    //         iss >> l_;
    //     } else if (parameterName == "f:") {
    //         continue;
    //     } 
        
    //     // else {
    //     //     // double value;
    //     //     // while (iss >> value) {
    //     //     //     f_.push_back(value);
    //     //     //     // f_values += std::to_string(value) + " "; // append each value to the f_values string
    //     //     // }

    //     //     while (std::getline(iss, f_values, ',')) {
    //     //                 double num = std::stod(f_values);
    //     //                 f_.push_back(num);
    //     //             }

    //     // }
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
