#include <common_structures.h>
#include <stdlib.h>
#include <general_logs.h>
#include <garbage_collector.h>
#include <signal.h>
#include <commons/string.h>
#include <unistd.h>
#include "../../Broker/include/broker_memory_manager.h"

void handler(){
    char* goodbye_message = "\nOK. Me vas a matar. :( Pero antes voy a liberar tooodaaa la memoria que utilicé! :D\n\n";
    write(STDOUT_FILENO, goodbye_message, string_length(goodbye_message));
    free_system();
}

void sigpipe_handler(){
    log_syscall_error("Se produjo una señal SIGPIPE");
}

void handle_signal(int signal_number, void (*handler_function) ()){

    struct sigaction signal_action = {.sa_handler = handler_function};

    if(sigaction(signal_number, &signal_action, NULL) == -1){
        log_syscall_error("Error en la creación de una acción de signals");
        free_system();
    }
}

void initialize_signal_handler(){
    handle_signal(SIGINT, handler);
    handle_signal(SIGTERM, handler);
    handle_signal(SIGPIPE, sigpipe_handler);
}

void* safe_malloc(size_t size){
    void* pointer = malloc(size);
    if(pointer == NULL && size != 0){
        log_syscall_error("Error al guardar espacio de memoria con safe_malloc");
        free_system();
    }
    return pointer;
}

t_identified_message* create_identified_message(uint32_t message_id, t_request* request){
    t_identified_message* identified_message = safe_malloc(sizeof(t_identified_message));
    identified_message -> message_id = message_id;
    identified_message -> request = request;

    return identified_message;
}

t_connection_request* create_connection_request(int connection_fd, t_request* request){
    t_connection_request* connection_request = safe_malloc(sizeof(t_connection_request));
    connection_request -> socket_fd = connection_fd;
    connection_request -> request = request;

    return connection_request;
}

unsigned long long hash(char* value){
    unsigned long long hash = 0;
    int value_length = string_length(value);

    for(int index = 0; index < value_length; index++) {
        unsigned char c = value[index];
        hash += c;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}

char* process_description_for(char* process_name, t_list* strings_to_hash){
    unsigned long long hash_sum = 0;

    for(int i = 0; i < list_size(strings_to_hash); i++){
        char* string = list_get(strings_to_hash, i);
        hash_sum += hash(string);
    }

    return string_from_format("%s-%llu", process_name, hash_sum);
}

void free_request(t_request* self){
    self -> sanitizer_function (self -> structure);
    free(self);
}

void free_identified_message(t_identified_message* identified_message){
    t_request* internal_request = identified_message -> request;
    free_request(internal_request);
    free(identified_message);
}

void free_char_array(char** char_array){
    //itero hasta que lo que apunta char array sea null, en cuyo caso null evalua como false
    for (int i = 0; char_array[i]; i++ ){
        free( char_array[i] );
    }
    free( char_array );
}

void free_serialization_information(t_serialization_information* serialization_information){
    free(serialization_information -> serialized_request);
    free(serialization_information);
}
void free_localized_pokemon(t_localized_pokemon* localized_pokemon){
    list_destroy_and_destroy_elements(localized_pokemon->positions,free);
    free(localized_pokemon);
}

void free_subscribe_me(t_subscribe_me* subscribe_me){
    free(subscribe_me -> process_description);
    free(subscribe_me);
}

uint32_t internal_operation_in(t_identified_message* identified_message){
    return identified_message -> request -> operation;
}

uint32_t internal_operation_in_correlative(t_identified_message* correlative_identified_message){
    t_identified_message* original_identified_message = (t_identified_message*) internal_object_in(correlative_identified_message);
    return internal_operation_in(original_identified_message);
}

void* internal_object_in(t_identified_message* identified_message){
    return identified_message -> request -> structure;
}

void* internal_request_in_correlative(t_identified_message* correlative_identified_message) {
    t_identified_message *original_identified_message = internal_object_in(correlative_identified_message);
    return original_identified_message -> request;
}

void* internal_object_in_correlative(t_identified_message* correlative_identified_message){
    t_identified_message* original_identified_message = internal_object_in(correlative_identified_message);
    return internal_object_in(original_identified_message);
}

void sem_initialize(sem_t* semaphore){
    sem_init(semaphore, false, 0);
}