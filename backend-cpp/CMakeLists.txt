cmake_minimum_required(VERSION 3.15)
project(AlumniVerseBackend VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Definitions
add_definitions(-DCROW_FILESYSTEM_IS_EXPERIMENTAL)
add_definitions(-DASIO_STANDALONE)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/vendor
)

# Core sources
set(CORE_SOURCES
    src/graph/AlumniGraph.cpp
    src/graph/AlumniTree.cpp
    src/repositories/DataRepository.cpp
    src/services/AuthService.cpp
    src/services/GraphService.cpp
    src/services/TreeService.cpp
)

# Main executable
add_executable(alumniverse_backend
    src/main.cpp
    ${CORE_SOURCES}
)

if(WIN32)
    target_link_libraries(alumniverse_backend PRIVATE ws2_32 mswsock)
    target_compile_options(alumniverse_backend PRIVATE -static-libgcc -static-libstdc++)
    target_link_options(alumniverse_backend PRIVATE -static-libgcc -static-libstdc++)
else()
    target_link_libraries(alumniverse_backend PRIVATE pthread)
endif()

# Unit tests executable
add_executable(graph_tests
    tests/GraphTests.cpp
    ${CORE_SOURCES}
)

if(WIN32)
    target_link_libraries(graph_tests PRIVATE ws2_32 mswsock)
    target_compile_options(graph_tests PRIVATE -static-libgcc -static-libstdc++)
    target_link_options(graph_tests PRIVATE -static-libgcc -static-libstdc++)
else()
    target_link_libraries(graph_tests PRIVATE pthread)
endif()
