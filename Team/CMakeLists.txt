add_library(team_manager src/team_manager.c include/team_manager.h)
target_link_libraries(team_manager -lcommons configuration_manager common_structures)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/target")
add_executable(team src/team.c)

target_link_libraries(team sockets configuration_manager operation_deserialization processes_information team_manager)