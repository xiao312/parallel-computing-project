#include "InputParameters.h"
#include "PointCloud.h"
#include "Distribution.h"
#include "VDistribution.h"
#include <cmath>
#include <cstring>
#include <iomanip>
#include <gsl/gsl_interp.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cblas.h>
#include <lapacke.h>

double distance(int x1, int y1, int z1, double x2, double y2, double z2)
{
    return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1));
}

// void print_matrix( const char* desc, int m, int n, double* a, int lda ) {
//         int i, j;
//         printf( "\n%s\n", desc );
//         for( i = 0; i < m; i++ ) {
//                 // for( j = 0; j < n; j++ ) printf( " %6.4f", a[i*lda+j] );
//                 for( j = 0; j < n; j++ ) printf( " %12.6f", a[i*lda+j] );
//                 printf( "\n" );
//         }
// }

char* print_matrix( const char* desc, int m, int n, double* a, int lda ) {
    int i, j;
    size_t buf_size = sizeof(char) * (strlen(desc) + m * (n + 1) * 12 + 1); // allocate space for matrix and description
    char* buf = (char*) malloc(buf_size);
    if (!buf) return NULL;
    sprintf(buf, "%s", desc);
    char* p = buf + strlen(buf);
    for (i = 0; i < m; i++) {
        p += sprintf(p, "\n");
        for (j = 0; j < n; j++) {
            p += sprintf(p, " %12.6f", a[i*lda+j]);
        }
    }
    return buf;
}


