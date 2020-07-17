#include "get_query_performer.h"
#include "gamecard_query_performers.h"
#include "gamecard_configuration_manager.h"
#include "file_system.h"
#include "file_system_utils.h"
#include "../../Utils/include/common_structures.h"
#include "../../Utils/include/paths.h"
#include <stdlib.h>
#include <stdio.h>
#include <commons/string.h>
#include <unistd.h>
#include "../../Utils/include/garbage_collector.h"
#include "open_files_structure.h"


t_gamecard_query_performer *get_pokemon_query_performer;

t_gamecard_query_performer* get_query_performer(){
    return get_pokemon_query_performer;
}

t_request* get_localized_request(char* pokemon_name, uint32_t quantity, t_list* positions){

    t_localized_pokemon* localized_structure = safe_malloc(sizeof(t_localized_pokemon));
    localized_structure -> pokemon_name = pokemon_name;
    localized_structure -> quantity = quantity;
    localized_structure -> positions = positions;

    t_request* localized_request = safe_malloc(sizeof(t_request));
    localized_request -> operation = LOCALIZED_POKEMON;
    localized_request -> structure = localized_structure;
    localized_request -> sanitizer_function = (void (*)(void *)) free_localized_pokemon;

    return localized_request;
}

t_identified_message* get_query_performer_function(t_identified_message* identified_message){
    printf("Se recibio el mensaje GET_POKEMON con id = %d\n", identified_message -> message_id);

    //Armo el path del metadata para el Pokemon recibido
    t_get_pokemon* get_pokemon = identified_message->request->structure;
    char* pokemon_name = get_pokemon -> pokemon_name;
    char* pokemon_metadata_path = string_from_format("%s/Files/%s/Metadata.bin", tallgrass_mount_point(), pokemon_name);

    t_request* localized_request;
    if(exists_file_at(pokemon_metadata_path)) {

        //Leo el archivo de metadata
        t_file_metadata* metadata_file_information = safe_malloc(sizeof(t_file_metadata));
        metadata_file_information = read_file_of_type(FILE_METADATA, pokemon_metadata_path);

        //Leo bloques del archivo
        t_list* blocks_information = read_file_of_type(BLOCK, metadata_file_information -> blocks);

        free(metadata_file_information);

        //Crear listado de posiciones
        t_list* positions_list = create_positions_list(blocks_information);
        uint32_t positions_amount = amount_of_positions(positions_list);

        //Esperar cantidad de segundos definidos por archivo de configuracion
        sleep(operation_delay_time_getter());

        //Cerrar archivo metadata
        close_metadata(pokemon_metadata_path);
        remove_from_open_files(pokemon_metadata_path);
        stop_considering_garbage(blocks_information);

        localized_request = get_localized_request(pokemon_name, positions_amount, positions_list);

    } else {

        //Devolver mensaje sin ninguna posicion
        localized_request = get_localized_request(pokemon_name, 0, list_create());
    }

    free(pokemon_metadata_path);

    //Armado de la estructura de mensaje
    t_identified_message* localized_message = safe_malloc(sizeof(t_identified_message));
    localized_message -> message_id = identified_message -> message_id;
    localized_message -> request = localized_request;

    return localized_message;
}

bool get_query_performer_can_handle(uint32_t operation){
    return operation == GET_POKEMON;
}

void initialize_get_query_performer(){
    get_pokemon_query_performer = malloc(sizeof(t_gamecard_query_performer));
    get_pokemon_query_performer->can_be_handled_function = get_query_performer_can_handle;
    get_pokemon_query_performer->perform_function = get_query_performer_function;
}
