set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/target")

add_executable(config_updater src/config_updater.c)

target_link_libraries(config_updater -lcommons)