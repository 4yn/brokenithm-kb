find_package(unofficial-libuv CONFIG REQUIRED)

project(uws C CXX)

set(INCROOT ${CMAKE_CURRENT_SOURCE_DIR}/include/uws)
set(SRCROOT ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB_RECURSE INC "${INCROOT}/*.h")

file(GLOB_RECURSE SRC "${SRCROOT}/*.c" "${SRCROOT}/*.h")

add_library(uws ${SRC} ${INC})

target_compile_features(uws PUBLIC cxx_std_17)
target_compile_definitions(uws PUBLIC UWS_NO_ZLIB LIBUS_NO_SSL)

target_include_directories(uws PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(uws PRIVATE ${INCROOT} ${SRCROOT})

target_link_libraries(uws PRIVATE unofficial::libuv::libuv)
