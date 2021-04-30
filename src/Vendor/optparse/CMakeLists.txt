project(optparse LANGUAGES CXX)
add_library(optparse INTERFACE)
target_include_directories(optparse INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
