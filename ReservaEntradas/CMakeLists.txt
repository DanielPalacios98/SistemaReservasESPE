cmake_minimum_required(VERSION 3.21)
project(ReservaEntradas LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(USE_MONGO "Enable MongoDB" ON)
set(USE_WX ON)

file(GLOB SOURCES "src/*.cpp")

add_executable(ReservaEntradas WIN32 ${SOURCES})

target_include_directories(ReservaEntradas PRIVATE include)

target_compile_definitions(ReservaEntradas PRIVATE USE_WX)

if(USE_MONGO)
    target_compile_definitions(ReservaEntradas PRIVATE USE_MONGO)
    find_package(mongocxx CONFIG REQUIRED)
    target_link_libraries(ReservaEntradas PRIVATE mongo::mongocxx_shared)
endif()

find_package(wxWidgets CONFIG REQUIRED COMPONENTS core base)
target_link_libraries(ReservaEntradas PRIVATE wx::core wx::base)
