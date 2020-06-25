#include <commons/config.h>
#include <stdint.h>
#include <commons/bitarray.h>
#include <commons/string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include "filesystem.h"
#include "gamecard_logs_manager.h"
#include "gamecard_configuration_manager.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/common_structures.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/logger.h"

t_list* all_files_information;

char* mount_point;
uint32_t block_size;
uint32_t blocks;
char* magic_number;
t_bitarray* bitmap;

char* create_block_path(char block_pointer[]){
    return string_from_format("%s/Blocks/%s.bin", tallgrass_mount_point(), block_pointer);
}


int split(char string[], int index, char* separator, char found_string[])
{

    int i = 1;
    int initial_size = strlen(string);

    char str1[initial_size];
    strcpy(str1,string);
    char delim[5];
    strcpy(delim,separator);

    char *ptr = strtok(str1, delim);

    while (ptr != NULL)
    {
        if (i == index)
        {
            strcpy(found_string, ptr);
        }
        i++;
        ptr = strtok(NULL, delim);
    }

    return i-1;
}

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

    pokemon_block_line* new_line = malloc(sizeof(pokemon_block_line));

    new_line -> positionX = x;
    new_line -> positionY = y;
    new_line -> pokemonQuantity = quantity;

    return new_line;
}

void change_file_open_flag(FILE* file_pointer, char* new_flag){

    fseek(file_pointer, -2, SEEK_END); // 1 para la y, otro por el \n

    char var[1];
    fscanf(file_pointer, "%c", var);

    if(var[0] == '='){
        fseek(file_pointer, -1, SEEK_END);
    }
    if(var[0] == 'N' || var[0] == 'Y'){
        fseek(file_pointer, -2, SEEK_END);
    }

    fprintf(file_pointer, "%s", new_flag);
}

void set_open(FILE* file_pointer){
    change_file_open_flag(file_pointer, "Y");
}

void set_closed(FILE* file_pointer){
    change_file_open_flag(file_pointer, "N");
}

void close_metadata(char* metadata_path) {

    FILE *file_pointer = fopen(metadata_path, "r+");
    int fd = fileno(file_pointer); //el file descriptor para los flocks :D
    flock(fd, LOCK_SH);

    set_closed(file_pointer);

    fclose(file_pointer);
    flock(fd, LOCK_UN);
}

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

    consider_as_garbage(metadata_config, (void (*)) config_destroy);

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

    //una vez que sali del loop tengo que escribir la Y en el open
    //set_open(file_pointer);
    fclose(file_pointer); //La escritura del flag OPEN se realiza al cerrar el file_pointer

    //Si yo ya lockee el metadata, el que ya tenia abierto el file no deberia poder actualizar el metadata porque lo tengo lockeado yo
    flock(fd,LOCK_UN);

    return (void*) pointer_file_metadata;
}

void* read_block(char* file_path){

    FILE* file_pointer;

    char pointer[MAXLINE];
    char line[MAXLINE];
    char block_pointer[8];
    int blocks_quantity;
    char* final_path;
    t_list* blocks_data = list_create();

    int i = 1;
    do{
        blocks_quantity = split(file_path, i, "[,]", block_pointer);

        //crear_path_bloque(tallgrass_mount_point(), block_pointer, final_path);
        final_path = create_block_path(block_pointer);

        file_pointer = fopen(final_path,"r");

        while(read_line(line, MAXLINE, file_pointer)){

            split(line,1,"-=",pointer);
            int x = atoi(pointer);

            split(line,2,"-=",pointer);
            int y = atoi(pointer);

            split(line,3,"-=",pointer);
            int quantity = atoi(pointer);

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

    switch(file_type){
        case FILESYSTEM_METADATA:{
            //TODO: verificar si existe el archivo, y en caso que no, retornar un valor especifico
            return (fs_metadata*) read_fs_metadata(file_name);

        }
        case ARCHIVO_METADATA:{

            //TODO: verificar si existe el archivo, y en caso que no, retornar un valor especifico
            return (file_metadata*) read_file_metadata(file_name);

        }
        case BLOQUE:{

            return (t_list*) read_block(file_name);

        }
        default:{
            return 0;
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////

/*void initialize_and_load_fs_metadata_information(){
    t_file_information* file_information = safe_malloc(sizeof(t_file_information));
    file_information -> file_type = FS_METADATA;
    file_information -> reader_function = read_fs_metadata;

    list_add(all_files_information, (void*) file_information);
}*/

/*void initialize_and_load_file_metadata_information(){
    t_file_information* file_information = safe_malloc(sizeof(t_file_information));
    file_information -> file_type = FILE_METADATA;
    file_information -> reader_function = read_file_metadata;

    list_add(all_files_information, (void*) file_information);
}*/

/*void initialize_and_load_block_information(){
    t_file_information* file_information = safe_malloc(sizeof(t_file_information));
    file_information -> file_type = BLOCK;
    file_information -> reader_function = read_block;

    list_add(all_files_information, (void*) file_information);
}*/


/*void initialize_files_information(){

    all_files_information = list_create();

    //inicializo estructuras de informacion de archivos
    initialize_and_load_fs_metadata_information();
    initialize_and_load_file_metadata_information();
    initialize_and_load_block_information();
}*/

/*void free_files_information(){
    //TODO: liberar array all_file_information y elementos dentro
}*/

//////////////////////////////////////////////////////////////////////////////////////////////////
void initialize_bitmap(){

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
}


void initialize_metadata(){

    //esta podria ser la funcion reader del fs_metadata
    char* metadata_path = string_from_format("%s/Metadata/Metadata.bin", tallgrass_mount_point());

    //Crear el t_config a partir del archivo mount_point/Metadata/Metadata.bin
    t_config* metadata_config = config_create(metadata_path);

    //Obtener valores blocks_size, blocks, magic_number
    block_size = config_get_int_at("BLOCK_SIZE");
    blocks = config_get_int_at("BLOCKS");
    magic_number = config_get_string_at("MAGIC_NUMBER");

    //Destruir t_config creado
    config_destroy(metadata_config);
}

void initialize_filesystem(){

    mount_point = config_get_string_at("PUNTO_MONTAJE_TALLGRASS");

    //Inicializar metadata y bitmap (estos archivos existen siempre ya que son la base del fs)
    initialize_metadata();
    initialize_bitmap();
 //   initialize_files_information();
}