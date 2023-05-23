#include "InputParameters.h"
#include <fstream>
#include <sstream>
#include <iostream>

InputParameters::InputParameters(const std::string& filename)
{
    std::ifstream inputFile(filename);
    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::string key, value;
        if (!(iss >> key >> value)) { continue; }
        if (key == "isHexahedral") { m_isHexahedral = (value == "1"); }
        else if (key == "lx") { m_lx = std::stoi(value); }
        else if (key == "ly") { m_ly = std::stoi(value); }
        else if (key == "lz") { m_lz = std::stoi(value); }
        else if (key == "thetaxy") { m_thetaxy = std::stod(value); }
        else if (key == "thetayz") { m_thetayz = std::stod(value); }
        else if (key == "thetaxz") { m_thetaxz = std::stod(value); }
        else if (key == "support_SH") { m_support_SH = (value == "1"); }
        else if (key == "diago_lib") { m_diago_lib = value; }
        else if (key == "support_Periodic_Boundary") { m_support_Periodic_Boundary = (value == "1"); }
        else if (key == "multi_parallel_strategies") { m_multi_parallel_strategies = (value == "1"); }
        else if (key == "points_path") { m_points_path = value; }
        else if (key == "venergy_path") { m_venergy_path = value; }
        else if (key == "distribution_path") { m_distribution_path = value; }
    }
}


// int main()
// {
//     InputParameters params("INPUT.txt");
//     std::cout << "isHexahedral: " << params.isHexahedral() << std::endl;
//     std::cout << "lx: " << params.getLx() << std::endl;
//     std::cout << "ly: " << params.getLy() << std::endl;
//     std::cout << "lz: " << params.getLz() << std::endl;
//     std::cout << "thetaxy: " << params.getThetaxy() << std::endl;
//     std::cout << "thetayz: " << params.getThetayz() << std::endl;
//     std::cout << "thetaxz: " << params.getThetaxz() << std::endl;
//     std::cout << "support_SH: " << params.supportSH() << std::endl;
//     std::cout << "diago_lib: " << params.getDiagoLib() << std::endl;
//     std::cout << "support_Periodic_Boundary: " << params.supportPeriodicBoundary() << std::endl;
//     std::cout << "multi_parallel_strategies: " << params.multiParallelStrategies() << std::endl;
//     std::cout << "points_path: " << params.getPointsPath() << std::endl;
//     std::cout << "venergy_path: " << params.getVPath() << std::endl;
//     std::cout << "distribution_path: " << params.getDistributionPath() << std::endl;
//     return 0;
// }