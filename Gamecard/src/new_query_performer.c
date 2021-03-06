#include "new_query_performer.h"
#include "file_system.h"
#include "file_system_utils.h"
#include "gamecard_query_performers.h"
#include "gamecard_configuration_manager.h"
#include "../../Utils/include/common_structures.h"
#include "../../Utils/include/paths.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <commons/string.h>
#include <unistd.h>
#include <gamecard_logs_manager.h>
#include "../../Utils/include/garbage_collector.h"
#include "open_files_structure.h"

void create_metadata_with_open_flag(t_file_metadata* metadata_with_open_flag){

	metadata_with_open_flag -> directory = "N";
	metadata_with_open_flag -> size = 0;
	metadata_with_open_flag -> blocks = "[-1]";
	metadata_with_open_flag -> open = "Y";
}


t_gamecard_query_performer *new_pokemon_query_performer;

t_gamecard_query_performer* new_query_performer(){
    return new_pokemon_query_performer;
}

t_request* new_appeared_request(t_new_pokemon* new_pokemon){

    t_appeared_pokemon* appeared_structure = safe_malloc(sizeof(t_appeared_pokemon));
    appeared_structure -> pokemon_name = new_pokemon -> pokemon_name;
    appeared_structure -> pos_x = new_pokemon -> pos_x;
    appeared_structure -> pos_y = new_pokemon -> pos_y;

    t_request* localized_request = safe_malloc(sizeof(t_request));
    localized_request -> operation = APPEARED_POKEMON;
    localized_request -> structure = appeared_structure;
    localized_request -> sanitizer_function = free;

    return localized_request;
}

void initialize_metadata_file_information(t_file_metadata* metadata_file_information){
    char* new_block_number = get_new_block();
    metadata_file_information -> blocks = string_from_format("[%s]", new_block_number);
    metadata_file_information -> directory = string_duplicate("N");
    metadata_file_information -> open = string_duplicate("N");
    free(new_block_number);
}

t_identified_message* new_query_performer_function(t_identified_message* identified_message){

    //Armo el path del metadata para el Pokemon recibido
	t_new_pokemon* new_pokemon = identified_message->request->structure;
	char* pokemon_metadata_path = string_from_format("%s/Files/%s/Metadata.bin", tallgrass_mount_point(), new_pokemon -> pokemon_name);

    t_file_metadata* metadata_file_information;

	t_request* appeared_request;

	if(exists_file_at(pokemon_metadata_path)) {
		//Leo el archivo de metadata
		metadata_file_information = read_file_of_type(FILE_METADATA, pokemon_metadata_path);

		//Leo bloques del archivo
		t_list* blocks_information = read_file_of_type(BLOCK, metadata_file_information -> blocks);

        update_pokemon_line_to(blocks_information, new_pokemon);

		//tengo que usar contenido file metadata para tomar el primer bloque y compactar
		write_pokemon_blocks(blocks_information, metadata_file_information);

        list_destroy_and_destroy_elements(blocks_information, free);
	}
	else{//el archivo no existe, hay que crearlo
		create_pokemon_metadata(new_pokemon -> pokemon_name);

		t_file_metadata* metadata_with_open_flag = safe_malloc(sizeof(t_file_metadata));
		create_metadata_with_open_flag(metadata_with_open_flag);

		write_pokemon_metadata(metadata_with_open_flag,pokemon_metadata_path);
		free(metadata_with_open_flag);

        metadata_file_information = safe_malloc(sizeof(t_file_metadata));
		initialize_metadata_file_information(metadata_file_information);

        t_list* line_to_write = data_to_write(new_pokemon);//creo una lista con la linea que quiero escribir en blocks
		write_pokemon_blocks(line_to_write, metadata_file_information);
		list_destroy_and_destroy_elements(line_to_write, free);
		}

    //Esperar cantidad de segundos definidos por archivo de configuracion
    sleep(operation_delay_time_getter());
    write_pokemon_metadata(metadata_file_information,pokemon_metadata_path);

    remove_from_open_files(pokemon_metadata_path);
    free_metadata_file(metadata_file_information);
	free(pokemon_metadata_path);

	appeared_request = new_appeared_request(new_pokemon);

	//Armado de la estructura de mensaje

	t_identified_message* appeared_message = safe_malloc(sizeof(t_identified_message));
	appeared_message -> message_id = identified_message -> message_id;
	appeared_message -> request = appeared_request;

	return appeared_message;
}

bool new_query_performer_can_handle(uint32_t operation){
    return operation == NEW_POKEMON;
}

void initialize_new_query_performer(){
    new_pokemon_query_performer = malloc(sizeof(t_gamecard_query_performer));
    new_pokemon_query_performer->can_be_handled_function = new_query_performer_can_handle;
    new_pokemon_query_performer->perform_function = new_query_performer_function;
}