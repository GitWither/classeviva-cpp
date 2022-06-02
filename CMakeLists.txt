cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE   ON)

set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")

set(CV_MAIN "classeviva")

project("${CV_MAIN}-cpp")

if (MSVC)
    # use Windows native SSL instead of OpenSSL or MbedTLS
    set(CURL_USE_SCHANNEL ON CACHE INTERNAL "")
else()
    # for curl SSL
    set(ENABLE_TESTING OFF CACHE INTERNAL "")
    set(ENABLE_PROGRAMS OFF CACHE INTERNAL "")
    set(CURL_USE_MBEDTLS ON CACHE INTERNAL "")
    add_subdirectory(deps/mbedtls EXCLUDE_FROM_ALL)
    set(MBEDTLS_LIBRARIES "mbedtls")   
endif()

set(BUILD_TESTING OFF)
set(USE_OPENSSL OFF)
set(CURL_USE_LIBSSH2 OFF)
set(CURL_DISABLE_FTP ON)
set(CURL_DISABLE_FILE ON)
set(CURL_DISABLE_TELNET ON)
set(CURL_DISABLE_LDAP ON)
set(CURL_DISABLE_DICT ON)
set(CURL_DISABLE_TFTP ON)
set(CURL_DISABLE_GOPHER ON)
set(CURL_DISABLE_IMAP ON)
set(CURL_DISABLE_SMB ON)
set(CURL_DISABLE_SMTP ON)
set(CURL_DISABLE_RTSP ON)
set(CURL_DISABLE_MQTT ON)
set(CURL_DISABLE_POP3 ON)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/deps/curl" EXCLUDE_FROM_ALL)

set(JSON_Install ON)
set(JSON_MultipleHeaders ON)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/deps/json")


add_library("${PROJECT_NAME}" "${PROJECT_NAME}/src/Classeviva.cpp" "${PROJECT_NAME}/include/${PROJECT_NAME}/Classeviva.h")


target_include_directories("${PROJECT_NAME}" PRIVATE "${PROJECT_NAME}/include/${PROJECT_NAME}" PUBLIC "${PROJECT_NAME}/include")
target_include_directories("${PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/deps/json/single_include")

target_link_libraries("${PROJECT_NAME}" PRIVATE "CURL::libcurl" "nlohmann_json")

add_executable("${CV_MAIN}-test" "${CV_MAIN}-test/src/Main.cpp")
target_link_libraries("${CV_MAIN}-test" PRIVATE "${PROJECT_NAME}")
