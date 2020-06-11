include_directories(include)
set(SOURCES
        src/broker.c
        src/queue_context_provider.c
        src/connection_handler.c
        src/broker_logs_manager.c
        src/free_broker.c
        src/message_role_identifier.c
        src/publisher_message_mode.c
        src/subscriber_message_mode.c
        src/subscriber.c
        src/queue_message_status.c
        src/publisher.c
        src/queue_context_operations.c
        src/subscriber_context_provider.c)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/target")
add_executable(broker ${SOURCES})

target_link_libraries(broker utils)