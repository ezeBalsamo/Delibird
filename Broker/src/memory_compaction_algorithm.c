#include <commons/collections/list.h>
#include <broker_memory_manager.h>
#include <stdlib.h>
#include "memory_compaction_algorithm.h"
#include "../../Utils/include/t_list_extension.h"


//esta funcion reposiciona un bloque dado, al final de la memoria y de la lista administrativa.
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
}