#include <commons/config.h>
#include <stdint.h>
#include <commons/bitarray.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "file_system.h"
#include "file_system_files_information.h"
#include "file_system_utils.h"
#include "gamecard_logs_manager.h"
#include "gamecard_configuration_manager.h"
#include "../../Utils/include/common_structures.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/logger.h"
#include "../../Utils/include/paths.h"
#include "../../Utils/include/t_list_extension.h"
#include "open_files_structure.h"
#include "../../Utils/include/pthread_wrapper.h"


t_bitarray* bitmap;
t_file_system_metadata* file_system_metadata;

pthread_mutex_t bitmap_mutex;

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

    FILE* file_pointer = fopen(file_path, "r");

    //Crear el t_config a partir del archivo mount_point/Metadata/Metadata.bin
    t_config* metadata_config = config_create(file_path);
    t_file_system_metadata* file_metadata = read_file_system_metadata_from_config(metadata_config);
    config_destroy(metadata_config);

    log_file_system_metadata_info(file_metadata);

    fclose(file_pointer);
    return file_metadata;
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

    config_destroy(metadata_config);

    set_open(file_pointer); //Una vez que sali del loop tengo que escribir la Y en el open
    add_to_open_files(file_path);
    fclose(file_pointer); //La escritura del flag OPEN se realiza al cerrar el file_pointer

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

        free(final_path);

        while(can_read_line(line, maximum_length_of_line, file_pointer)){

            split(line, 1, "-=", line_data_parameter);
            uint32_t x = atoi(line_data_parameter);

            split(line, 2, "-=", line_data_parameter);
            uint32_t y = atoi(line_data_parameter);

            split(line, 3, "-=", line_data_parameter);
            uint32_t quantity = atoi(line_data_parameter);

            //Guardarlo en la lista, lo vi asi en las commons el agregado a la lista
            t_pokemon_block_line* pokemon_line = create_pokemon_block_line(x, y, quantity);
            list_add(blocks_data, pokemon_line);

            log_block_metadata_info(x, y, quantity);
        }
        fclose(file_pointer);
        i++;
    }
    while(i <= blocks_quantity);

    return blocks_data;
}

t_pokemon_block_line* find_line_with_position(uint32_t position_x, uint32_t position_y, t_list* list, uint32_t* index){
	uint32_t i = 0;
	uint32_t auxiliar_index;
	bool found_flag = false;

	void _positions_match(t_pokemon_block_line *position_in_list) {
		if(position_in_list -> position_x == position_x && position_in_list -> position_y == position_y){
			auxiliar_index = i;
			found_flag = true;
			return;
		}
		i++;
	}

	list_iterate(list, (void*) _positions_match);

	if(!found_flag){
		return NULL;
	} else {
		if(index != NULL){
			*index = auxiliar_index;
		}
		return list_get(list, auxiliar_index);
	}
}

bool there_are_no_more_pokemons_in(t_pokemon_block_line* requested_line){
       return (requested_line -> quantity - 1) == 0;
}

void remove_pokemon_line_with_index(t_list* blocks_information, uint32_t index){
    list_remove(blocks_information, index);
}

bool subtract_pokemon_from(t_list* blocks_information, t_catch_pokemon* pokemon_to_subtract){

	uint32_t index;

	t_pokemon_block_line* pokemon_line = find_line_with_position(pokemon_to_subtract -> pos_x, pokemon_to_subtract -> pos_y, blocks_information, &index);

	if (pokemon_line == NULL){
        log_pokemon_not_found_error(pokemon_to_subtract);
	    return false;
	}
	else{
		if(there_are_no_more_pokemons_in(pokemon_line)){
			remove_pokemon_line_with_index(blocks_information, index);
			free(pokemon_line);
		}
		else{
            pokemon_line -> quantity -= 1;
		}
		return true;
	}
}

bool is_a_new_pokemon_line (t_pokemon_block_line* pokemon_block_line){
    return pokemon_block_line == NULL;
}

void add_pokemon_line_to(t_list* blocks_information, t_new_pokemon* pokemon_to_add){
    t_pokemon_block_line* new_pokemon_line = create_pokemon_block_line(pokemon_to_add -> pos_x, pokemon_to_add -> pos_y, pokemon_to_add -> quantity);
    list_add(blocks_information, new_pokemon_line);
}

void update_pokemon_line_to(t_list* blocks_information, t_new_pokemon* pokemon_to_add){

	t_pokemon_block_line* requested_line = find_line_with_position(pokemon_to_add -> pos_x, pokemon_to_add -> pos_y, blocks_information, NULL);

	if(is_a_new_pokemon_line(requested_line)){
	    add_pokemon_line_to(blocks_information, pokemon_to_add);
	} else {
        requested_line -> quantity += pokemon_to_add -> quantity;
	}
}

