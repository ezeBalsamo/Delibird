include_directories(include)
set(SOURCES
        src/publisher_mode_strategy.c
        src/subscriber_mode_strategy.c
        src/role_mode_strategy.c
        src/entry_point_validator.c
        src/entry_point_executor.c
        src/entry_point_logs_manager.c
        src/entry_point_connection_builder.c
        src/entry_point_processes_information.c
        src/gameboy.c
        src/pokemon_operation_parser.c
        src/appeared_pokemon_operation_parser.c
        src/new_pokemon_operation_parser.c
        src/get_pokemon_operation_parser.c
        src/catch_pokemon_operation_parser.c
        src/caught_pokemon_operation_parser.c
        src/free_gameboy.c)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/target")
add_executable(gameboy ${SOURCES})

target_link_libraries(gameboy utils)