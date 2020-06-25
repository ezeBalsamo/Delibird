include_directories(include)
set(SOURCES
        src/common_structures.c
        src/configuration_manager.c
        src/logger.c
        src/matrix.c
        src/operation_deserialization.c
        src/operation_serialization.c
        src/serializable_objects.c
        src/pthread_wrapper.c
        src/socket.c
        src/t_list_extension.c
        src/paths.c
        src/pretty_printer.c
        src/queue_code_name_associations.c
        src/general_logs.c
        src/chained_evaluation.c
        src/garbage_collector.c)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/target")
add_library(utils SHARED ${SOURCES})

target_link_libraries(utils -lcommons pthread)