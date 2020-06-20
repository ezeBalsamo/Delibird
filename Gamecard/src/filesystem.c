#include <commons/config.h>
#include <stdint.h>
#include <commons/bitarray.h>
#include <commons/string.h>
#include <stdio.h>
#include <stdlib.h>
#include "filesystem.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/common_structures.h"

char* mount_point;
uint32_t block_size;
uint32_t blocks;
char* magic_number;
t_bitarray* bitmap;

void initialize_bitmap(){

    char* bitmap_path = string_from_format("%s/Metadata/Bitmap.bin", mount_point);

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

    char* metadata_path = string_from_format("%s/Metadata/Metadata.bin", mount_point);

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