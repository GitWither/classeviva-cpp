cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CV_MAIN "classeviva")

# set the project name
project("${CV_MAIN}-cpp")


add_library("${PROJECT_NAME}" "classeviva-cpp/src/Classeviva.cpp" "classeviva-cpp/include/classeviva-cpp/Classeviva.h")
add_executable("${CV_MAIN}-test" "classeviva-test/src/Main.cpp")

target_include_directories("${PROJECT_NAME}" PUBLIC "classeviva-cpp/include/classeviva-cpp")
target_include_directories("${PROJECT_NAME}" PUBLIC "deps")

add_subdirectory("deps/openssl-cmake")
add_subdirectory("deps/cpp-httplib")
add_subdirectory("deps/json")

target_link_libraries("${PROJECT_NAME}" PRIVATE "httplib" "ssl" "crypto" "nlohmann_json::nlohmann_json")
target_link_libraries("${CV_MAIN}-test" PRIVATE "${PROJECT_NAME}")
target_link_libraries("${CV_MAIN}-test" PRIVATE "httplib" "ssl" "crypto" "nlohmann_json::nlohmann_json")