#include <common_structures.h>
#include <stdlib.h>
#include <general_logs.h>
#include <garbage_collector.h>
#include <signal.h>
#include <commons/string.h>
#include <unistd.h>
#include "../../Broker/include/broker_memory_manager.h"
#include <commons/temporal.h>

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

uint64_t current_time_in_milliseconds(){

   char* time = temporal_get_string_time();
   char** time_splitted = string_split(time, ":");
   char* hours = time_splitted[0];
   char* minutes = time_splitted[1];
   char* seconds = time_splitted[2];
   int milliseconds = atoi(time_splitted[3]);

   int hours_in_milliseconds = atoi(hours) * 3600000;
   int minutes_in_milliseconds = atoi(minutes) * 60000;
   int seconds_in_milliseconds = atoi(seconds) * 1000;

   free(time);
   free_char_array(time_splitted);

   return hours_in_milliseconds + minutes_in_milliseconds + seconds_in_milliseconds + milliseconds;
}

void assert_only_one_in(t_list* self){
    if(list_size(self) != 1){
        log_expected_to_have_only_one_element_error();
        free_system();
    }
}

int minimum_integer_between(int number, int another_number){
    return number < another_number ? number : another_number;
}