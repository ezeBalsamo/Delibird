#include <stdlib.h>
#include <pthread.h>
#include <broker_logs_manager.h>
#include <subscriber.h>
#include <broker_memory_manager.h>
#include "../include/queue_message_status.h"
#include "../../Utils/include/common_structures.h"
#include "../../Utils/include/socket.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/t_list_extension.h"
#include "../../Utils/include/queue_code_name_associations.h"
#include "../../Utils/include/operation_deserialization.h"
#include "../../Utils/include/serializable_objects.h"
#include "../../Utils/include/pthread_wrapper.h"
#include <deserialization_information_content_provider.h>

extern pthread_mutex_t message_status_mutex;

t_message_status* create_message_status_using(uint32_t message_id, t_deserialization_information* deserialization_information){

    t_message_status* message_status = safe_malloc(sizeof(t_message_status));
    message_status -> message_id = message_id;
    message_status -> operation_queue = operation_queue_using(deserialization_information);
    message_status -> subscribers_to_send = list_create();
    message_status -> subscribers_who_received = list_create();

    return message_status;
}

void delete_message(uint32_t operation_message, uint32_t message_id, char* reason){

    t_queue_context* queue_context = queue_context_of_queue_named(queue_name_of(operation_message));

    bool _message_status_with_message_id(t_message_status* message_status){
        return message_status -> message_id  == message_id;
    }

    safe_mutex_lock(&message_status_mutex);
    t_message_status* message_status = list_remove_by_condition(queue_context -> messages, (bool (*) (void*)) _message_status_with_message_id);
    safe_mutex_unlock(&message_status_mutex);

    if(message_status == NULL){
        log_message_status_not_found_in_queue_error(message_id);    //PUEDE PASAR QUE SE QUIERA BORRAR ALGO QUE YA SE BORRO DE LA LISTA DE SUBSCRIPTORES POR HABERSE ENVIADO A TODOS
    }else{
        log_succesful_deleting_message_of_a_queue(message_id, reason);
        free_message_status(message_status);
    }
}

void delete_message_if_necessary(t_message_status* message_status, t_list* acks){


    bool _is_not_a_failed_ack(void* ack){
        uint32_t cast_ack = *((uint32_t *) ack);
        return cast_ack != FAILED_ACK;
    }

    bool can_be_erased = list_all_satisfy(acks, _is_not_a_failed_ack);

    if (can_be_erased){
        uint32_t operation = message_status -> operation_queue;
        delete_message(operation, message_status -> message_id,
                       "Se le envió a todos los suscriptores de la cola.");
    }
}

void* join_reception_for_ack_thread(pthread_t waiting_for_ack_thread, t_subscriber_context* subscriber_context, t_message_status* message_status){

    void *subscriber_ack;
    pthread_join(waiting_for_ack_thread, &subscriber_ack);

    uint32_t expected_ack = message_status -> message_id;

    uint32_t cast_subscriber_ack = *((uint32_t *) subscriber_ack);

    if (cast_subscriber_ack == FAILED_ACK){
        log_failed_to_receive_ack_error(subscriber_context);
        log_subscriber_disconnection(subscriber_context);
        set_inactive_connection_for(subscriber_context);

    } else {
        subscriber_context -> last_message_id_received = expected_ack;
        move_subscriber_to_ACK(message_status, subscriber_context);
        log_succesful_message_received_by(subscriber_context, message_status -> message_id);
    }

    return subscriber_ack;
}

void* receive_ack_thread(void* subscriber_fd){
    int cast_subscriber_fd = *((int *) subscriber_fd);
    int timeout_in_seconds = config_get_int_at("ACK_TIMEOUT");

    return receive_ack_with_timeout_in_seconds(cast_subscriber_fd, timeout_in_seconds);
}

void move_subscriber_to_ACK(t_message_status* message_status, t_subscriber_context* subscriber_context){

    bool _are_equivalents_subscribers(t_subscriber_context* subscriber_to_compare){
        return are_equivalent_subscribers(subscriber_context, subscriber_to_compare);
    }

    void* subscriber_found =
            list_remove_by_condition(message_status -> subscribers_to_send, (bool (*)(void *)) _are_equivalents_subscribers);

    if(!subscriber_found){
        log_subscriber_not_found_in_message_status_subscribers_error(subscriber_context, message_status -> message_id);
        free_system();
    }

    list_add(message_status -> subscribers_who_received, (void*) subscriber_found);
}

void free_message_status(t_message_status* message_status){
    list_destroy(message_status -> subscribers_who_received);
    list_destroy(message_status -> subscribers_to_send);
    free(message_status);
}