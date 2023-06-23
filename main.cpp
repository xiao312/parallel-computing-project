#include "InputParameters.h"
#include "PointCloud.h"
#include "Distribution.h"

int main()
{

    // Load input parameters from file:
    InputParameters params("./input/INPUT.txt");

    // Print input parameter values:
    std::cout << "isHexahedral: " << params.isHexahedral() << std::endl;
    std::cout << "lx: " << params.getLx() << std::endl;
    std::cout << "ly: " << params.getLy() << std::endl;
    std::cout << "lz: " << params.getLz() << std::endl;
    std::cout << "thetaxy: " << params.getThetaxy() << std::endl;
    std::cout << "thetayz: " << params.getThetayz() << std::endl;
    std::cout << "thetaxz: " << params.getThetaxz() << std::endl;
    std::cout << "support_SH: " << params.supportSH() << std::endl;
    std::cout << "diago_lib: " << params.getDiagoLib() << std::endl;
    std::cout << "support_Periodic_Boundary: " << params.supportPeriodicBoundary() << std::endl;
    std::cout << "multi_parallel_strategies: " << params.multiParallelStrategies() << std::endl;
    std::cout << "points_path: " << params.getPointsPath() << std::endl;
    std::cout << "venergy_path: " << params.getVPath() << std::endl;
    std::cout << "distribution_path: " << params.getDistributionPath() << std::endl;


    // Load the point cloud from file:
    PointCloud cloud;
    cloud.loadFromFile(params.getPointsPath());

    // Print information about the point cloud:
    int i_ = 0;
    std::cout << "\nNumber of points: " << cloud.points.size() << '\n';
    for (const auto& point : cloud.points) {
        std::cout << "Point " << i_++ << ":\n";
        std::cout << "x=" << point.x << ", y=" << point.y << ", z=" << point.z << '\n';
    }

    Distribution Distribution(params.getDistributionPath());
    std::cout << "\nCutoff: " << Distribution.getCutoff() << std::endl;
    std::cout << "Dr: " << Distribution.getDr() << std::endl;
    std::cout << "Mesh: " << Distribution.getMesh() << std::endl;
    std::cout << "L: " << Distribution.getL() << std::endl;
    std::cout << "F values:" << std::endl;
    const auto& fValues = Distribution.getF();
    for(auto f : fValues) {
        std::cout << f << " ";
    }
    std::cout << std::endl;


    return 0;
}