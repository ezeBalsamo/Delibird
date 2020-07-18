#include <commons/collections/list.h>
#include <broker_memory_manager.h>
#include <stdlib.h>
#include "memory_compaction_algorithm.h"
#include "../../Utils/include/t_list_extension.h"
#include "../../Utils/include/configuration_manager.h"


/*//esta funcion reposiciona un bloque dado, al final de la memoria y de la lista administrativa.
//Mover un bloque en memoria implica ajustar la posicion de todos los siguientes al dado "1 bloque para atras" y attacheando el dado a la posicion del ultimo (luego de reajustarlo)
void reposition_free_block_to_end(t_block_information *block_to_reposition, t_list *blocks_information,int block_index){
    void* initial_position_to_occupy_for_next_block = block_to_reposition->initial_position;

    //ajustar las posiciones de todos los bloques (como si se hubiera borrado el que quiero reposicionar)
    for(int i = block_index+1; i<list_size(blocks_information)-1;i++){
        t_block_information* block_to_adjust = list_get(blocks_information,i);
        block_to_adjust ->initial_position = initial_position_to_occupy_for_next_block;

        initial_position_to_occupy_for_next_block = block_to_adjust->initial_position+block_to_adjust->block_size;
    }
    //moverlo al final de la memoria y de la lista administrativa
    t_block_information* last_block = list_get_last_element(blocks_information);
    if(last_block != block_to_reposition){
        block_to_reposition->initial_position = last_block->initial_position + last_block->block_size;
        list_remove(blocks_information,block_index);
        list_add(blocks_information,block_to_reposition);
    }
}
//esta funcion supone que a partir del master block (primer bloque libre), todos los siguientes bloques van a estar libres
//mejora para mañana: el master block podria obtenerse al principio y no hacer un get en cada iteracion al dope
//va hasta list_size -1, porque no tiene sentido compactar el ultimo bloque
void combine_all_free_partitions(t_list* blocks_information){
    for (int i = 0; i < list_size(blocks_information)-1;i++){
        t_block_information* master_block = (t_block_information*) list_get(blocks_information,i);
        if (master_block->is_free){

            t_block_information* block_to_compact = (t_block_information*) list_remove(blocks_information,i+1);

            master_block->block_size += block_to_compact->block_size;
            free(block_to_compact);
            //analizo el tamaño de la lista en cada iteracion ya que fue achicando su tamaño
            if ((i+1) == list_size(blocks_information)){
                break;
            }
            i--; //ya que achique su tamaño, si no disminuyo este i, en la siguiente iteracion voy a peder un bloque
        }
    }
}

void memory_compaction_algorithm(t_list* blocks_information){

    for (int i = 0; i < list_size(blocks_information);i++){

        t_block_information* block_information = (t_block_information*) list_get(blocks_information,i);

        if (block_information->is_free){
            reposition_free_block_to_end(block_information,blocks_information,i);
        }
    }
    //Combinar particiones vacias contiguas a 1 sola particion vacia de mayor tamaño
    combine_all_free_partitions(blocks_information);
}*/

int next_occupied_block_information_index_using(int free_block_information_index, t_list* blocks_information){

    if(list_size(blocks_information) == free_block_information_index){
        return free_block_information_index;
    }

    int next_occupied_block_information_index = free_block_information_index;
    t_block_information* block_information = list_get(blocks_information, free_block_information_index + 1);

    while(block_information -> is_free){
        next_occupied_block_information_index += 1;
        block_information = list_get(blocks_information, next_occupied_block_information_index);
    }

    return next_occupied_block_information_index;
}

void* reposition_block_information(void* initial_position, t_block_information* block_information){
    block_information -> initial_position = initial_position;
    return block_information -> initial_position + block_information -> block_size;
}

bool is_free(t_block_information* block_information){
    return block_information -> is_free;
}

void find_and_load_first_free_block_information(t_list* blocks_information,
                                                int* first_free_block_information_index,
                                                t_block_information** first_free_block_information){

    bool found = false;

    for(int i = 0; !found; i++){

        t_block_information* block_information = list_get(blocks_information, i);

        if(is_free(block_information)){
            *first_free_block_information_index = i;
            *first_free_block_information = block_information;
            found = true;
        }
    }
}

void reposition_occupied_blocks_information(int first_free_block_information_index,
                                            void* next_initial_position,
                                            int* occupied_blocks_size,
                                            t_list* blocks_information){

    for (int i = first_free_block_information_index; i < list_size(blocks_information); i++) {

        int next_occupied_block_information_index =
                next_occupied_block_information_index_using(i, blocks_information);

        t_block_information* occupied_block_information =
                list_get(blocks_information, next_occupied_block_information_index);

        next_initial_position =
                reposition_block_information(next_initial_position, occupied_block_information);

        *occupied_blocks_size += occupied_block_information -> block_size;
        i = next_occupied_block_information_index;
    }
}

void build_and_add_free_partition_block_information(void* next_initial_position,
                                                    int occupied_blocks_size,
                                                    t_list* blocks_information){

    int total_memory_size = config_get_int_at("TAMANO_MEMORIA");

    t_block_information* free_block_information = safe_malloc(sizeof(t_block_information));
    free_block_information -> is_free = true;
    free_block_information -> initial_position = next_initial_position;
    free_block_information -> memory_block = NULL;
    free_block_information -> block_size = total_memory_size - occupied_blocks_size;

    list_add(blocks_information, free_block_information);
}

void memory_compaction_algorithm(t_list* blocks_information) {

    int occupied_blocks_size = 0;

    int first_free_block_information_index;
    t_block_information* first_free_block_information;

    find_and_load_first_free_block_information(blocks_information,
                                               &first_free_block_information_index,
                                               &first_free_block_information);

    void* next_initial_position = first_free_block_information -> initial_position;

    reposition_occupied_blocks_information(first_free_block_information_index,
                                           next_initial_position,
                                           &occupied_blocks_size,
                                           blocks_information);

    list_remove_and_destroy_all_by_condition(blocks_information, (bool (*)(void *)) is_free, free);

    build_and_add_free_partition_block_information(next_initial_position, occupied_blocks_size, blocks_information);
}