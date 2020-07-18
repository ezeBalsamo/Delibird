#include <logger.h>
#include <commons/string.h>
#include <garbage_collector.h>
#include <general_logs.h>
#include "../include/t_list_extension.h"

t_list* list_flat(t_list* self){
    t_list* flattened_list = list_create();

    void _add_all(void* list_element){
        list_add_all(flattened_list, list_element);
    }

    list_iterate(self, _add_all);

    return flattened_list;
}

bool list_includes(t_list* self, void* element_to_find, bool (*comparer) (void*, void*)){

    bool _is_equals(void* element_to_compare){
        return (*comparer) (element_to_find, element_to_compare);
    }

    return list_any_satisfy(self, _is_equals);
}

t_list* list_difference(t_list* self, t_list* other, bool (*comparer) (void*, void*)){
    t_list* result = list_duplicate(self);

    for(int i = 0; i < list_size(other); i++){

        void* element_to_find = list_get(other, i);

        if(list_includes(self, element_to_find, comparer)){

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

void list_swap(t_list* self, int index ,int another_index){

    if (list_size(self)< index || list_size(self)<another_index){
        log_list_invalid_index_access( index,  another_index, self);
        free_system();
    }
    void* element_to_swap = list_get(self,index);
    void* second_element_to_swap = list_get(self,another_index);

    list_replace(self,index,second_element_to_swap);
    list_replace(self,another_index,element_to_swap);
}  
void list_add_as_set(t_list* self, void* element){

    bool _equality(void* element_to_find, void* element_to_compare){
        return element_to_find == element_to_compare;
    }

    if(!list_includes(self, element, _equality)){
        list_add(self, element);
    }
}

void list_add_as_first(t_list* self, void* element){
    list_add_in_index(self, 0, element);
}

void* list_remove_first(t_list* self){
    return list_remove(self, 0);
}

void* list_first(t_list* self){
    return list_get(self, 0);
}

t_list* list_intersection(t_list* self, t_list* another, bool (*comparer) (void*, void*)){

    t_list* list_intersection = list_create();

    void _load_if_belongs_to_both(void* element_to_find){
        if(list_includes(another, element_to_find, comparer)){
            list_add(list_intersection, element_to_find);
        }
    }

    list_iterate(self, _load_if_belongs_to_both);
    return list_intersection;
}

void list_remove_all_by_condition(t_list* self, bool (*comparer) (void*)){

    t_list* elements_to_remove = list_filter(self, comparer);

    void _remove_it(void* element_to_remove){

        bool _are_equals(void* element_to_compare){
            return element_to_compare == element_to_remove;
        }

        list_remove_by_condition(self, _are_equals);
    }

    list_iterate(elements_to_remove, _remove_it);
    list_destroy(elements_to_remove);
}

void list_remove_and_destroy_all_by_condition(t_list* self, bool (*comparer) (void*), void (*sanitizer) (void*)){

    t_list* elements_to_remove = list_filter(self, comparer);

    void _remove_it(void* element_to_remove){

        bool _are_equals(void* element_to_compare){
            return element_to_compare == element_to_remove;
        }

        list_remove_and_destroy_by_condition(self, _are_equals, sanitizer);
    }

    list_iterate(elements_to_remove, _remove_it);
    list_destroy(elements_to_remove);
}

bool is_valid_index(t_list* self, int index){
    return (list_size(self) > index) && (index >= 0);
}

void* list_get_last_element(t_list* self){
    return list_get(self, list_size(self) - 1);
}