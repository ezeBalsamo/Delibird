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
                (*comparer) (element_to_compare, element_to_find);
            }

            list_remove_by_condition(result, _comparer);
        }
    }

    return result;
}