#include <stdlib.h>
#include <free_broker.h>
#include "../include/queue_message_status.h"
#include "../../Utils/include/common_structures.h"

t_message_status* create_message_status_for(t_identified_message* identified_message){
    t_message_status* message_status = safe_malloc(sizeof(t_message_status));
    message_status -> identified_message = identified_message;
    message_status -> subscribers_to_send = list_create();
    message_status -> subscribers_who_received = list_create();

    return message_status;
}

void move_subscriber_to_ACK(t_message_status* message_status, int subscriber){

    bool equals_subscribers_(void* another_subscriber){
        return subscriber == *((int*) another_subscriber);
    }

    void* subscriber_found =
            list_remove_by_condition(message_status -> subscribers_to_send, (bool (*)(void *)) equals_subscribers_);

    if(!subscriber_found){
        //TODO log
        free_system();
    }

    list_add(message_status -> subscribers_who_received, subscriber_found);

}

void free_message_status(t_message_status* message_status){
    free_identified_message(message_status -> identified_message);
    list_destroy_and_destroy_elements(message_status -> subscribers_who_received, free);
    list_destroy_and_destroy_elements(message_status ->subscribers_to_send, free);
    free(message_status);
}
