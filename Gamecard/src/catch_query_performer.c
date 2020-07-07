#include "catch_query_performer.h"
#include "file_system.h"
#include "gamecard_query_performers.h"
#include "gamecard_configuration_manager.h"
#include "../../Utils/include/common_structures.h"
#include "../../Utils/include/paths.h"
#include <stdlib.h>
#include <stdio.h>
#include <commons/string.h>

t_gamecard_query_performer *catch_pokemon_query_performer;

t_gamecard_query_performer* catch_query_performer(){
    return catch_pokemon_query_performer;
}

t_identified_message* catch_query_performer_function(t_identified_message* identified_message){
    printf("Se recibio el mensaje CATCH_POKEMON con id = %d\n", identified_message -> message_id);

    //Armo el path del metadata para el Pokemon recibido
    	t_catch_pokemon* catch_pokemon = identified_message->request->structure;
    	char* pokemon_name = catch_pokemon -> pokemon_name;
    	char* pokemon_metadata_path = string_from_format("%s/Files/%s/Metadata.bin", tallgrass_mount_point(), pokemon_name);

    	t_request* localized_request;

    	if(exists_file_at(pokemon_metadata_path)) {
    		//Leo el archivo de metadata
    		t_file_metadata* metadata_file_information = safe_malloc(sizeof(t_file_metadata));
    		metadata_file_information = read_file_of_type(FILE_METADATA, pokemon_metadata_path);

    		//Leo bloques del archivo
    		t_list* blocks_information = read_file_of_type(BLOCK, metadata_file_information -> blocks);

    		subtract_or_remove_from(blocks_information, catch_pokemon);

    		//tengo que usar contenido file metadata para tomar el primer bloque y compactar
    		write_pokemon_data(blocks_information, metadata_file_information -> blocks);

//TODO:    		rewrite_pokemon_metadata(metadata_file_information,pokemon_metadata_path);
    	}
    	else{
    		//fallo, no existe el que queres catchear
    	}
}

bool catch_query_performer_can_handle(uint32_t operation){
    return operation == CATCH_POKEMON;
}

void initialize_catch_query_performer(){
    catch_pokemon_query_performer = malloc(sizeof(t_gamecard_query_performer));
    catch_pokemon_query_performer->can_be_handled_function = catch_query_performer_can_handle;
    catch_pokemon_query_performer->perform_function = catch_query_performer_function;
}

