include_directories(include)
set(SOURCES
        src/gamecard.c
        src/gamecard_sanitizer.c
        src/gamecard_logs_manager.c
        src/gamecard_serializable_objects.c
        src/gamecard_query_performer.c
        src/gamecard_configuration_manager.c
        src/filesystem.c
        src/gameboy_connection_handler.c
        src/broker_connection_handler.c
        src/new_query_performer.c
        src/catch_query_performer.c
        src/get_query_performer.c)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/target")
add_executable(gamecard ${SOURCES})

target_link_libraries(gamecard utils)