#include <commons/config.h>
#include <stdint.h>
#include <commons/bitarray.h>
#include <commons/string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include "filesystem.h"
#include "filesystem_files_information.h"
#include "filesystem_utils.h"
#include "gamecard_logs_manager.h"
#include "gamecard_configuration_manager.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/common_structures.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/logger.h"

int maxline = 40;

int read_line(char* buffer, int size, FILE* file_pointer)
{
    buffer[0] = '\0';
    buffer[size - 1] = '\0';             /* mark end of buffer */
    char* temp;

    if (fgets(buffer, size, file_pointer) == NULL) {
        buffer = '\0';                   /* EOF */
        return false;
    }
    else {
        /* remove newline */
        if ((temp = strrchr(buffer, '\n')) != NULL) {
            *temp = '\0';
        }
    }
    return true;
}

pokemon_block_line* create_line(int x, int y, int quantity){

    pokemon_block_line* pokemon_line = malloc(sizeof(pokemon_block_line));

    pokemon_line -> positionX = x;
    pokemon_line -> positionY = y;
    pokemon_line -> pokemonQuantity = quantity;

    return pokemon_line;
}

//Estas 2 funciones se usan para el armado del mensaje LOCALIZED
t_list* create_positions_list(t_list* data_list){

    t_list* positions_list = list_create();

    void _create_position_element(pokemon_block_line* position) {
        uint32_t* positionX = safe_malloc(sizeof(uint32_t));
        uint32_t* positionY = safe_malloc(sizeof(uint32_t));

        *positionX = position -> positionX;
        *positionY = position -> positionY;

        list_add(positions_list, positionX);
        list_add(positions_list, positionY);
    }

    list_iterate(data_list, (void*) _create_position_element);

    return positions_list;
}
uint32_t amount_of_positions(t_list* positions_list){
    return list_size(positions_list) / 2;
}

void* read_fs_metadata(char* file_path){

    FILE* file_pointer = fopen(file_path, "r+");

    fs_metadata* pointer_fs_metadata = (fs_metadata*) malloc(sizeof(fs_metadata));

    //Crear el t_config a partir del archivo mount_point/Metadata/Metadata.bin
    t_config* metadata_config = config_create(file_path);

    //Obtener valores blocks_size, blocks, magic_number
    pointer_fs_metadata -> Block_size = config_get_int_at("BLOCK_SIZE");
    pointer_fs_metadata -> Blocks = config_get_int_at("BLOCKS");
    pointer_fs_metadata -> Magic_Number = config_get_string_at("MAGIC_NUMBER");

    consider_as_garbage(metadata_config, (void (*) (void*)) config_destroy);

    log_filesystem_metadata_info(pointer_fs_metadata);

    fclose(file_pointer);
    return (void*) pointer_fs_metadata;
}

void* read_file_metadata(char* file_path){

    FILE* file_pointer = fopen(file_path, "r+");

    int fd = fileno(file_pointer); //el file descriptor para los flocks :D
    flock(fd, LOCK_SH);

    file_metadata* pointer_file_metadata = (file_metadata*) safe_malloc(sizeof(file_metadata));
    int first_time_reading = 0;
    t_config* metadata_config;

    do{
        if(first_time_reading){
            //Destruir t_config creado
            config_destroy(metadata_config);
            flock(fd,LOCK_UN);
            sleep(op_retry_time());
            flock(fd,LOCK_SH);
        }

    //Crear el t_config a partir del archivo mount_point/Metadata/Metadata.bin
    metadata_config = config_create(file_path);

    //Obtener valores del pokemon_metadata
    pointer_file_metadata -> directory = config_get_string_value(metadata_config, "DIRECTORY");
    pointer_file_metadata -> size = config_get_int_value(metadata_config, "SIZE");
    pointer_file_metadata -> blocks = config_get_string_value(metadata_config, "BLOCKS");
    pointer_file_metadata -> open = config_get_string_value(metadata_config, "OPEN");

    fseek(file_pointer, 0, 0);

    log_file_metadata_info(pointer_file_metadata);

    first_time_reading = 1;

    } while(string_equals_ignore_case(pointer_file_metadata -> open, "Y"));

    consider_as_garbage(metadata_config, (void (*)(void *)) config_destroy);

    //una vez que sali del loop tengo que escribir la Y en el open
    set_open(file_pointer);
    fclose(file_pointer); //La escritura del flag OPEN se realiza al cerrar el file_pointer

    consider_as_garbage(file_path, (void (*) (void*)) close_metadata);

    //Si yo ya lockee el metadata, el que ya tenia abierto el file no deberia poder actualizar el metadata porque lo tengo lockeado yo
    flock(fd,LOCK_UN);

    return (void*) pointer_file_metadata;
}

