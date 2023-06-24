#include "InputParameters.h"
#include "PointCloud.h"
#include "Distribution.h"
#include "VDistribution.h"
#include <cmath>
#include <gsl/gsl_interp.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>
#include <lapacke.h>

double distance(int x1, int y1, int z1, double x2, double y2, double z2)
{
    return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1));
}

int main()
{

    // Load input parameters from file:
    InputParameters params("./input/INPUT.txt");
    int lx = params.getLx();
    int ly = params.getLy();
    int lz = params.getLz();

    // Print input parameter values:
    std::cout << "isHexahedral: "              << params.isHexahedral()             << std::endl;
    std::cout << "lx: "                        << params.getLx()                    << std::endl;
    std::cout << "ly: "                        << params.getLy()                    << std::endl;
    std::cout << "lz: "                        << params.getLz()                    << std::endl;
    std::cout << "thetaxy: "                   << params.getThetaxy()               << std::endl;
    std::cout << "thetayz: "                   << params.getThetayz()               << std::endl;
    std::cout << "thetaxz: "                   << params.getThetaxz()               << std::endl;
    std::cout << "support_SH: "                << params.supportSH()                << std::endl;
    std::cout << "diago_lib: "                 << params.getDiagoLib()              << std::endl;
    std::cout << "support_Periodic_Boundary: " << params.supportPeriodicBoundary()  << std::endl;
    std::cout << "multi_parallel_strategies: " << params.multiParallelStrategies()  << std::endl;
    std::cout << "points_path: "               << params.getPointsPath()            << std::endl;
    std::cout << "venergy_path: "              << params.getVPath()                 << std::endl;
    std::cout << "distribution_path: "         << params.getDistributionPath()      << std::endl;


    // Load the point cloud from file:
    PointCloud cloud;
    cloud.loadFromFile(params.getPointsPath());

    // Print information about the point cloud:
    int i_ = 0;
    int N = cloud.points.size();
    std::cout << "\nNumber of points: " << N << '\n';
    for (std::size_t i = 0; i < N; ++i) {
            double x_i = cloud.points[i].x;
            double y_i = cloud.points[i].y;
            double z_i = cloud.points[i].z;

            std::cout << "Point " << i << ":\n";
            std::cout << "x=" << x_i << ", y=" << y_i << ", z=" << z_i << '\n';
    }


    Distribution Distribution(params.getDistributionPath());
    double cutoff = Distribution.getCutoff();
    int mesh = Distribution.getMesh();
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
    int nx = VDistribution.getNx();
    int ny = VDistribution.getNy();
    int nz = VDistribution.getNz();

    std::cout << "\nnx: " << nx << std::endl;
    std::cout <<   "ny: " << ny << std::endl;
    std::cout <<   "nz: " << nz << std::endl;
    std::cout << "ngrid: " << VDistribution.getNgrid() << std::endl;

    int Vcount_ = 0;
    std::map vMap = VDistribution.getVMap();

    for (int x = 1; x <= nx; x++) {
        for (int y = 1; y <= ny; y++) {
            for (int z = 1; z <= nz; z++) {
                std::string tag_ = "(" + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + ")";
                auto iter = vMap.find(tag_);
                if (iter != vMap.end()) {
                    // Key found in map
                    // std::cout << vMap[tag_] << std::endl;
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
    

    

    int dx = lx / nx;
    int dy = ly / ny;
    int dz = lz / nz;

    for (int x_ = 1; x_ <= nx; x_++) {
        for (int y_ = 1; y_ <= ny; y_++) {
            for (int z_ = 1; z_ <= nz; z_++) {
                int x = (x_ - 1) * dx;
                int y = (y_ - 1) * dy;
                int z = (z_ - 1) * dz;
                double V = VDistribution.getValue(x_, y_, z_);
                // std::cout << V << std::endl;

                double f_i = 0;
                double f_j = 0;

                for (std::size_t i = 0; i < cloud.points.size(); ++i) {
                    double x_i = cloud.points[i].x;
                    double y_i = cloud.points[i].y;
                    double z_i = cloud.points[i].z;

                    // std::cout << "Point " << i << ":\n";
                    // std::cout << "x=" << x_i << ", y=" << y_i << ", z=" << z_i << '\n';

                    double r_i = distance(x, y, z, x_i, y_i, z_i);
                    if (r_i > cutoff) {continue;}
                    else {
                            gsl_interp_accel *acc = gsl_interp_accel_alloc(); // 创建加速器对象
                            gsl_spline *spline = gsl_spline_alloc(gsl_interp_cspline, mesh); // 创建插值对象

                            gsl_spline_init(spline, Distribution.getR(), Distribution.getFval(), mesh); // 初始化插值对象

                            f_i = gsl_spline_eval(spline, r_i, acc); // 对新点进行插值计算
                            // std::cout << "f(" << f_i << ") = " << r_i << std::endl; // 输出插值结果
                            
                            gsl_spline_free(spline); // 释放插值对象
                            gsl_interp_accel_free(acc); // 释放加速器对象
                    }


                    for (std::size_t j = 0; j < i+1; ++j) {
                        double x_j = cloud.points[j].x;
                        double y_j = cloud.points[j].y;
                        double z_j = cloud.points[j].z;                        

                        double r_j = distance(x, y, z, x_j, y_j, z_j);
                        if (r_j > cutoff) {continue;}
                        else {
                                gsl_interp_accel *acc = gsl_interp_accel_alloc(); // 创建加速器对象
                                gsl_spline *spline = gsl_spline_alloc(gsl_interp_cspline, mesh); // 创建插值对象

                                gsl_spline_init(spline, Distribution.getR(), Distribution.getFval(), mesh); // 初始化插值对象

                                f_j = gsl_spline_eval(spline, r_j, acc); // 对新点进行插值计算
                                // std::cout << "f(" << f_j << ") = " << r_j << std::endl; // 输出插值结果
                                
                                gsl_spline_free(spline); // 释放插值对象
                                gsl_interp_accel_free(acc); // 释放加速器对象
                        }
                        // std::cout << "Point " << j << ":\n";
                        // std::cout << "x=" << x_j << ", y=" << y_j << ", z=" << z_j << '\n';

                        double sum_ij = f_i * V * f_j;
                    }
                }


            }
        }
    }

    return 0;
}