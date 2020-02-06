cmake_minimum_required(VERSION 3.1)

set(project_name beam-diagnosis) ## rename your project here

project(${project_name})

set(CMAKE_CXX_STANDARD 11)

# Making json file for auto complete
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(libmongocxx REQUIRED)

add_library(${project_name}-lib
        src/AppComponent.hpp
        src/SwaggerComponent.hpp
        src/controller/MyController.cpp
        src/controller/MyController.hpp
        src/dto/DTOs.hpp
        src/db/TDBHandler.cpp
        src/db/TDBHandler.hpp
)

## link libs

find_package(oatpp 1.0.0 REQUIRED)
find_package(oatpp-swagger  1.0.0 REQUIRED)

target_link_libraries(${project_name}-lib
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-swagger
        ${LIBMONGOCXX_LIBRARIES}
)

add_definitions(-DOATPP_SWAGGER_RES_PATH="${oatpp-swagger_INCLUDE_DIRS}/../bin/oatpp-swagger/res")

target_include_directories(${project_name}-lib PUBLIC src ${LIBMONGOCXX_INCLUDE_DIRS})

## add executables

add_executable(${project_name}-exe
        src/App.cpp)
target_link_libraries(${project_name}-exe ${project_name}-lib)
add_dependencies(${project_name}-exe ${project_name}-lib)

set_target_properties(${project_name}-lib ${project_name}-exe PROPERTIES
        CXX_STANDARD 11
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON
)
