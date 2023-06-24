#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H

#include <vector>
#include <string>
#include <map>

class Distribution {
public:
    Distribution(const std::string& filename); // Constructor to read input file
    ~Distribution();
    double getCutoff() const; // Getter for cutoff value
    double getDr() const; // Getter for dr value
    int getMesh() const; // Getter for mesh value
    double getL() const; // Getter for L value
    const std::vector<double>& getF() const; // Getter for vector of f values
    const std::map<std::string, double>& getFMap() const; // Getter for fmap_

    double* getR() { return r_; } // Getter for r array
    double* getFval() { return fval_; }

private:
    double cutoff_;
    double dr_;
    int mesh_;
    double l_;

    double* r_;
    double* fval_;
    std::vector<double> f_;
    // using FMap = std::map<double, double>; // define a type alias
    // FMap fmap_; // use the type alias instead of the fully qualified name
    std::map<std::string, double> fmap_;
};

#endif /* DISTRIBUTION_H */
