#include <commons/collections/queue.h>
#include "../include/t_list_extension.h"

t_list* list_flat(t_list* self){
    t_list* flattened_list = list_create();

    void _add_all(void* list_element){
        list_add_all(flattened_list, list_element);
    }

    list_iterate(self, _add_all);

    return flattened_list;
}

void free_list_to_flat(t_list* self){
    list_iterate(self, (void (*)(void *)) list_destroy);
    list_destroy(self);
}

bool list_contains(t_list* self, void* element_to_find, bool (*comparer) (void*, void*)){

    for(int i = 0; i < list_size(self); i++){

        void* element_to_compare = list_get(self, i);

        if((*comparer)(element_to_find, element_to_compare)){
            return true;
        }
    }
    return false;
}

t_list* list_difference(t_list* self, t_list* other, bool (*comparer) (void*, void*)){
    t_list* result = list_duplicate(self);

    for(int i = 0; i < list_size(other); i++){

        void* element_to_find = list_get(other, i);

        if(list_contains(self, element_to_find, comparer)){

            bool _comparer(void* element_to_compare){
                return (*comparer) (element_to_compare, element_to_find);
            }

            list_remove_by_condition(result, _comparer);
        }
    }

    return result;
}
void list_of_lists_destroy_and_destroy_elements(t_list* self,void(*element_destroyer)(void*)){
    for(int i = 0; i < list_size(self); i++){
        t_list* internal_list = (t_list*) list_get(self, i);
        list_clean_and_destroy_elements(internal_list, element_destroyer);
    }

    list_destroy_and_destroy_elements(self, (void (*)(void *)) list_destroy);
}

bool are_equal_lists(t_list* one_list, t_list* another_list)
{
    bool partial_result;
    bool iterate_result;
    if(list_size(one_list) == list_size(another_list)){
        for(int i = 0; i < list_size(one_list);i++){
         void* one_element = list_get(one_list, i);
         void* another_element = list_get(another_list, i);
         partial_result = one_element == another_element;
         iterate_result = partial_result * iterate_result;
        }
        return iterate_result;
    }
}

void queue_iterator(t_queue* queue, void(*closure)(void*)){
    return list_iterate(queue -> elements, closure);
}