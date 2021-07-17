cmake_minimum_required(VERSION 3.5)

set (CMAKE_CXX_COMPILER "g++")
project(GAME CXX C) 
set(CMAKE_SUPPRESS_REGENERATION FALSE)
IF (UNIX)  
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")  
ELSE()
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ")  
ENDIF ()

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99") 
set(CMAKE_BUILD_TYPE "Debug")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMakeTargets")

IF (WIN32)  
    link_directories("lib/win/")
ELSEIF(UNIX)
    link_directories("lib/linux/")
ENDIF()


include_directories("include/GLFW/" "include/glad/" "include/KHR/" "include/")
FILE(GLOB SOURCE_FILES "src/*.cpp" "src/*.c" "include/*.hpp" "include/*.h")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./build/)
add_executable( games ${SOURCE_FILES})

    IF (WIN32)  
        target_link_libraries(games glfw3) 
    ELSEIF (APPLE)  
        MESSAGE(STATUS "system dismatch")  
    ELSEIF (UNIX)  
        target_link_libraries(games glfw GL X11 pthread Xrandr Xinerama Xcursor Xi dl)  
    ENDIF () 


