cmake_minimum_required(VERSION 3.17)
project(MandelbrotViewer)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")

set(CMAKE_CXX_STANDARD 14)

set(PROJECT_NAME MandelbrotViewer)
set(SFML_ROOT_DIR "C:/SFML")
set(TGUI_ROOT_DIR "C:/TGUI")
set(SFML_DIR "${SFML_ROOT_DIR}/lib/cmake/SFML")
set(TGUI_DIR "${TGUI_ROOT_DIR}/lib/cmake/TGUI")

set(SFML_STATIC_LIBRARIES TRUE)
set(TGUI_STATIC_LIBRARIES TRUE)

find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)

add_executable(${PROJECT_NAME} main.cpp mandelbrot.cpp mandelbrot.h)

if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} sfml-audio sfml-network sfml-graphics sfml-window sfml-system)
endif()

include_directories(${TGUI_ROOT_DIR}/include)




