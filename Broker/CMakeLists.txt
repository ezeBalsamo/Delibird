include_directories(include)
set(SOURCES
        src/broker.c
        src/queue_context_provider.c
        src/connection_handler.c
        src/broker_logs_manager.c
        src/broker_sanitizer.c
        src/message_role_identifier.c
        src/publisher_message_mode.c
        src/subscriber_message_mode.c
        src/subscriber.c
        src/queue_message_status.c
        src/publisher.c
        src/queue_context_operations.c
        src/subscriber_context_provider.c
        src/broker_memory_manager.c
        src/dynamic_partition_message_allocator.c
        src/broker_memory_algorithms.c
        src/first_fit_search_parition_algorithm.c
        src/fifo_free_partition_algorithm.c)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/target")
add_executable(broker ${SOURCES})

target_link_libraries(broker utils)