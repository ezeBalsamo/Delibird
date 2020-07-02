#include <commons/config.h>
#include <stdint.h>
#include <commons/bitarray.h>
#include <commons/string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include "file_system.h"
#include "file_system_files_information.h"
#include "file_system_utils.h"
#include "gamecard_logs_manager.h"
#include "gamecard_configuration_manager.h"
#include "../../Utils/include/common_structures.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/logger.h"

uint32_t maximum_length_of_line = 40;

bool can_read_line(char* buffer, uint32_t size, FILE* file_pointer)
{
    buffer[0] = '\0';
    buffer[size - 1] = '\0'; //mark end of buffer
    char* end_of_string;

    if (fgets(buffer, size, file_pointer) == NULL) {
        //Se leyó SOLO el EOF
        return false;
    }
    else {
        if ((end_of_string = strrchr(buffer, '\n')) != NULL) {
            *end_of_string = '\0'; //Modifico el \n del final de buffer por un \0 para que quede en formato string
        }
    }
    return true;
}

t_pokemon_block_line* create_pokemon_block_line(int32_t x, int32_t y, uint32_t quantity){

    t_pokemon_block_line* pokemon_line = safe_malloc(sizeof(t_pokemon_block_line));

    pokemon_line -> position_x = x;
    pokemon_line -> position_y = y;
    pokemon_line -> quantity = quantity;

    return pokemon_line;
}

//Se usa para el armado del mensaje LOCALIZED
t_list* create_positions_list(t_list* pokemon_block_lines){

    t_list* positions_list = list_create();

    void _create_position_element(t_pokemon_block_line* position) {
        int32_t* position_x = safe_malloc(sizeof(int32_t));
        int32_t* position_y = safe_malloc(sizeof(int32_t));

        *position_x = position -> position_x;
        *position_y = position -> position_y;

        list_add(positions_list, position_x);
        list_add(positions_list, position_y);
    }

    list_iterate(pokemon_block_lines, (void*) _create_position_element);

    return positions_list;
}

//Se usa para el armado del mensaje LOCALIZED
uint32_t amount_of_positions(t_list* positions_list){
    return list_size(positions_list) / 2;
}

t_file_system_metadata* read_file_system_metadata(char* file_path){

    FILE* file_pointer = fopen(file_path, "r+");

    //Crear el t_config a partir del archivo mount_point/Metadata/Metadata.bin
    t_config* metadata_config = config_create(file_path);
    t_file_system_metadata* file_system_metadata = read_file_system_metadata_from_config(metadata_config);

    consider_as_garbage(metadata_config, (void (*) (void*)) config_destroy);

    log_file_system_metadata_info(file_system_metadata);

    fclose(file_pointer);
    return file_system_metadata;
}

t_file_metadata* read_file_metadata(char* file_path){

    FILE* file_pointer = fopen(file_path, "r+");
    uint32_t file_descriptor = fileno(file_pointer); //el file descriptor para los flocks :D
    flock(file_descriptor, LOCK_SH);

    t_file_metadata* file_metadata;
    bool first_time_reading = true;
    t_config* metadata_config;

    do{
        if(!first_time_reading){
            config_destroy(metadata_config); //Destruir t_config creado ya que la lectura no me sirve porque el archivo está abierto.
            unlock_file_during_time(file_descriptor, operation_retry_time_getter());
        }


    //Crear el t_config a partir del archivo mount_point/Metadata/Metadata.bin
    metadata_config = config_create(file_path);
    file_metadata = read_file_metadata_from_config(metadata_config);

    log_file_metadata_info(file_metadata);

    first_time_reading = false;

    } while(is_open(file_metadata));

    consider_as_garbage(metadata_config, (void (*)(void *)) config_destroy);

    set_open(file_pointer); //Una vez que sali del loop tengo que escribir la Y en el open
    fclose(file_pointer); //La escritura del flag OPEN se realiza al cerrar el file_pointer

    consider_as_garbage(file_path, (void (*) (void*)) close_metadata); //En caso de que se corte la ejecución, nos aseguramos que el archivo metadata sea cerrado.

    flock(file_descriptor,LOCK_UN);

    return file_metadata;
}

t_list* read_block(char* file_path){

    FILE* file_pointer;

    char line_data_parameter[12];
    char line[maximum_length_of_line];
    char block_pointer[12];
    uint32_t blocks_quantity;
    char* final_path;
    t_list* blocks_data = list_create();

    uint32_t i = 1;
    do{
        blocks_quantity = split(file_path, i, "[,]", block_pointer);

        final_path = create_block_path(block_pointer);

        file_pointer = fopen(final_path, "r");

        while(can_read_line(line, maximum_length_of_line, file_pointer)){

            split(line, 1, "-=", line_data_parameter);
            uint32_t x = atoi(line_data_parameter);

            split(line, 2, "-=", line_data_parameter);
            uint32_t y = atoi(line_data_parameter);

            split(line, 3, "-=", line_data_parameter);
            uint32_t quantity = atoi(line_data_parameter);

            //Guardarlo en la lista, lo vi asi en las commons el agregado a la lista
            list_add(blocks_data, create_pokemon_block_line(x, y, quantity));

            log_block_metadata_info(x, y, quantity);
        }
        fclose(file_pointer);
        i++;
    }
    while(i <= blocks_quantity);

    return blocks_data;
}

void modify_specific_position(int add_susbtract_amount, line_block_information* element_to_modify, int* index, t_list *list_blocks_information){
	if((element_to_modify->PokemonQuantity + add_susbtract_amount) == 0){
		element_to_modify->PokemonQuantity += add_susbtract_amount;
		list_remove(list_blocks_information,*index);
	}
	else{
		element_to_modify->PokemonQuantity += add_susbtract_amount;
	}
}

