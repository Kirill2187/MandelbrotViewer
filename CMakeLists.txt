cmake_minimum_required(VERSION 3.17)
project(MandelbrotViewer)

#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")

set(CMAKE_CXX_STANDARD 17)

set(PROJECT_NAME MandelbrotViewer)

add_executable(${PROJECT_NAME} main.cpp)

IF (WIN32)
    set(SFML_STATIC_LIBRARIES TRUE)
    set(TGUI_STATIC_LIBRARIES TRUE)
    set(SFML_ROOT_DIR "C:/SFML")
    set(TGUI_ROOT_DIR "C:/TGUI")
    set(SFML_DIR "${SFML_ROOT_DIR}/lib/cmake/SFML")
    set(TGUI_DIR "${TGUI_ROOT_DIR}/lib/cmake/TGUI")
    find_package(SFML REQUIRED COMPONENTS window graphics)
    find_package(TGUI REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window tgui)
ELSE()
    set(SFML_STATIC_LIBRARIES TRUE)
    set(TGUI_ROOT_DIR "/home/kirill/TGUI/build")
    set(TGUI_DIR "${TGUI_ROOT_DIR}")
    find_package(TGUI REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE sfml-system sfml-graphics sfml-window tgui)
ENDIF()

file(COPY theme DESTINATION ${CMAKE_BINARY_DIR})
file(COPY images DESTINATION ${CMAKE_BINARY_DIR})

file(COPY mandelbrot.frag DESTINATION ${CMAKE_BINARY_DIR})





