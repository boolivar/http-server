project(final)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(Boost COMPONENTS system regex REQUIRED)
find_package(OpenMP)

include_directories(${Boost_INCLUDE_DIRS})
if (OPENMP_FOUND)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

if(WIN32)
    set(ADDITIONAL_LIBRARIES ws2_32 wsock32)
else()
    set(ADDITIONAL_LIBRARIES "")
endif()

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${ADDITIONAL_LIBRARIES})

