#include <messages_roles.h>
#include <connection_handler.h>
#include <publisher.h>
#include <publisher_message_mode.h>
#include <broker_memory_manager.h>
#include <pthread.h>
#include "../../Utils/include/socket.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/pthread_wrapper.h"

t_message_role* publisher_message_mode;
pthread_mutex_t mutex_for_id_and_allocation;
pthread_mutex_t message_status_mutex;

t_message_role* publisher_mode(){
    return publisher_message_mode;
}

bool publisher_mode_can_handle(uint32_t operation){
    return operation != SUBSCRIBE_ME;
}

void publisher_mode_attending_message_function(t_connection_deserialization_information* connection_deserialization_information){

    t_deserialization_information* deserialization_information = connection_deserialization_information -> deserialization_information;

    safe_mutex_lock(&mutex_for_id_and_allocation);
    uint32_t message_id = update_and_get_message_id();
    allocate_message_using(message_id, deserialization_information);

    send_ack_message(message_id, connection_deserialization_information -> socket_fd);
    t_message_status* message_status = create_message_status_using(message_id, deserialization_information);

    push_to_queue(message_status);
    safe_mutex_unlock(&mutex_for_id_and_allocation);

    free_connection_deserialization_information(connection_deserialization_information);
}

void initialize_publisher_message_mode(){
    publisher_message_mode = safe_malloc(sizeof(t_message_role));
    publisher_message_mode -> can_handle_function  = publisher_mode_can_handle;
    publisher_message_mode -> attending_message_function = publisher_mode_attending_message_function;

    safe_mutex_initialize(&mutex_for_id_and_allocation);
    safe_mutex_initialize(&message_status_mutex);
//    consider_as_garbage(&mutex_for_id_and_allocation, (void (*)(void *)) safe_mutex_destroy);
}