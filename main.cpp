#include "InputParameters.h"
#include "PointCloud.h"

int main()
{

    InputParameters params("INPUT.txt");
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
    std::cout << "v_path: " << params.getVPath() << std::endl;
    std::cout << "distribution_path: " << params.getDistributionPath() << std::endl;


    // Load the point cloud from file:
    PointCloud cloud;
    cloud.loadFromFile("POINTS.txt");

    // Now you can access the point data like this:
    int i_ = 0;
    std::cout << "\nNumber of points: " << cloud.points.size() << '\n';
    for (const auto& point : cloud.points) {
        std::cout << "Point " << i_++ << ":\n";
        std::cout << "x=" << point.x << ", y=" << point.y << ", z=" << point.z << '\n';
    }



    return 0;
}