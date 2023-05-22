#ifndef POINT_CLOUD_H
#define POINT_CLOUD_H

#include <iostream>
#include <fstream>
#include <vector>

class Point {
public:
    double x, y, z;
};

class PointCloud {
public:
    std::vector<Point> points;

    void loadFromFile(const std::string& filename);
};

#endif // POINT_CLOUD_H
