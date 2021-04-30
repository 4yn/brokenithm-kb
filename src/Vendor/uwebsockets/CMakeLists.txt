find_package(ZLIB REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(unofficial-libuv CONFIG REQUIRED)

project(uws C CXX)

set(INCROOT ${CMAKE_CURRENT_SOURCE_DIR}/include/uws)
set(SRCROOT ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB_RECURSE INC "${INCROOT}/*.h")

file(GLOB_RECURSE SRC "${SRCROOT}/*.c" "${SRCROOT}/*.h")

add_library(uws ${SRC} ${INC})

target_compile_features(uws PUBLIC cxx_std_17)
target_compile_definitions(uws PRIVATE "-DLIBUS_USE_OPENSSL")

target_include_directories(uws PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(uws PRIVATE ${INCROOT} ${SRCROOT})

target_link_libraries(uws PUBLIC OpenSSL::SSL OpenSSL::Crypto ZLIB::ZLIB)
target_link_libraries(uws PRIVATE unofficial::libuv::libuv)
