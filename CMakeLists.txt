cmake_minimum_required (VERSION 3.6)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download" )
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download" )

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This adds
# the following targets: gtest, gtest_main, gmock
# and gmock_main
add_subdirectory("${CMAKE_BINARY_DIR}/googletest-src"
                 "${CMAKE_BINARY_DIR}/googletest-build")

# The gtest/gmock targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if(CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include"
                        "${gmock_SOURCE_DIR}/include")
endif()

set(TESTS_TARGET mdga_tests)
project (man-dont-get-angry)
#add_custom_target (${TESTS_TARGET})

set(CMAKE_CXX_STANDARD 17)

find_package(Curses REQUIRED)
find_package(Boost REQUIRED)

include_directories(${CURSES_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/Include)

add_executable(man-dont-get-angry main.cpp)
target_link_libraries(man-dont-get-angry ${CURSES_LIBRARIES})



file(GLOB TESTS_SOURCE_FILES ${PROJECT_SOURCE_DIR}/Tests/*.cpp)

add_executable(${TESTS_TARGET} ${TESTS_SOURCE_FILES})

add_dependencies(${TESTS_TARGET} gtest)
add_dependencies(${TESTS_TARGET} gmock)
add_dependencies(${TESTS_TARGET} gtest_main)
add_dependencies(${TESTS_TARGET} gmock_main)
target_link_libraries(${TESTS_TARGET} gtest_main)



#enable_testing()

