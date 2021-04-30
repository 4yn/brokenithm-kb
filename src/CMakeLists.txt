add_subdirectory(Vendor)

set(SRCROOT ${CMAKE_CURRENT_SOURCE_DIR}/src/)

file(GLOB SRC "${SRCROOT}/*.cpp" "${SRCROOT}/*.hpp" "${SRCROOT}/*.rc")
source_group("Sources" FILES ${SRC})

message("${SRCROOT}")

add_executable(brokenithm-kb ${SRC})

target_compile_features(brokenithm-kb PRIVATE cxx_std_17)
target_include_directories(brokenithm-kb PRIVATE ${SRCROOT})

target_link_libraries(brokenithm-kb PRIVATE uws optparse spdlog)

add_custom_command(
    TARGET brokenithm-kb POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res/ $<TARGET_FILE_DIR:brokenithm-kb>/res/
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/scripts/ $<TARGET_FILE_DIR:brokenithm-kb>/
)
