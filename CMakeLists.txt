cmake_minimum_required(VERSION 3.17)
project(MandelbrotViewer)

#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")

set(CMAKE_CXX_STANDARD 17)

set(PROJECT_NAME MandelbrotViewer)

add_executable(${PROJECT_NAME} main.cpp gui/GUI.cpp gui/GUI.h gui/Widget.cpp gui/Widget.h gui/ImageButton.cpp gui/ImageButton.h gui/AssetManager.cpp gui/AssetManager.h MandelbrotRenderer.cpp MandelbrotRenderer.h gui/ThemeSwitcher.cpp gui/ThemeSwitcher.h)

set(SFML_STATIC_LIBRARIES TRUE)

IF (WIN32)
    set(SFML_ROOT_DIR "C:/SFML")
    set(SFML_DIR "${SFML_ROOT_DIR}/lib/cmake/SFML")
    find_package(SFML REQUIRED COMPONENTS window graphics)
    target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-window)
ELSE()
    target_link_libraries(${PROJECT_NAME} PRIVATE sfml-system sfml-graphics sfml-window)
ENDIF()

file(COPY images DESTINATION ${CMAKE_BINARY_DIR})

file(COPY mandelbrot.frag DESTINATION ${CMAKE_BINARY_DIR})