line_block_information *find_line_with_position(int posX, int posY, t_list *lista, int* index){
	int i = 0;
	int* auxiliar_index;
	void _positions_match(line_block_information *position_in_list) {
		if(position_in_list->positionX == posX && position_in_list->positionY == posY){
			*auxiliar_index = i;
			return;
		}
		i++;
	}

	list_iterate(lista,(void*) _positions_match);
	index = auxiliar_index;
	if(auxiliar_index == NULL){
		return NULL;
	}
	else{
		return list_get(lista, *auxiliar_index);
	}
}

void substract_or_remove_from(t_list* blocks_information, t_catch_pokemon pokemon_to_susbtract){

	int *index = (int *)safe_malloc(sizeof(int));
	t_pokemon_block_line *coord = safe_malloc(sizeof(t_pokemon_block_line));

	coord = find_line_with_position(pokemon_to_susbtract -> pos_x, pokemon_to_susbtract -> pos_y, blocks_information, index);

	//si no lo encuentro coord == NULL
	if (coord == NULL){
		//deberia tirar un error
	}
	else{
		if((coord -> PokemonQuantity -1) == 0){
			list_remove(blocks_information,*index);
		}
		else{
			coord -> PokemonQuantity -= 1;
		}
	}

	free(coord);
	free(indice);
}

void add_or_modify_to(t_list* blocks_information, t_new_pokemon pokemon_to_add){

	t_pokemon_block_line *requested_line = find_line_with_position(pokemon_to_add -> pos_x, pokemon_to_add -> pos_y, blocks_information, NULL);

	//si no existe ninguna linea con esa posicion, lo agrego a la lista de lineas
	if (coord == NULL){
		list_add(blocks_information,pokemon_to_add);
	}
	else{//si ya existia agrego en esa linea la cantidad que se esta sumando
		coord -> quantity += pokemon_to_add -> quantity;
		//modify_specific_position(pokemon_to_add -> quantity, coord, NULL, blocks_information);
	}

	free(coord);
}

void remove_from_blocks(char* string_to_modify, char* block_to_remove){
	int aux = strlen(block_to_remove);
	aux += 2; //uno por el ']' y otro por la ','
	string_to_modify = string_substring(string_to_modify,0,strlen(string_to_modify)-aux);
	string_append(&string_to_modify, "]");
}

void add_to_blocks(char* string_to_modify, char* block_to_add){
	string_to_modify = string_substring(string_to_modify,0,strlen(string_to_modify)-1); //recorto el ']'
	string_append_with_format(&string_to_modify, ",%s]",block_to_add); //agrego el bloque nuevo y ]
}

bool write_until_full(char* block_path, t_list* pokemon_data_list){

	FILE *fp = fopen(block_path,"w");

	int size_already_written = 0;
	char* line_with_string_format;
	int number_of_lines_to_write = pokemon_data_list->elements_count;

	for(int i = 0; i < number_of_lines_to_write; i++){
		t_pokemon_block_line *line = safe_malloc(sizeof(t_pokemon_block_line));

		line = list_remove(pokemon_data_list,0);
		line_with_string_format = block_line_to_string(line);

		int line_length = strlen(line_with_string_format) + 1;

		if(size_already_written + line_length > file_system_metadata -> blocks_size){ //si la linea no me entra en el bloque, la mando de nuevo a la lista
			list_add(pokemon_data_list,line);
			fclose(fp);
			return false; //se acabo el espacio del bloque, no termine de escribir
		}

		fprintf(fp,"%s", line_with_string_format);
		free(line);
		size_already_written += line_length;
	}

	fclose(fp);
	return true; //termine de escribir, entraron todos en este bloque :D
}

void write_pokemon_data(t_list* pokemon_data_list, char* blocks){
	int i=1;
	char block_name[12];
	char* block_path;
	bool all_data_writed;
	int blocks_quantity = split(blocks,1,"[,]",block_name);//calculo la cantidad de bloques que hay

	if(is_empty(pokemon_data_list)){
		blocks = "[]"
		log
		return;
	}

	do{
		if(i > blocks_quantity){
			char* new_block_number = get_new_block(); //NECESITO UN BLOQUE NUEVO
			add_to_blocks(blocks, new_block_number); // block_name tiene forma [n1,n2,n3,n] yo quiero borrar ], agregar ",new_block_number]\0"
			block_name = new_block_number;
		}
		else{
			split(blocks,i,"[,]",block_name);
		}
		block_path = create_block_path(block_name);
		all_data_writed = write_until_full(block_path, pokemon_data_list);
		i++;

	}while(!all_data_writed);

	//si i quedo = a cant_bloques o menor, => le sobraron bloques
	while(i <= blocks_quantity){//si NO estoy escribiendo en el ultimo bloque
		split(blocks,blocks_quantity,"[,]",block_name);
		remove_from_blocks(blocks, block_name);
		blocks_quantity--;
		free_block_number(block_name);
	}//borro los bloques que sobraron, deberia ser 1 solo podrian ser mas dependiendo de como se inicie el sistema

}

void* read_file_of_type(uint32_t file_type, char* file_name){

    t_file_information* file_information = file_information_with_code(file_type);

    return (*(file_information -> reader_function)) (file_name);

}

void initialize_file_system(){

    //TODO: verificar si existe el archivo de bitmap, y en caso que no, crearlo a partir de los datos del metadata. inicializar metadata y bitmap

    initialize_files_information();
}
