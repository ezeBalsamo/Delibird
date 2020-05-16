add_library(team_logs_manager src/team_logs_manager.c include/team_logs_manager.h)
add_library(team_manager src/team_manager.c include/team_manager.h)
add_library(map src/map.c include/map.h)
add_library(broker_connection_handler src/broker_connection_handler.c include/broker_connection_handler.h)
add_library(gameboy_connection_handler src/gameboy_connection_handler.c include/gameboy_connection_handler.h)

target_link_libraries(team_logs_manager logger)
target_link_libraries(team_manager -lcommons configuration_manager common_structures t_list_extension)
target_link_libraries(map -lcommons team_manager matrix)
target_link_libraries(broker_connection_handler configuration_manager sockets team_manager -lcommons -pthread)
target_link_libraries(gameboy_connection_handler configuration_manager sockets)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/target")
add_executable(team src/team.c)

target_link_libraries(team configuration_manager operation_deserialization processes_information
        team_logs_manager team_manager map pthread_wrapper
        broker_connection_handler gameboy_connection_handler)