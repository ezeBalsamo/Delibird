include_directories(include)
set(SOURCES
        src/broker.c
        src/broker_deserialization.c
        src/queue_message_manager.c
        src/connection_handler.c
        src/broker_logs_manager.c
        src/free_broker.c)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/target")
add_executable(broker ${SOURCES})

target_link_libraries(broker utils)