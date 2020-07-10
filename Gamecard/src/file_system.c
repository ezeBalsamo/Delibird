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

void* read_file_of_type(uint32_t file_type, char* file_name){

    t_file_information* file_information = file_information_with_code(file_type);

    return (*(file_information -> reader_function)) (file_name);

}

void initialize_file_system(){

    //TODO: verificar si existe el archivo de bitmap, y en caso que no, crearlo a partir de los datos del metadata. inicializar metadata y bitmap

    initialize_files_information();
}