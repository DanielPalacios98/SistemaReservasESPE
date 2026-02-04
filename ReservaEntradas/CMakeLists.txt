cmake_minimum_required(VERSION 3.21)
project(ReservaEntradas LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Opciones para habilitar integraciones
option(USE_MONGO "Enable MongoDB repository (mongocxx)" OFF)
# wxWidgets siempre requerido ahora
set(USE_WX ON)

# Fuentes comunes
set(LIB_SRC
    lib/Reserva.cpp
    lib/NodoReserva.cpp
    lib/ListaReserva.cpp
    lib/JsonReservaRepository.cpp
    lib/MongoReservaRepository.cpp
    lib/Config.cpp
    lib/Log.cpp
)

# Build GUI app
add_executable(ReservaEntradas WIN32
    ${LIB_SRC}
    gui/App.cpp
    gui/MainFrame.cpp
    gui/ReservaDialog.cpp
)
target_compile_definitions(ReservaEntradas PRIVATE USE_WX)

target_include_directories(ReservaEntradas PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/include)

# Definiciones para controlar código por flags
if(USE_MONGO)
    target_compile_definitions(ReservaEntradas PRIVATE USE_MONGO)
endif()

# Integración con vcpkg (opcional): mongocxx y wxwidgets
if(USE_MONGO)
    find_package(mongocxx CONFIG REQUIRED)
    target_link_libraries(ReservaEntradas PRIVATE mongo::mongocxx_shared)
endif()

find_package(wxWidgets CONFIG REQUIRED COMPONENTS core base)
target_link_libraries(ReservaEntradas PRIVATE wx::core wx::base)

# Tests basicos (opcional)
option(BUILD_TESTS "Build unit tests" ON)
if(BUILD_TESTS)
    add_executable(ReservaEntradasTests
        lib/Reserva.cpp
        lib/NodoReserva.cpp
        lib/ListaReserva.cpp
        lib/Config.cpp
        lib/Log.cpp
        tests/ListaReservaTests.cpp
    )
    target_include_directories(ReservaEntradasTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/include)
endif()
