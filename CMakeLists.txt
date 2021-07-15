cmake_minimum_required(VERSION 3.5)
find_package(glfw3 3.3 REQUIRED)
set (CMAKE_CXX_COMPILER "g++")
project(GAME CXX C) 
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")  
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99") 
set(CMAKE_BUILD_TYPE "Debug")


include_directories("/usr/local/include/GLFW/" "/usr/local/include/glad/" "/usr/local/include/KHR/" "include/")
FILE(GLOB SOURCE_FILES "src/*.cpp" "src/*.c" )

add_executable(games ${SOURCE_FILES})

target_link_libraries(games glfw GL X11 pthread Xrandr Xinerama Xcursor Xi dl)
