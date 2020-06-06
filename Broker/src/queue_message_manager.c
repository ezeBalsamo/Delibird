#include "../include/queue_message_manager.h"
#include "commons/collections/queue.h"
#include "../../Utils/include/socket.h"
#include "../include/broker_logs_manager.h"
#include "../../Utils/include/queue_code_name_associations.h"
#include "subscribers_manager.h"
#include <stdlib.h>
#include <free_broker.h>
#include <commons/collections/dictionary.h>
#include <publish_message_mode.h>
#include "../../Utils/include/pthread_wrapper.h"

t_queue* new_queue;
t_queue* appeared_queue;
t_queue* get_queue;
t_queue* localized_queue;
t_queue* catch_queue;
t_queue* caught_queue;

t_dictionary* queue_dictionary;


void initialize_messages_queue(){
    new_queue = queue_create();
    appeared_queue = queue_create();
    get_queue = queue_create();
    localized_queue = queue_create();
    catch_queue = queue_create();
    caught_queue = queue_create();
}

void initialize_queue_dictionary(){
    queue_dictionary = dictionary_create();
    dictionary_put(queue_dictionary,"NEW_POKEMON", (void*) new_queue);
    dictionary_put(queue_dictionary,"APPEARED_POKEMON", (void*) appeared_queue);
    dictionary_put(queue_dictionary,"GET_POKEMON", (void*) get_queue);
    dictionary_put(queue_dictionary,"LOCALIZED_POKEMON", (void*) localized_queue);
    dictionary_put(queue_dictionary,"CATCH_POKEMON", (void*) catch_queue);
    dictionary_put(queue_dictionary,"CAUGHT_POKEMON", (void*) caught_queue);
}

void initialize_queue_message_manager(){
    initialize_messages_queue();
    initialize_queue_dictionary();

    log_succesful_initialize_queue_message_manager();
}

t_request* create_request_id(t_message_status* message_status){
    t_request* request = safe_malloc(sizeof(t_request));
    request -> operation = IDENTIFIED_MESSAGE;
    request -> structure = message_status -> identified_message;
    request -> sanitizer_function = (void (*)(void *)) free_identified_message;

    return request;
}

void get_and_update_subscribers_to_send(t_message_status* message_status, uint32_t operation){

    t_list* subscribers_of_a_queue = get_subscribers_of_a_queue(operation);
    list_add_all(message_status -> subscribers_to_send, subscribers_of_a_queue);
}

t_queue* get_queue_of(uint32_t operation){

    char* queue_name = queue_name_of(operation);
    void* queue = dictionary_get(queue_dictionary, queue_name);
    return (t_queue*) queue;
}

bool equals_message_status_(t_message_status* message_status, t_message_status* another_message_status){
    return message_status -> identified_message == another_message_status -> identified_message &&
           message_status -> subscribers_to_send == another_message_status -> subscribers_to_send &&
           message_status -> subscribers_who_received == another_message_status -> subscribers_who_received;
}

void push_to_queue(t_message_status* message_status){

    uint32_t operation = internal_operation_in(message_status -> identified_message);

    if(operation == IDENTIFIED_MESSAGE){ //caso en donde es un identified con otro identified adentro.
    uint32_t internal_operation = internal_operation_in_correlative(message_status->identified_message);
    operation = internal_operation;
    }

    get_and_update_subscribers_to_send(message_status, operation);
    log_succesful_get_and_update_subscribers_to_send(message_status -> identified_message);

    t_queue* queue = get_queue_of(operation);

    queue_push(queue, message_status);
    log_succesful_new_message_pushed_to_a_queue(message_status -> identified_message);

    publish(message_status);

    //TODO ESTO ES LA LOGICA DE BORRAR MENSAJE EN LA COLA, VER MAS ADELANTE EN QUE MOMENTO SE DEBERIA DE HACER ESTO.
    //Ahora se esta haciendo cuando el message_status ya no tiene suscriptores a los cuales enviarle.
    if(list_is_empty(message_status -> subscribers_to_send)){ //si el mensaje fue enviado a todos los suscriptores se borra.
        for(int i = 0; i < list_size(queue -> elements); i++){
            t_message_status* message_status_to_compare = (t_message_status*) list_get(queue->elements, i);
            if(equals_message_status_(message_status, message_status_to_compare)){
                list_remove(queue -> elements, i);
                free_message_status(message_status);
            }
        }
    }
}

void publish(t_message_status* message_status){

    t_list* subscribers = message_status -> subscribers_to_send;
    t_request* request = create_request_id(message_status);

    void _send_message(void* subscriber){
        int cast_subscriber = *((int*) subscriber);
        serialize_and_send_structure(request, cast_subscriber);
        log_succesful_message_sent_to_a_suscriber(request); //loguea por cada suscriptor al cual se el fue enviado el mensaje.

        void* ack;
        pthread_t waiting_for_ack_thread = default_safe_thread_create(receive_ack_message, subscriber);
        pthread_join(waiting_for_ack_thread, &ack);
        uint32_t cast_received_ack = *((uint32_t*) ack);
        if(cast_received_ack != message_status -> identified_message -> message_id){
            log_ack_received_error();
        }
        move_subscriber_to_ACK(message_status, cast_subscriber);
    }

    if(list_is_empty(subscribers)){
            log_no_subscribers_for_request(message_status -> identified_message);
    } else {
            list_iterate(subscribers, _send_message);
            log_succesful_message_sent_to_suscribers(message_status -> identified_message);
    }
}

void free_queue_dictionary(){
    dictionary_destroy_and_destroy_elements(queue_dictionary, (void (*)(void *)) queue_destroy_and_destroy_elements);
}

void free_queue_message_manager(){
    free_queue_dictionary();
}