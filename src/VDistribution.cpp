#include "VDistribution.h"
#include <fstream>
#include <sstream>
#include <iostream>

VDistribution::VDistribution(const std::string& filename) {
    std::ifstream inputFile(filename);
    std::string line;
    


    if (inputFile.is_open()) {
        getline(inputFile, line);
        nx_ = stoi(line.substr(line.find(' ')+1));
        getline(inputFile, line);
        ny_ = stoi(line.substr(line.find(' ')+1));
        getline(inputFile, line);
        nz_ = stoi(line.substr(line.find(' ')+1));

        ngrid_ = nx_ * ny_ * nz_;


        getline(inputFile, line);
        if (line == "V:") {

            double value;
            int x = 1, y = 1, z = 1;
            while (inputFile >> value) {
                if (z > nz_) {
                    z = 1;
                    y++;
                    if (y > ny_) {
                        y = 1;
                        x++;
                    }
                }
                setValue(x, y, z, value);

                z++;
            }
        }

        inputFile.close();
    }

}

int VDistribution::getNx() const {
    return nx_;
}

int VDistribution::getNy() const {
    return ny_;
}

int VDistribution::getNz() const {
    return nz_;
}

int VDistribution::getNgrid() const {
    return ngrid_;
}

// const std::vector<double>& VDistribution::getV() const {
//     return v_;
// }

const std::map<std::string, double>& VDistribution::getVMap() const {
    return vmap_;
}


void VDistribution::setValue(int x, int y, int z, double value) {
    std::string tag = "(" + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + ")";
    vmap_[tag] = value;
}

double VDistribution::getValue(int x, int y, int z) const {
    std::string tag = "(" + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + ")";
    auto it = vmap_.find(tag);
    if (it != vmap_.end()) {
        return it->second;
    } else {
        return 0.0;
    }
}