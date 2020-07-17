#include "catch_query_performer.h"
#include "file_system.h"
#include "file_system_utils.h"
#include "gamecard_query_performers.h"
#include "gamecard_configuration_manager.h"
#include "../../Utils/include/common_structures.h"
#include "../../Utils/include/paths.h"
#include <stdlib.h>
#include <stdio.h>
#include <commons/string.h>
#include <unistd.h>
#include "../../Utils/include/garbage_collector.h"
#include "open_files_structure.h"

t_gamecard_query_performer *catch_pokemon_query_performer;

t_gamecard_query_performer* catch_query_performer(){
    return catch_pokemon_query_performer;
}

t_request* catch_caught_request(uint32_t caught_status){

    t_caught_pokemon* caught_structure = safe_malloc(sizeof(t_caught_pokemon));
    caught_structure -> caught_status = caught_status;

    t_request* caught_request = safe_malloc(sizeof(t_request));
    caught_request -> operation = CAUGHT_POKEMON;
    caught_request -> structure = caught_structure;
    caught_request -> sanitizer_function = (void (*)(void *)) free_caught_pokemon;

    return caught_request;
}

t_identified_message* catch_query_performer_function(t_identified_message* identified_message){
    printf("Se recibio el mensaje CATCH_POKEMON con id = %d\n", identified_message -> message_id);

    //Armo el path del metadata para el Pokemon recibido
    	t_catch_pokemon* catch_pokemon = identified_message->request->structure;
    	char* pokemon_name = catch_pokemon -> pokemon_name;
    	char* pokemon_metadata_path = string_from_format("%s/Files/%s/Metadata.bin", tallgrass_mount_point(), pokemon_name);

    	t_request* caught_request;
    	uint32_t caught_status;


    	if(exists_file_at(pokemon_metadata_path)) {
    		//Leo el archivo de metadata
    		t_file_metadata* metadata_file_information = safe_malloc(sizeof(t_file_metadata));
    		metadata_file_information = read_file_of_type(FILE_METADATA, pokemon_metadata_path);

    		//Leo bloques del archivo
    		t_list* blocks_information = read_file_of_type(BLOCK, metadata_file_information -> blocks);

    		bool line_exists = subtract_or_remove_from(blocks_information, catch_pokemon);
    		if(line_exists){
    			//tengo que usar contenido file metadata para tomar el primer bloque y compactar
				write_pokemon_blocks(blocks_information, metadata_file_information);

				//Esperar cantidad de segundos definidos por archivo de configuracion
				sleep(operation_delay_time_getter());

				write_pokemon_metadata(metadata_file_information,pokemon_metadata_path);

				list_destroy_and_free_elements(blocks_information);
				stop_considering_garbage(blocks_information);

				caught_status = 1;
    		}
    		else{//fallo, existe el pokemon pero no en la posicion pedida
    			close_metadata(pokemon_metadata_path);

			    caught_status = 0;
    		}
    		//remove_from_open_files(pokemon_metadata_path);
            free_metadata_file(metadata_file_information);
    	}
    	else{
    		//fallo, no existe el archivo pokemon
			caught_status = 0;
    	}

		free(pokemon_metadata_path);
    	caught_request = catch_caught_request(caught_status);

		//Armado de la estructura de mensaje
		t_identified_message* caught_message = safe_malloc(sizeof(t_identified_message));
		caught_message -> message_id = identified_message -> message_id;
		caught_message -> request = caught_request;

		return caught_message;
}

bool catch_query_performer_can_handle(uint32_t operation){
    return operation == CATCH_POKEMON;
}

void initialize_catch_query_performer(){
    catch_pokemon_query_performer = malloc(sizeof(t_gamecard_query_performer));
    catch_pokemon_query_performer->can_be_handled_function = catch_query_performer_can_handle;
    catch_pokemon_query_performer->perform_function = catch_query_performer_function;
}

