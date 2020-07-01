#include "new_query_performer.h"
#include "gamecard_query_performers.h"
#include "../../Utils/include/common_structures.h"
#include <stdlib.h>
#include <stdio.h>

t_gamecard_query_performer *new_pokemon_query_performer;

t_gamecard_query_performer* new_query_performer(){
    return new_pokemon_query_performer;
}

t_identified_message* new_query_performer_function(t_identified_message* identified_message){
    printf("Se recibio el mensaje NEW_POKEMON con id = %d\n", identified_message -> message_id);

    //Armo el path del metadata para el Pokemon recibido
	t_new_pokemon* new_pokemon = identified_message->request->structure;
	char* pokemon_name = new_pokemon -> pokemon_name;
	char* pokemon_metadata_path = string_from_format("%s/Files/%s/Metadata.bin", tallgrass_mount_point(), pokemon_name);

	t_request* localized_request;

	if(exists_file_at(pokemon_metadata_path)) {
		//Leo el archivo de metadata
		t_file_metadata* metadata_file_information = safe_malloc(sizeof(t_file_metadata));
		metadata_file_information = read_file_of_type(FILE_METADATA, pokemon_metadata_path);

		//Leo bloques del archivo
		t_list* blocks_information = read_file_of_type(BLOCK, metadata_file_information -> blocks);

		t_list* new_blocks_information = add_or_modify_to(blocks_information, new_pokemon);

		//tengo que usar contenido file metadata para tomar el primer bloque y compactar
		write_pokemon_data(new_blocks_information, metadata_file_information -> blocks);

	}
	else{
		//tengo que crear el metadata
		//tengo que que pedir un bloque, escribir en ese bloque
		//crear el metadata de ese pokemon, en bloques tendra el bloque creado arriba
	}

}

bool new_query_performer_can_handle(uint32_t operation){
    return operation == NEW_POKEMON;
}

void initialize_new_query_performer(){
    new_pokemon_query_performer = malloc(sizeof(t_gamecard_query_performer));
    new_pokemon_query_performer->can_be_handled_function = new_query_performer_can_handle;
    new_pokemon_query_performer->perform_function = new_query_performer_function;
}
