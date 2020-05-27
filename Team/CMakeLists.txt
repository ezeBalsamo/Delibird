include_directories(include)
set(SOURCES
        src/broker_connection_handler.c
        src/gameboy_connection_handler.c
        src/map.c
        src/team.c
        src/team_deserialization.c
        src/team_logs_manager.c
        src/team_manager.c
        src/team_operations_information.c
        src/trainers_parser.c
        src/goal_calculator.c src/query_performer.c src/appeared_query_performer.c src/localized_query_performer.c src/caught_query_performer.c include/caught_query_performer.h)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/target")
add_executable(team ${SOURCES})

target_link_libraries(team utils)