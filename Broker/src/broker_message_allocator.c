#include <broker_logs_manager.h>
#include <commons/collections/dictionary.h>
#include <dynamic_partition_message_allocator.h>
#include <buddy_system_message_allocator.h>
#include "broker_message_allocator.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/serialization_interface.h"
#include "../../Utils/include/pokemon_request_bytes_calculator.h"
#include "../../Utils/include/configuration_manager.h"

t_dictionary* allocation_algorithms;
t_message_allocator* message_allocator;

void initialize_allocation_algorithms(){
    allocation_algorithms = dictionary_create();
    dictionary_put(allocation_algorithms,"PD", (void*) initialize_dynamic_partition_message_allocator);
    dictionary_put(allocation_algorithms, "BS", (void*) initialize_buddy_system_message_allocator);
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

t_memory_block* build_memory_block_from_message(t_identified_message* message) {
    //Obtengo el mensaje
    t_request* message_request = message_request_from_identified_message(message);

    t_memory_block *memory_block_to_save = safe_malloc(sizeof(t_memory_block));

    memory_block_to_save->message_id =  message->message_id;
    memory_block_to_save->message_operation = message_request->operation;

    t_serialization_information *request_serialized = serialize(message_request);

    memory_block_to_save->message_size = size_to_allocate_for(message_request);
    memory_block_to_save->message = ((t_request *) request_serialized->serialized_request)->structure;
    memory_block_to_save->lru_value = current_time_in_milliseconds();

    if(memory_block_to_save->message_size > message_allocator->max_partition_size){
        log_invalid_operation_to_save_message_error();
        free_system();
    }
    return memory_block_to_save;
}

t_message_allocator* initialize_message_allocator() {

    char* memory_algorithm = config_get_string_at("ALGORITMO_MEMORIA");
    void* initialize_message_allocator_from_dictionary = dictionary_get(allocation_algorithms, memory_algorithm);
    t_message_allocator *(*initialize_message_allocator_function)() = (t_message_allocator *(*)()) initialize_message_allocator_from_dictionary;
    message_allocator = initialize_message_allocator_function();

    return message_allocator;
}