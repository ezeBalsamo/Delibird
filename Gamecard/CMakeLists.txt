include_directories(include)
set(SOURCES
        src/gamecard.c
        src/gamecard_sanitizer.c)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/target")
add_executable(gamecard ${SOURCES})

target_link_libraries(gamecard utils)