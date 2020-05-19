#include "../include/queue_message_manager.h"
#include "commons/collections/queue.h"
#include "../include/broker_deserialization.h"
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

void initialize_message_manager(){
    initialize_messages_queue();
    list_create(subscribers);
}

void subscribe_process(t_connection_request* connection_request){

    uint32_t* suscribed_queue = deserialize(connection_request->serialized_request);

    t_subscriber* subscriber = malloc(sizeof(t_subscriber));
    subscriber -> queue = *suscribed_queue;
    subscriber -> socket_fd = connection_request -> socket_fd;

    list_add(subscribers, subscriber);
}


void move_to_queue(uint32_t operation, t_connection_request* connection_request) {

    switch (operation) {

        case APPEARED_POKEMON :
            queue_push(appeared_queue, connection_request->serialized_request);
            break;

        case NEW_POKEMON :
            queue_push(new_queue, connection_request->serialized_request); //accedemos a esto para que en un futuro
            // directamente en la cola enviamos la serialized request que es un void*.
            break;

        case CATCH_POKEMON :
            queue_push(catch_queue, connection_request->serialized_request);
            break;

        case CAUGHT_POKEMON :
            queue_push(caught_queue, connection_request->serialized_request);
            break;

        case GET_POKEMON :
            queue_push(get_queue, connection_request->serialized_request);
            break;

        case LOCALIZED_POKEMON :
            queue_push(localized_queue, connection_request->serialized_request);
        case SUBSCRIBE_ME :
            subscribe_process(connection_request);
            break;

        default: ;
    }
}

t_list* subscribers_queue_list(){
    t_subscriber* subscriber;
    return list_map(subscribers, subscriber->queue);
}


bool equals_queues(t_subscriber* subscriber, uint32_t queue){
    return subscriber->queue == queue;
}

t_list* subscribers_to_a_queue(uint32_t operation){
    t_list* subscribers_queue = subscribers_queue_list();
    list_filter(subscribers_queue, equals_queues); //esto no se como era.
}
