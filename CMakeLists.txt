cmake_minimum_required (VERSION 3.8)

project ("server")

include(conanbuildinfo.cmake)
conan_basic_setup()

add_executable (server "main.cpp" "Server.h" "Server.cpp" "RequestHandler.cpp" "RequestHandler.h" "dataBaseHandler.h" "dataBaseHandler.cpp")

target_link_libraries(server ${CONAN_LIBS})