char* remove_from_blocks(char* pokemon_blocks, char* block_to_remove){
	uint32_t aux = strlen(block_to_remove) + 2; //uno por el ']' y otro por la ','
	pokemon_blocks = string_substring_until(pokemon_blocks, strlen(pokemon_blocks) - aux);
	string_append(&pokemon_blocks, "]");
	return pokemon_blocks;
}

char* add_to_blocks(char* string_to_modify, char* block_to_add){
	string_to_modify = string_substring(string_to_modify,0,strlen(string_to_modify)-1); //recorto el ']'
	string_append_with_format(&string_to_modify, ",%s]",block_to_add); //agrego el bloque nuevo y ]
	return string_to_modify;
}

char* get_new_block(){
	char* block_number_string_format;
	uint32_t index = 0;

    safe_mutex_lock(&bitmap_mutex);
	//<-------region critica-------->
	while(bitarray_test_bit(bitmap,index)){
		index++;
	}//itero mientras los bits esten en 1, hasta encontrar un 0

	bitarray_set_bit(bitmap, index);
    //<-------region critica-------->
    safe_mutex_unlock(&bitmap_mutex);

	block_number_string_format = string_from_format("%d",index);
	return block_number_string_format;
}

void free_block_number(char* block_name){
	uint32_t index = atoi(block_name);

    safe_mutex_lock(&bitmap_mutex);
    //<-------region critica-------->
	bitarray_clean_bit(bitmap,index);
    //<-------region critica-------->
    safe_mutex_unlock(&bitmap_mutex);
}

bool write_until_full(char* block_path, t_list* pokemon_data_list, uint32_t* total_size){

	FILE* fp = fopen(block_path,"w");

	uint32_t size_already_written = 0;
	char* line_with_string_format;
	uint32_t number_of_lines_to_write = pokemon_data_list -> elements_count;

	for(uint32_t i = 0; i < number_of_lines_to_write; i++){
		t_pokemon_block_line* line = list_remove_first(pokemon_data_list);

		line_with_string_format = block_line_to_string(line);

		int line_length = strlen(line_with_string_format);

		if(size_already_written + line_length > file_system_metadata -> block_size){ //si la linea no me entra en el bloque,
		                                                                             // la mando de nuevo a la lista
			list_add(pokemon_data_list,line);
			fclose(fp);
			*total_size += size_already_written;
			free(line_with_string_format);
			return false; //se acabo el espacio del bloque, no termine de escribir
		}

		fprintf(fp,"%s", line_with_string_format);
		free(line);
        free(line_with_string_format);
		size_already_written += line_length;
	}

	fclose(fp);
	*total_size += size_already_written;
	return true; //termine de escribir, entraron todos en este bloque :D
}

t_list* data_to_write(t_new_pokemon* new_pokemon){//hago esto para que coincida con el tipo de dato que espera la funcion que escribe
	t_list* line = list_create();
    t_pokemon_block_line * pokemon_block_line = create_pokemon_block_line(new_pokemon -> pos_x, new_pokemon -> pos_y, new_pokemon -> quantity);
	list_add(line, pokemon_block_line);
	return line;
}

void write_pokemon_metadata(t_file_metadata* metadata_file_information, char* pokemon_metadata_path){

    if(string_equals_ignore_case(metadata_file_information -> blocks, "[]")){//el pokemon ya no tiene bloques

		remove(pokemon_metadata_path);
		char* pokemon_path = string_substring_until(pokemon_metadata_path,strlen(pokemon_metadata_path)-strlen("/Metadata.bin"));
		remove(pokemon_path);
		char* pokemon_name = get_pokemon_name_from_path(pokemon_path);
		log_pokemon_file_destroyed(pokemon_name);
		free(pokemon_path);
		free(pokemon_name);

	} else{

        char* line_to_write;
		FILE* file_pointer = fopen(pokemon_metadata_path, "w");
		int file_descriptor = fileno(file_pointer);

		flock(file_descriptor, LOCK_SH);

		line_to_write = string_from_format("DIRECTORY=%s\n" ,metadata_file_information -> directory);
		fprintf(file_pointer, "%s", line_to_write);
		free(line_to_write);
		line_to_write = string_from_format("SIZE=%d\n" ,metadata_file_information -> size);
		fprintf(file_pointer, "%s", line_to_write);
        free(line_to_write);
		line_to_write = string_from_format("BLOCKS=%s\n" ,metadata_file_information -> blocks);
		fprintf(file_pointer, "%s", line_to_write);
        free(line_to_write);
		line_to_write = string_from_format("OPEN=%s\n" ,metadata_file_information -> open);
		fprintf(file_pointer, "%s", line_to_write);
        free(line_to_write);

		fclose(file_pointer);
		flock(file_descriptor,LOCK_UN);
	}
}