int main()
{



    // Load input parameters from file:
    InputParameters params("./input/INPUT_test.txt");
    int lx = params.getLx();
    int ly = params.getLy();
    int lz = params.getLz();

    // Write to log file
    std::ofstream outputFile("runCode.log");
    int colWidth = 32;
    int lineLength = 55;
    std::string line(lineLength, '-');
    outputFile << "\\*" << line << "*\\" << std::endl;
    outputFile << " COURSE PROJECT OF PARALLEL COMPUTATION AND DESIGN" << std::endl;
    outputFile << std::left << std::setw(8) << " By:" << "肖轲" << std::endl;
    outputFile << "\\*" << line << "*\\" << std::endl;

    // Print input parameter values:
    outputFile << "\nREADING INPUT FILES!!" << std::endl;
    outputFile << "\\*" << line << "*\\" << std::endl;
    outputFile << std::left << std::setw(colWidth) << " isHexahedral: "              << params.isHexahedral()             << std::endl;
    outputFile << std::left << std::setw(colWidth) << " lx: "                        << params.getLx()                    << std::endl;
    outputFile << std::left << std::setw(colWidth) << " ly: "                        << params.getLy()                    << std::endl;
    outputFile << std::left << std::setw(colWidth) << " lz: "                        << params.getLz()                    << std::endl;
    outputFile << std::left << std::setw(colWidth) << " thetaxy: "                   << params.getThetaxy()               << std::endl;
    outputFile << std::left << std::setw(colWidth) << " thetayz: "                   << params.getThetayz()               << std::endl;
    outputFile << std::left << std::setw(colWidth) << " thetaxz: "                   << params.getThetaxz()               << std::endl;
    outputFile << std::left << std::setw(colWidth) << " support_SH: "                << params.supportSH()                << std::endl;
    outputFile << std::left << std::setw(colWidth) << " diago_lib: "                 << params.getDiagoLib()              << std::endl;
    outputFile << std::left << std::setw(colWidth) << " support_Periodic_Boundary: " << params.supportPeriodicBoundary()  << std::endl;
    outputFile << std::left << std::setw(colWidth) << " multi_parallel_strategies: " << params.multiParallelStrategies()  << std::endl;
    outputFile << std::left << std::setw(colWidth) << " points_path: "               << params.getPointsPath()            << std::endl;
    outputFile << std::left << std::setw(colWidth) << " venergy_path: "              << params.getVPath()                 << std::endl;
    outputFile << std::left << std::setw(colWidth) << " distribution_path: "         << params.getDistributionPath()      << std::endl;
    outputFile << "\\*" << line << "*\\" << std::endl;
    outputFile.close();



    // Load the point cloud from file:
    PointCloud cloud;
    cloud.loadFromFile(params.getPointsPath());
    int N = cloud.points.size();

    // Print information about the point cloud:
    std::ofstream outputFile_("runCode.log", std::ios::app);
    outputFile_ << "\nREADING POINT LOCATIONS!!" << std::endl;
    outputFile_ << "\\*" << line << "*\\" << std::endl;
    outputFile_ << "Number of points: " << N << '\n';
    for (std::size_t i = 0; i < N; ++i) {
            double x_i = cloud.points[i].x;
            double y_i = cloud.points[i].y;
            double z_i = cloud.points[i].z;

            outputFile_ << " Point " << i << ":\n";
            outputFile_ << "  (" << std::right << std::setw(6) << x_i 
                                 << std::right << std::setw(6) << y_i 
                                 << std::right << std::setw(6) << z_i << ")" << std::endl;
    }
    outputFile_ << "\\*" << line << "*\\" << std::endl;
    outputFile_.close();



    Distribution Distribution(params.getDistributionPath());
    double cutoff   = Distribution.getCutoff();
    int mesh        = Distribution.getMesh();
    std::map fMap   = Distribution.getFMap();


    std::ofstream _outputFile("runCode.log", std::ios::app);
    _outputFile << "\nREADING DISTRIBUTION & V VALUES ON GRID POINTS!!" << std::endl;
    _outputFile << "\\*" << line << "*\\" << std::endl;
    _outputFile << std::left << std::setw(colWidth) << " Cutoff: " << Distribution.getCutoff() << std::endl;
    _outputFile << std::left << std::setw(colWidth) << " Dr: "     << Distribution.getDr()     << std::endl;
    _outputFile << std::left << std::setw(colWidth) << " Mesh: "   << Distribution.getMesh()   << std::endl;
    _outputFile << std::left << std::setw(colWidth) << " L: "      << Distribution.getL()      << std::endl;


    // std::cout << "F values:";

    // const auto& fValues = Distribution.getF();
    // int count = 0;
    // for(double f : fValues) {
    //     // std::cout << f << " ";
    //     count++;
    // }
    // // std::cout << "\nCount = " << count << std::endl;

    // if (count == (int) Distribution.getMesh()) {
    //     std::cout << " Number of values check with mesh size!" << std::endl;
    // }

    // int count_ = 0;
    // double tag_ = 0;

    // while (tag_ <= Distribution.getCutoff()){
    //     auto iter = fMap.find(std::to_string(tag_));
    //     if (iter != fMap.end()) {
    //         // Key found in map
    //         count_++;
    //         tag_ += Distribution.getDr();
    //     }
    // }
    // if (count_ == (int) Distribution.getMesh()) {
    //     std::cout << "F values in map type: Number of values check with mesh size!" << std::endl;
    // }
    // else {
    //     std::cout << count_ << std::endl;
    // }

    VDistribution VDistribution(params.getVPath());
    int nx = VDistribution.getNx();
    int ny = VDistribution.getNy();
    int nz = VDistribution.getNz();
    std::map vMap = VDistribution.getVMap();

    _outputFile << std::endl;
    _outputFile << std::left << std::setw(colWidth) << " nx: "    << nx << std::endl;
    _outputFile << std::left << std::setw(colWidth) << " ny: "    << ny << std::endl;
    _outputFile << std::left << std::setw(colWidth) << " nz: "    << nz << std::endl;
    _outputFile << std::left << std::setw(colWidth) << " ngrid: " << VDistribution.getNgrid() << std::endl;

    _outputFile << "\\*" << line << "*\\" << std::endl;
    _outputFile.close();



    // int Vcount_ = 0;
    // for (int x = 1; x <= nx; x++) {
    //     for (int y = 1; y <= ny; y++) {
    //         for (int z = 1; z <= nz; z++) {
    //             std::string tag_ = "(" + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + ")";
    //             auto iter = vMap.find(tag_);
    //             if (iter != vMap.end()) {
    //                 // Key found in map
    //                 // std::cout << vMap[tag_] << std::endl;
    //                 Vcount_++;
    //             }
    //         }
    //     }
    // }
    // if (Vcount_ == (int) VDistribution.getNgrid()) {
    //     std::cout << "V values in map type: Number of values check with grid size!" << std::endl;
    // }
    // else {
    //     std::cout << Vcount_ << std::endl;
    // }
    

    int LDA = N, info;
    double w[N];
    double a[N*LDA];
    std::memset(a, 0, sizeof(a));

    // for (int i=0;i<N*LDA;i++) {
    //     std::cout << a[i] << " ";
    // }
    // std::cout << std::endl;


    int dx = lx / nx;
    int dy = ly / ny;
    int dz = lz / nz;
    int dv = dx * dy * dz;

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
                            // std::cout << r_i << "   " << f_i << std::endl;
                    }


                    for (std::size_t j = 0;j < i+1; ++j) {
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

                        double incre_ij = f_i * V * f_j * dv;
                        a[i * N + j] += incre_ij;
                    }
                }


            }
        }
    }

    // std::cout << "\n";
    // for (int i=0;i<N*LDA;i++) {
    //     std::cout << a[i] << " ";
    // }
    // std::cout << "\n" << std::endl;

    std::ofstream of("runCode.log", std::ios::app);
    of << "\nWRITING COMPUTATION RESULT OF H MATRIX!!" << std::endl;
    of << "\\*" << line << "*\\" << std::endl;
    of << " " << print_matrix( "H matrix", N, N,  a, LDA ) << std::endl;
    of << "\\*" << line << "*\\" << std::endl;
    of.close();






    info = LAPACKE_dsyev(LAPACK_ROW_MAJOR, 'V', 'L', N, a, LDA, w );

    if( info > 0 ) {
        printf( "The algorithm failed to compute eigenvalues.\n" );
        exit( 1 );
    }

    // Print eigenvalues
    std::ofstream of_("eigenvalues.log");
    of_ << "LAPACKE_dsyev (row-major, high-level) Results:" << std::endl;
    of_ << "\\*" << line << "*\\" << std::endl;
    of_ << " " << print_matrix( "Eigenvalues", 1, N, w, 1 ) << std::endl;
    of_ << "\\*" << line << "*\\" << std::endl;
    of_.close();

    // Print eigenvectors
    std::ofstream _of("eigenvectors.log");
    _of << "LAPACKE_dsyev (row-major, high-level) Results:" << std::endl;
    _of << "\\*" << line << "*\\" << std::endl;
    _of << " " << print_matrix( "Eigenvectors (stored columnwise)", N, N, a, LDA ) << std::endl;
    _of << "\\*" << line << "*\\" << std::endl;
    _of.close();



    

    return 0;
}