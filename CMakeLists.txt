cmake_minimum_required(VERSION 3.10)

project(MyProjectName VERSION 1.0)

find_package(OpenMP REQUIRED)

find_package(GSL REQUIRED)

set(OPENBLAS_PATH $ENV{OPENBLAS_PATH})
set(BLAS_INC $ENV{BLAS_INC})
set(BLAS_LIB $ENV{BLAS_LIB})

include_directories(include ${BLAS_INC} ${GSL_INCLUDE_DIRS} ${OPENBLAS_PATH}/include)

link_directories(${OPENBLAS_PATH}/lib)

add_executable(MyExecutable main.cpp src/InputParameters.cpp src/PointCloud.cpp src/Distribution.cpp src/VDistribution.cpp)

target_link_libraries(MyExecutable PUBLIC ${BLAS_LIB} ${GSL_LIBRARIES} OpenMP::OpenMP_CXX m openblas)
