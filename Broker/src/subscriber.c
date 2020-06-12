#include <subscriber.h>
#include <pthread.h>
#include <broker_logs_manager.h>
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/socket.h"
#include "../../Utils/include/t_list_extension.h"
#include "../../Utils/include/pthread_wrapper.h"
#include <commons/string.h>
#include <free_broker.h>

void configure_ack_timeout_for(int socket_fd){

    int timeout_in_seconds = config_get_int_at("ACK_TIMEOUT");
    configure_socket_timeout_in_seconds(socket_fd, timeout_in_seconds);
}

void subscribe_client_to_queue(t_subscriber_context* subscriber_context){
    t_queue_context* queue_context = queue_context_with_code(subscriber_context -> operation_queue);

    configure_ack_timeout_for(subscriber_context -> socket_fd);

    /*TODO
     * 0 - Llegamos acá porque claramente alguien se suscribió.
     * 1 - Busquemos si ya estaba subscripto, esto quiere decir que para dicha cola, existe (list_remove_by_condition)
     * un subscriber_context con process_description = al del subscriber_context que vino por parámetro.
     * 2 - Si no estaba subscripto, (remove_by_contidion == NULL) agregamos a la cola con el add_subscriber_function
     * 3 - Si estaba subscripto, es decir, remove_by_contidion != NULL, entonces, al que vino por parámetro le
     * actualizamos el last_message_id_received con el que removimos. Finalmente, agregamos el nuevo subscriber_context
     */

    queue_context -> queue_context_operations -> add_subscriber_function (queue_context, (void*) subscriber_context);
    log_succesful_subscription_process(subscriber_context);
}

bool are_equals_subscribers(t_subscriber_context* subscriber_context, t_subscriber_context* another_subscriber_context){
    return
        subscriber_context -> operation_queue == another_subscriber_context -> operation_queue &&
        subscriber_context -> socket_fd == another_subscriber_context -> socket_fd &&
        subscriber_context -> last_message_id_received == another_subscriber_context -> last_message_id_received &&
        string_equals_ignore_case(subscriber_context -> process_description, another_subscriber_context -> process_description);
}

bool is_still_subscribed(t_queue_context* queue_context, t_subscriber_context* subscriber_context){

    bool _is_subscribed(t_subscriber_context* subscriber_to_find, t_subscriber_context* subscriber_to_compare){
       return are_equals_subscribers(subscriber_to_find, subscriber_to_compare);
    }

    return list_contains(queue_context -> subscribers, subscriber_context, (bool (*)(void *, void*)) _is_subscribed);
}

void send_all_messages(t_subscriber_context* subscriber_context) {
    t_queue_context* queue_context = queue_context_with_code(subscriber_context -> operation_queue);
    t_list* queue_messages = queue_context -> queue -> elements;

    /*TODO
     * Independientemente de si es nuevo o no, ya tiene el last_message_id_received actualizado
     * Filtramos de la cola de mensajes aquellos que no le fueron enviados, es decir, los mensajes
     * que tengan id_mensaje > a last_message_id_received.
     * Analizar la necesidad de tener un subscribers_to_send, no me alcanza con el message_id y
     * el subscribers_who_received? Tal vez hace falta entender que información se necesita.
     * Si hacen falta 2 estados (recibido, no recibido) o más (SIN ENVIAR, ENVIADO, NO RECIBIDO, RECIBIDO)
     */

    for(int i = 0; i < list_size(queue_messages) && is_still_subscribed(queue_context, subscriber_context); i++) {

        t_message_status* message_status = list_get(queue_messages, i);

            if(message_status -> identified_message -> message_id <= subscriber_context -> last_message_id_received){
                continue;
            }

        t_request* request = create_request_from(message_status);
        serialize_and_send_structure(request, subscriber_context -> socket_fd);

        pthread_t waiting_for_ack_thread = default_safe_thread_create(receive_ack_thread, (void*) &subscriber_context -> socket_fd);

        join_reception_for_ack_thread(waiting_for_ack_thread, subscriber_context, message_status, queue_context);
    }
}