#include "../include/queue_message_manager.h"
#include "commons/collections/queue.h"
#include "../include/broker_deserialization.h"
#include "../../Utils/include/socket.h"
#include "../include/broker_logs_manager.h"
#include <stdlib.h>

//son queues con estructuras del tipo: connection_request.

t_queue* appeared_queue;
t_queue* new_queue;
t_queue* catch_queue;
t_queue* caught_queue;
t_queue* get_queue;
t_queue* localized_queue;

t_list* subscribers;

void initialize_messages_queue(){
    appeared_queue = queue_create();
    new_queue = queue_create();
    catch_queue = queue_create();
    caught_queue = queue_create();
    get_queue = queue_create();
    localized_queue = queue_create();
}

void initialize_queue_message_manager(){
    initialize_messages_queue();
    log_succesful_queues_creation();
    subscribers = list_create();
    log_succesful_subscribers_list_creation()
}

void subscribe_process(t_connection_request* connection_request){

    void* deserialized_suscribed_queue = deserialize(connection_request->serialized_request); //me devuelve la cola a la cual esta suscripto.
    uint32_t suscribed_queue = *((uint32_t*) deserialized_suscribed_queue);

    t_subscriber* subscriber = malloc(sizeof(t_subscriber));
    subscriber -> queue = suscribed_queue;
    subscriber -> socket_fd = connection_request -> socket_fd;

    list_add(subscribers, (void*) subscriber);
}

void push_to_queue(uint32_t operation, t_connection_request* connection_request) {

    switch (operation) {

        case APPEARED_POKEMON :
            queue_push(appeared_queue, connection_request -> serialized_request);
            log_succesful_new_message_pushed_to_a_queue(); //log main
            log_pushed_message(); //log nuestro
            break;

        case NEW_POKEMON :
            queue_push(new_queue, connection_request -> serialized_request); //accedemos a esto para que en un futuro
            // directamente en la cola enviamos la serialized request que es un void*.
            log_succesful_new_message_pushed_to_a_queue();
            log_pushed_message();
            break;

        case CATCH_POKEMON :
            queue_push(catch_queue, connection_request -> serialized_request);
            log_succesful_new_message_pushed_to_a_queue();
            log_pushed_message();
            break;

        case CAUGHT_POKEMON :
            queue_push(caught_queue, connection_request -> serialized_request);
            log_succesful_new_message_pushed_to_a_queue();
            log_pushed_message();
            break;

        case GET_POKEMON :
            queue_push(get_queue, connection_request -> serialized_request);
            log_succesful_new_message_pushed_to_a_queue();
            log_pushed_message();
            break;

        case LOCALIZED_POKEMON :
            queue_push(localized_queue, connection_request -> serialized_request);
            log_succesful_new_message_pushed_to_a_queue();
            log_pushed_message();
            break;

        case SUBSCRIBE_ME :
            subscribe_process(connection_request);
            log_succesful_subscription_process(); //log main
            log_succesful_subscription(); //log nuestro
            break;

        default: ;
        //    received_unknown_operation_error();

    }

}

void publish(uint32_t queue, t_serialization_information* serialization_information){

    bool _is_subscribed(void* subscriber){
        return ((t_subscriber*) subscriber) -> queue == queue;
    }

    t_list* subscribers_of_a_queue = list_filter(subscribers, _is_subscribed);

    void _send_message(void* subscriber){
        send_structure(serialization_information, ((t_subscriber*) subscriber) -> socket_fd);
    }
    list_iterate(subscribers_of_a_queue, _send_message);

    list_destroy(subscribers_of_a_queue);
}

void free_subscriber(t_subscriber* subscriber){
    free(&(subscriber -> queue));
    free(subscriber);
}