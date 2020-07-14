#include <broker_logs_manager.h>
#include <commons/collections/dictionary.h>
#include <dynamic_partition_message_allocator.h>
#include <buddy_system_message_allocator.h>
#include <stdlib.h>
#include "broker_message_allocator.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/pokemon_request_bytes_calculator.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/serializable_objects.h"
#include <string.h>

t_dictionary* allocation_algorithms;
t_message_allocator* message_allocator;
uint64_t fifo_id;
pthread_mutex_t memory_mutex;

pthread_mutex_t get_memory_mutex(){
    return memory_mutex;
}

void initialize_allocation_algorithms(){
    allocation_algorithms = dictionary_create();
    dictionary_put(allocation_algorithms,"PD", (void*) initialize_dynamic_partition_message_allocator);
    dictionary_put(allocation_algorithms, "BS", (void*) initialize_buddy_system_message_allocator);
}

//consolida los bloques en memoria, no en la lista administrativa (asume que conseguiste el bloque con un list_removes)
void consolidate_block_with(t_block_information* master_block,t_block_information* block_to_be_consolidated){
    master_block->block_size += block_to_be_consolidated ->block_size;

    if(master_block->initial_position > block_to_be_consolidated ->initial_position){

        master_block->initial_position = block_to_be_consolidated ->initial_position;
    }

    free(block_to_be_consolidated);
}

uint32_t block_size_for(t_memory_block *  memory_block_to_save){
    bool block_to_save_bigger_than_min_partition_size = memory_block_to_save->message_size > message_allocator->min_partition_size;

    return block_to_save_bigger_than_min_partition_size ? memory_block_to_save->message_size : message_allocator->min_partition_size;
}

t_request* message_request_from_identified_message(t_identified_message* message){
    uint32_t operation = internal_operation_in(message);
    return operation == IDENTIFIED_MESSAGE ?  (t_request *) internal_request_in_correlative(message) : message->request;
}

int block_index_position(t_block_information* block_to_find,t_list* blocks_information){

    for(int i= 0; i < list_size(blocks_information); i++){
        t_block_information* block_found = (t_block_information*) list_get(blocks_information,i);
        if(block_found->initial_position == block_to_find->initial_position){
            return i;
        }
    }
    free_system();
    return -1;
}

uint32_t get_correlative_message_id_from(t_identified_message* message){
    if(internal_operation_in(message) == IDENTIFIED_MESSAGE){
       t_identified_message* correlative_identified_message = internal_object_in(message);
       return correlative_identified_message -> message_id;
    } else {
        return 0;
    }
}

uint32_t get_size_of(t_identified_message* message){

    t_request* message_request = message_request_from_identified_message(message);

    return size_to_allocate_for(message_request);
}

t_memory_block* build_memory_block_from(t_identified_message* message, t_block_information* block_information) {
    //Obtengo el mensaje
    t_request* message_request = message_request_from_identified_message(message);

    t_serializable_object* serializable_object = serializable_object_with_code(message -> request -> operation);
    t_memory_block *memory_block_to_save = safe_malloc(sizeof(t_memory_block));

    uint32_t correlative_message_id = get_correlative_message_id_from(message);
    memory_block_to_save->message_id =  message->message_id;
    memory_block_to_save->correlative_message_id = correlative_message_id;
    memory_block_to_save->message_operation = message_request->operation;

    memory_block_to_save->message_size = size_to_allocate_for(message_request);
    t_serialization_information* serialization_information = serializable_object -> serialize_function (message -> request -> structure);

    memory_block_to_save -> message = block_information ->initial_position;
    //No pasar serialize request directo, hay que sacar operation, y creo que el espacio del structure tambien!!
    //me parece que en el caso de un identified hay que poner solo un sizeof.

    if(memory_block_to_save ->correlative_message_id != 0){
        //tamaño de estructura (identified_message)
        //id mensaje
        //tamaño de la estructura(identified_message)
        //id mensaje
        //tamaño de la estructura
        //estos son los 6 uint32_t que hay que moverse!
        memcpy(memory_block_to_save -> message,serialization_information -> serialized_request + sizeof(uint32_t) * 6, memory_block_to_save -> message_size);
    } else {
        memcpy(memory_block_to_save -> message,serialization_information -> serialized_request + sizeof(uint32_t) + sizeof(uint32_t), memory_block_to_save -> message_size);
    }

    free_serialization_information(serialization_information);

    memory_block_to_save->lru_value = current_time_in_milliseconds();
    memory_block_to_save->memory_block_id = get_next_fifo_id();

    if(memory_block_to_save->message_size > message_allocator->max_partition_size){
        log_invalid_operation_to_save_message_error();
        free_system();
    }

    return memory_block_to_save;
}

uint64_t get_next_fifo_id(){
    //se podria chequear ademas de que no supere la cantidad maxima de un uint64
    fifo_id++;
    return fifo_id;
}

t_message_allocator* initialize_message_allocator() {

    char* memory_algorithm = config_get_string_at("ALGORITMO_MEMORIA");
    void* initialize_message_allocator_from_dictionary = dictionary_get(allocation_algorithms, memory_algorithm);
    t_message_allocator *(*initialize_message_allocator_function)() = (t_message_allocator *(*)()) initialize_message_allocator_from_dictionary;
    message_allocator = initialize_message_allocator_function();
    fifo_id = 0;
    return message_allocator;
}

void free_message_allocator(){
    dictionary_destroy(allocation_algorithms);
    free(message_allocator);
}