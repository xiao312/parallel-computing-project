#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H

#include <vector>
#include <string>

class Distribution {
public:
    Distribution(const std::string& filename); // Constructor to read input file
    double getCutoff() const; // Getter for cutoff value
    double getDr() const; // Getter for dr value
    int getMesh() const; // Getter for mesh value
    double getL() const; // Getter for L value
    const std::vector<double>& getF() const; // Getter for vector of f values

private:
    double cutoff_;
    double dr_;
    int mesh_;
    double l_;
    std::vector<double> f_;
};

#endif /* DISTRIBUTION_H */
