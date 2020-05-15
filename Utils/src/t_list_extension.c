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