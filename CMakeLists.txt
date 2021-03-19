cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE   ON)
set(CV_MAIN "classeviva")

project("${CV_MAIN}-cpp")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/deps/openssl-cmake")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/deps/cpp-httplib")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/deps/json")

add_library("${PROJECT_NAME}" "${PROJECT_NAME}/src/Classeviva.cpp" "${PROJECT_NAME}/include/${PROJECT_NAME}/Classeviva.h")

target_compile_definitions("${PROJECT_NAME}" PRIVATE CPPHTTPLIB_OPENSSL_SUPPORT)

target_include_directories("${PROJECT_NAME}" PRIVATE "${PROJECT_NAME}/include/${PROJECT_NAME}" PUBLIC "${PROJECT_NAME}/include")
target_include_directories("${PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/deps/cpp-httplib" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/deps/json/single_include")

target_link_libraries("${PROJECT_NAME}" PRIVATE "crypto" "ssl" "httplib" "nlohmann_json")

add_executable("${CV_MAIN}-test" "${CV_MAIN}-test/src/Main.cpp")
target_link_libraries("${CV_MAIN}-test" PRIVATE "${PROJECT_NAME}")
