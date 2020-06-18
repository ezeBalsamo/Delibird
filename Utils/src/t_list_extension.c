#include "../include/t_list_extension.h"

t_list* list_flat(t_list* self){
    t_list* flattened_list = list_create();

    void _add_all(void* list_element){
        list_add_all(flattened_list, list_element);
    }

    list_iterate(self, _add_all);

    return flattened_list;
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

t_list* list_swap(t_list* self, int i ,int j){
    if (list_size(self)< i || list_size(self)<j){
        //TODO: romper
    }
    void* element_to_swap = list_get(self,i);
    void* second_element_to_swap = list_get(self,j);

    list_replace(self,i,second_element_to_swap);
    list_replace(self,j,element_to_swap);
}  
void list_add_as_set(t_list* self, void* element){

    bool _equality(void* element_to_find, void* element_to_compare){
        return element_to_find == element_to_compare;
    }

    if(!list_contains(self, element, _equality)){
        list_add(self, element);
    }
}