void write_pokemon_blocks(t_list* pokemon_data_list, t_file_metadata* metadata_file_information){
	uint32_t i = 1;
	char block_name[12];
	char* block_path;
	bool all_data_writed;
	uint32_t total_size = 0;
	uint32_t blocks_quantity = split(metadata_file_information -> blocks, 1, "[,]", block_name);//calculo la cantidad de bloques que hay

	if(list_is_empty(pokemon_data_list)){
        free_block_number(block_name);
		char* block_path = create_block_path(block_name);
		remove(block_path);
		free(block_path);
		free(metadata_file_information -> blocks);
		metadata_file_information -> blocks = string_duplicate("[]");
		return;
	}

	do{
		if(i > blocks_quantity){
			char* new_block_number = get_new_block(); //NECESITO UN BLOQUE NUEVO
			metadata_file_information -> blocks = add_to_blocks(metadata_file_information -> blocks, new_block_number); // block_name tiene forma [n1,n2,n3,n] yo quiero borrar ], agregar ",new_block_number]\0"
			strcpy(block_name, new_block_number);
		}
		else{
			split(metadata_file_information -> blocks, i, "[,]", block_name);
		}
		block_path = create_block_path(block_name);
		all_data_writed = write_until_full(block_path, pokemon_data_list, &total_size);
		free(block_path);

		i++;

	}while(!all_data_writed);

	//si i quedo = a cant_bloques o menor, => le sobraron bloques
	while(i <= blocks_quantity){//si NO estoy escribiendo en el ultimo bloque
		split(metadata_file_information -> blocks,blocks_quantity,"[,]",block_name);
		metadata_file_information -> blocks = remove_from_blocks(metadata_file_information -> blocks, block_name);
		blocks_quantity--;
		free_block_number(block_name);
		remove(create_block_path(block_name));
	}//borro los bloques que sobraron, deberia ser 1 solo podrian ser mas dependiendo de como se inicie el sistema

	metadata_file_information -> size = total_size;
}

void create_pokemon_metadata(char* pokemon_name){

    char* pokemon_metadata_path = string_from_format("%s/Files/%s", tallgrass_mount_point(), pokemon_name);
	mode_t old_mask = umask(0);
    mkdir(pokemon_metadata_path, 0777);
    umask(old_mask);
    free(pokemon_metadata_path);
}

void* read_file_of_type(uint32_t file_type, char* file_name){

    t_file_information* file_information = file_information_with_code(file_type);

    return file_information -> reader_function (file_name);
}

void create_bitmap_using(FILE* bitmap_file, uint32_t bitmap_size_in_bytes){
    int file_descriptor = fileno(bitmap_file);

    ftruncate(file_descriptor, bitmap_size_in_bytes);

    char* bitmap_buffer = mmap(NULL, bitmap_size_in_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, file_descriptor, 0);

    bitmap = bitarray_create_with_mode(bitmap_buffer, bitmap_size_in_bytes, LSB_FIRST);
}

void initialize_bitmap(){

    char* bitmap_path = string_from_format("%s/Metadata/Bitmap.bin", tallgrass_mount_point());
    uint32_t bitmap_size_in_bytes = (file_system_metadata -> blocks) / 8;
    FILE* bitmap_file;

	if(exists_file_at(bitmap_path)){

		bitmap_file = fopen(bitmap_path, "r+");
        free(bitmap_path);
        create_bitmap_using(bitmap_file, bitmap_size_in_bytes);
	}else{

		bitmap_file = fopen(bitmap_path, "w+");
        free(bitmap_path);
        create_bitmap_using(bitmap_file, bitmap_size_in_bytes);

		for(uint32_t i = 0; i < file_system_metadata -> blocks; i++){
			bitarray_clean_bit(bitmap, i);
		}

		fprintf(bitmap_file, "%s", bitmap -> bitarray);
	}
	fclose(bitmap_file);
}

void initialize_metadata(){

    char* metadata_path = string_from_format("%s/Metadata/Metadata.bin", tallgrass_mount_point());
    file_system_metadata = read_file_of_type(FILE_SYSTEM_METADATA, metadata_path); //Obtener valores blocks_size, blocks, magic_number
    free(metadata_path);
}

void initialize_file_system(){

    initialize_files_information();
    initialize_metadata();
    safe_mutex_initialize(&bitmap_mutex);
 	initialize_bitmap();
}

void free_file_system(){
    safe_mutex_destroy(&bitmap_mutex);
    munmap(bitmap -> bitarray, bitmap -> size);
    bitarray_destroy(bitmap);
    free_file_system_metadata(file_system_metadata);
}