void* read_block(char* file_path){

    FILE* file_pointer;

    char line_data_parameter[12];
    char line[maxline];
    char block_pointer[12];
    int blocks_quantity;
    char* final_path;
    t_list* blocks_data = list_create();

    int i = 1;
    do{
        blocks_quantity = split(file_path, i, "[,]", block_pointer);

        final_path = create_block_path(block_pointer);

        file_pointer = fopen(final_path, "r");

        while(read_line(line, maxline, file_pointer)){

            split(line, 1, "-=", line_data_parameter);
            int x = atoi(line_data_parameter);

            split(line, 2, "-=", line_data_parameter);
            int y = atoi(line_data_parameter);

            split(line, 3, "-=", line_data_parameter);
            int quantity = atoi(line_data_parameter);

            //Guardarlo en la lista, lo vi asi en las commons el agregado a la lista
            list_add(blocks_data, create_line(x, y, quantity));

            log_block_metadata_info(x, y, quantity);
        }
        fclose(file_pointer);
        i++;
    }
    while(i <= blocks_quantity);

    return (void*) blocks_data;
}

void* read_file_of_type(int file_type, char* file_name){

    t_file_information* file_information = file_information_with_code(file_type);

    return (*(file_information -> reader_function)) (file_name);

}

/*void initialize_bitmap(){

    char* bitmap_path = string_from_format("%s/Metadata/Bitmap.bin", tallgrass_mount_point());

    //Abrir el archivo mount_point/Metadata/Bitmap.bin y guardar el puntero al archivo
    FILE* bitmap_file = fopen(bitmap_path, "r");

    int size;
    char* buffer;
    //Muevo el puntero de archivo a la ultima posicion del archivo
    //OL = long int con todos los bits seteados en 0
    fseek(bitmap_file, 0L, SEEK_END);
    //Me guardo la posicion actual del puntero de archivo, al estar en la ultima posicion, me dira el tamaño
    size = ftell(bitmap_file);
    //Devuelvo el puntero de archivo al principio del archivo
    fseek(bitmap_file, 0L, SEEK_SET);

    buffer = safe_malloc(size);
    //Escribir en buffer, el stream contenido en bitmap_file de tamaño "size"
    fread(buffer, size, 1, bitmap_file);
    //Guardar en buffer los "size" caracteres de buffer como string
    buffer = string_substring_until(buffer, size);

    //Guardo en bitmap el t_bitarray con los datos correspondientes
    bitmap = bitarray_create_with_mode(buffer, blocks / 8, LSB_FIRST);

    fclose(bitmap_file);
}*/

/*void initialize_metadata(){

    //esta podria ser la funcion reader del fs_metadata
    char* metadata_path = string_from_format("%s/Metadata/Metadata.bin", tallgrass_mount_point());

    //Obtener valores blocks_size, blocks, magic_number
    fs_metadata* file_system_metadata = read_fs_metadata(metadata_path);

    //ejecutar funcion donde si no existe el bitmap, se crea el bitarray con la cantidad de blocks del fs_metadata con todos los valores en 0

}*/

void initialize_filesystem(){

    //TODO: verificar si existe el archivo de bitmap, y en caso que no, crearlo a partir de los datos del metadata

    //Inicializar metadata y bitmap

//    initialize_metadata();
//    initialize_bitmap();

    initialize_files_information();
}