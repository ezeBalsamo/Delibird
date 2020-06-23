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
    int init_size = strlen(string);

    char str1[init_size];
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

int read_line(char *buff, int size, FILE *fp)
{
    buff[0] = '\0';
    buff[size - 1] = '\0';             /* mark end of buffer */
    char *tmp;

    if (fgets(buff, size, fp) == NULL) {
        buff = '\0';                   /* EOF */
        return false;
    }
    else {
        /* remove newline */
        if ((tmp = strrchr(buff, '\n')) != NULL) {
            *tmp = '\0';
        }
    }
    return true;
}

pokemon_block_line* create_line(int x, int y, int quantity){

    pokemon_block_line* new = malloc(sizeof(pokemon_block_line));

    new -> positionX = x;
    new -> positionY = y;
    new -> pokemonQuantity = quantity;

    return new;
}

bool exists_file(char* filename) {
    struct stat buffer;
    if (stat(filename, &buffer) == 0){
        return true;
    }
    else{
        return false;
    }
}

void change_file_open_flag(FILE* fp, char* new_flag){

    fseek(fp, 0, SEEK_END);
    int pos = ftell(fp);
    fseek(fp, pos-2, SEEK_SET); // 1 para la y, otro por el \n

    char var[1];
    fscanf(fp, "%c", var);

    if(var[0] == '='){
        fseek(fp, pos-1, SEEK_SET);
    }
    if(var[0] == 'N' || var[0] == 'Y'){
        fseek(fp, pos-2, SEEK_SET);
    }

    fprintf(fp, "%s\n", new_flag);
}

void* read_file_of_type(int file_type, char* file_name){


    FILE *fp;
    int fd;

    if(file_type != BLOCK){

        //TODO: verificar si existe el archivo, y en caso que no, retornar un valor especifico

        fp = fopen(file_name, "r+");

        if(file_type == FILE_METADATA){
            fd = fileno(fp); //el file descriptor para los flocks :D
            flock(fd, LOCK_SH);
        }
    }

    char pointer[MAXLINE];
    char line[MAXLINE];

    switch(file_type){
        case FS_METADATA:{
            fs_metadata* pointer_fs_metadata = (fs_metadata *) malloc(sizeof(fs_metadata));

            read_line(line, MAXLINE, fp);
            split(line,2,"-=",pointer);
            pointer_fs_metadata -> Block_size = atoi(pointer);

            read_line(line, MAXLINE, fp);
            split(line,2,"-=",pointer);
            pointer_fs_metadata -> Blocks = atoi(pointer);

            read_line(line, MAXLINE, fp);
            split(line,2,"-=",pointer);
            strcpy(pointer_fs_metadata -> Magic_Number, pointer);

            log_filesystem_metadata_info(pointer_fs_metadata);

            fclose(fp);
            return pointer_fs_metadata;
        }
        case FILE_METADATA:{
            file_metadata* pointer_file_metadata = (file_metadata *) malloc(sizeof(file_metadata));
            int first_time_reading = 0;
            do{
                if(first_time_reading){
                    flock(fd,LOCK_UN);
                    sleep(op_retry_time());
                    flock(fd,LOCK_SH);
                }

                read_line(line, MAXLINE, fp);
                split(line,2,"-=",pointer);
                strcpy(pointer_file_metadata->directory,pointer);

                read_line(line, MAXLINE, fp);
                split(line,2,"-=",pointer);
                pointer_file_metadata->size = atoi(pointer);

                read_line(line, MAXLINE, fp);
                split(line,2,"-=",pointer);
                strcpy(pointer_file_metadata->blocks,pointer);
                //pointer_file_metadata.blocks = pointer;

                read_line(line, MAXLINE, fp);
                split(line,2,"-=",pointer);
                strcpy(pointer_file_metadata->open,pointer);

                fseek(fp, 0, 0);

                log_file_metadata_info(pointer_file_metadata);

                first_time_reading = 1;

            }while(pointer_file_metadata -> open[0] == 'Y');

            //una vez que sali del loop tengo que escribir la Y en el open
            change_file_open_flag(fp, "Y");

            fclose(fp);
            //Si yo ya lockee el metadata, el que ya tenia abierto el file no deberia poder actualizar el metadata porque lo tengo lockeado yo
            flock(fd,LOCK_UN);

            return pointer_file_metadata;
        }
        case BLOCK:{

            char block_pointer[8];
            int blocks_quantity;
            char* final_path;
            t_list* blocks_data = list_create();

            int i=1;
            do{
                blocks_quantity = split(file_name, i, "[,]", block_pointer);

                //crear_path_bloque(tallgrass_mount_point(), block_pointer, final_path);
                final_path = create_block_path(block_pointer);

                fp = fopen(final_path,"r");

                while(read_line(line, MAXLINE, fp)){

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
                fclose(fp);
                i++;
            }
            while(i <= blocks_quantity);

            return blocks_data;
        }
        default:{
            return 0;
        }
    }
}




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
}