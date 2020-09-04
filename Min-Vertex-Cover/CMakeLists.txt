cmake_minimum_required(VERSION 2.8)
project(ece650-prj)

if (EXISTS "${CMAKE_SOURCE_DIR}/minisat")
  add_subdirectory(minisat)
else()
  message (FATAL_ERROR "MiniSat not found. Use `git clone https://github.com/agurfinkel/minisat` to check it out.")
endif()


## ECE650 Specific configuration start here
## Project specific configurations go here
find_package(Threads)
# enable c++11 support
set (CMAKE_CXX_FLAGS "-std=c++11 -Wall ${CMAKE_CXX_FLAGS}")
set (THREADS_PREFER_PTHREAD_FLAG ON)
# expose minisat header files
include_directories(${CMAKE_SOURCE_DIR}/minisat)

# create the main executable
## add additional .cpp files if needed
add_executable(ece650-prj ece650-prj.cpp)
add_executable(ece650-prj-rff ece650-prj-rff.cpp)

# link libraries
target_link_libraries(ece650-prj minisat-lib-static)
target_link_libraries(ece650-prj ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(ece650-prj-rff minisat-lib-static)
target_link_libraries(ece650-prj-rff ${CMAKE_THREAD_LIBS_INIT})


