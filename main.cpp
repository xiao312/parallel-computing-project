#include "InputParameters.h"
#include "PointCloud.h"
#include "Distribution.h"
#include "VDistribution.h"

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
    for (std::size_t i = 0; i < cloud.points.size(); ++i) {
            double x_i = cloud.points[i].x;
            double y_i = cloud.points[i].y;
            double z_i = cloud.points[i].z;

            std::cout << "Point " << i << ":\n";
            std::cout << "x=" << x_i << ", y=" << y_i << ", z=" << z_i << '\n';
    }


    Distribution Distribution(params.getDistributionPath());
    std::cout << "\nCutoff: " << Distribution.getCutoff() << std::endl;
    std::cout << "Dr: " << Distribution.getDr() << std::endl;
    std::cout << "Mesh: " << Distribution.getMesh() << std::endl;
    std::cout << "L: " << Distribution.getL() << std::endl;
    std::cout << "F values:";

    const auto& fValues = Distribution.getF();
    int count = 0;
    for(double f : fValues) {
        // std::cout << f << " ";
        count++;
    }
    // std::cout << "\nCount = " << count << std::endl;

    if (count == (int) Distribution.getMesh()) {
        std::cout << " Number of values check with mesh size!" << std::endl;
    }

    int count_ = 0;
    double tag_ = 0;
    std::map fMap = Distribution.getFMap();

    while (tag_ <= Distribution.getCutoff()){
        auto iter = fMap.find(std::to_string(tag_));
        if (iter != fMap.end()) {
            // Key found in map
            count_++;
            tag_ += Distribution.getDr();
        }
    }
    if (count_ == (int) Distribution.getMesh()) {
        std::cout << "F values in map type: Number of values check with mesh size!" << std::endl;
    }
    else {
        std::cout << count_ << std::endl;
    }

    VDistribution VDistribution(params.getVPath());
    std::cout << "\nnx: " << VDistribution.getNx() << std::endl;
    std::cout << "ny: " << VDistribution.getNy() << std::endl;
    std::cout << "nz: " << VDistribution.getNz() << std::endl;
    std::cout << "ngrid: " << VDistribution.getNgrid() << std::endl;
    std::cout << "V values:";

    int Vcount_ = 0;
    std::map vMap = VDistribution.getVMap();

    for (int x=1; x<=VDistribution.getNx();x++) {
        for (int y=1; y<=VDistribution.getNx();y++) {
            for (int z=1; z<=VDistribution.getNz();z++) {
                std::string tag_ = "(" + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + ")";
                auto iter = vMap.find(tag_);
                if (iter != vMap.end()) {
                    // Key found in map
                    Vcount_++;
                }
            }
        }
    }
    if (Vcount_ == (int) VDistribution.getNgrid()) {
        std::cout << "V values in map type: Number of values check with grid size!" << std::endl;
    }
    else {
        std::cout << Vcount_ << std::endl;
    }
    


    return 0;
}