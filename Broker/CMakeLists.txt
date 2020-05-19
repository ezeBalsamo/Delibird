include_directories(include)
file(GLOB SOURCES "src/*.c")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/target")
add_executable(broker ${SOURCES})

target_link_libraries(broker utils)