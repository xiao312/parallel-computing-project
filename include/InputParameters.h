#ifndef INPUT_PARAMETERS_H
#define INPUT_PARAMETERS_H

#include <string>
#include <map>

class InputParameters {
public:
    InputParameters(const std::string& filename);

    bool isHexahedral() const { return m_isHexahedral; }
    int getLx() const { return m_lx; }
    int getLy() const { return m_ly; }
    int getLz() const { return m_lz; }
    double getThetaxy() const { return m_thetaxy; }
    double getThetayz() const { return m_thetayz; }
    double getThetaxz() const { return m_thetaxz; }
    bool supportSH() const { return m_support_SH; }
    std::string getDiagoLib() const { return m_diago_lib; }
    bool supportPeriodicBoundary() const { return m_support_Periodic_Boundary; }
    bool multiParallelStrategies() const { return m_multi_parallel_strategies; }
    std::string getPointsPath() const { return m_points_path; }
    std::string getVPath() const { return m_venergy_path; }
    std::string getDistributionPath() const { return m_distribution_path; }

private:
    bool m_isHexahedral;
    int m_lx;
    int m_ly;
    int m_lz;
    double m_thetaxy;
    double m_thetayz;
    double m_thetaxz;
    bool m_support_SH;
    std::string m_diago_lib;
    bool m_support_Periodic_Boundary;
    bool m_multi_parallel_strategies;
    std::string m_points_path;
    std::string m_venergy_path;
    std::string m_distribution_path;
};

#endif
