#include "get_query_performer.h"
#include "gamecard_query_performers.h"
#include "gamecard_configuration_manager.h"
#include "filesystem.h"
#include "../../Utils/include/common_structures.h"
#include <stdlib.h>
#include <stdio.h>
#include <commons/string.h>

t_gamecard_query_performer *get_pokemon_query_performer;

t_gamecard_query_performer* get_query_performer(){
    return get_pokemon_query_performer;
}

void get_query_performer_function(t_identified_message* identified_message){
    printf("Se recibio el mensaje GET_POKEMON con id = %d\n", identified_message -> message_id);

    //Armo el path del metadata para el Pokemon recibido
    t_get_pokemon* get_pokemon = (t_get_pokemon*) identified_message->request->structure;
    char* pokemon_name = get_pokemon -> pokemon_name;
    char* pokemon_metadata_file = string_from_format("%s/Files/%s/Metadata.bin", tallgrass_mount_point(), pokemon_name);

    //Leo el archivo de metadata
    file_metadata* metadata_file_information = (file_metadata*) malloc(sizeof(file_metadata));
    metadata_file_information = read_file_of_type(FILE_METADATA, pokemon_metadata_file);//con un if cacheamos si no existe y enviamos mensaje "sin posiciones" y cortamos

    t_list* blocks_information = read_file_of_type(BLOCK, metadata_file_information -> blocks);

    void _print(pokemon_block_line* line){
        printf("PosX: %d, PosY: %d, Cantidad: %d", line -> positionX, line -> positionY, line -> pokemonQuantity);
    }

    list_iterate(blocks_information, (void (*)) _print);
}

bool get_query_performer_can_handle(uint32_t operation){
    return operation == GET_POKEMON;
}

void initialize_get_query_performer(){
    get_pokemon_query_performer = malloc(sizeof(t_gamecard_query_performer));
    get_pokemon_query_performer->can_be_handled_function = get_query_performer_can_handle;
    get_pokemon_query_performer->perform_function = get_query_performer_function;
}
