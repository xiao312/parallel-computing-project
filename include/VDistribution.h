#ifndef VDistribution_H
#define VDistribution_H

#include <vector>
#include <string>
#include <map>

class VDistribution {
public:
    VDistribution(const std::string& filename); // Constructor to read input file
    int getNx() const; // Getter for nx value
    int getNy() const; // Getter for ny value
    int getNz() const; // Getter for nz value
    int getNgrid() const; // Getter for ngrid value

    const std::map<std::string, double>& getVMap() const; // Getter for fmap_

    void setValue(int x, int y, int z, double value); // Set point value
    double getValue(int x, int y, int z) const; // Get point value

private:
    int nx_, ny_, nz_, ngrid_;

    std::map<std::string, double> vmap_;
};

#endif /* VDistribution_H */
