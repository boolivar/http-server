project(final)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

set_property(TARGET final PROPERTY CXX_STANDARD 11)
set_property(TARGET final PROPERTY CXX_STANDARD_REQUIRED ON)

set(Boost_USE_MULTITHREADED ON)
find_package(Boost COMPONENTS system regex REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ws2_32 wsock32)
