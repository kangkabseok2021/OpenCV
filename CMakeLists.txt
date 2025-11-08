cmake_minimum_required(VERSION 3.10)
project(ScratchDetector)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Include directories
include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Source files
set(SOURCES
    src/ImageLoader.cpp
    src/ScratchDetector.cpp
    src/ResultVisualizer.cpp
    main.cpp
)

# Create executable
add_executable(ScratchDetector ${SOURCES})

# Link OpenCV libraries
target_link_libraries(ScratchDetector ${OpenCV_LIBS})

# Print OpenCV version (helpful for debugging)
message(STATUS "OpenCV version: ${OpenCV_VERSION}")
message(STATUS "OpenCV libraries: ${OpenCV_LIBS}")