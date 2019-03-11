cmake_minimum_required(VERSION 3.1)
project(GBSWebAPI)

set(CMAKE_CXX_STANDARD 11)

find_package(libmongocxx REQUIRED)

## include directories

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hpp)

## link libs

find_package(oatpp          0.19.1 REQUIRED)
find_package(oatpp-swagger  0.19.1 REQUIRED)

add_library(GBSWebAPI ${sources} ${headers})
target_include_directories(GBSWebAPI PUBLIC include ${LIBMONGOCXX_INCLUDE_DIRS})
target_link_libraries(GBSWebAPI
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-swagger
)

## define path to swagger-ui res folder

add_definitions(-DOATPP_SWAGGER_RES_PATH="${oatpp-swagger_INCLUDE_DIRS}/../bin/oatpp-swagger/res")


## add executables

add_executable(apiServer main.cpp ${sources} ${headers})
target_link_libraries(apiServer GBSWebAPI ${LIBMONGOCXX_LIBRARIES})
