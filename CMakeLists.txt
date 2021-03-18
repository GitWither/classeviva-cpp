cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE   ON)
set(CV_MAIN "classeviva")

# set the project name
project("${CV_MAIN}-cpp")

add_library("${PROJECT_NAME}" "${PROJECT_NAME}/src/Classeviva.cpp" "${PROJECT_NAME}/include/${PROJECT_NAME}/Classeviva.h")
add_executable("${CV_MAIN}-test" "${CV_MAIN}-test/src/Main.cpp")

target_compile_definitions("${PROJECT_NAME}" PRIVATE CPPHTTPLIB_OPENSSL_SUPPORT)

target_include_directories("${PROJECT_NAME}" PUBLIC "${PROJECT_NAME}/include/${PROJECT_NAME}")
target_include_directories("${PROJECT_NAME}" PUBLIC "deps")

add_subdirectory("deps/openssl-cmake")
add_subdirectory("deps/cpp-httplib")
add_subdirectory("deps/json")

target_link_libraries("${PROJECT_NAME}" PUBLIC "httplib" "ssl" "crypto" "nlohmann_json::nlohmann_json")
target_link_libraries("${CV_MAIN}-test" PRIVATE "${PROJECT_NAME}")
