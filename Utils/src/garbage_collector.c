#include <garbage_collector.h>
#include <commons/collections/list.h>
#include <common_structures.h>
#include <stdlib.h>
#include <general_logs.h>

t_list* garbage_collector;

void initialize_garbage_collector(){
    garbage_collector = list_create();
}

void consider_as_garbage(void* object, void (*object_cleaner_function) (void*)){
    t_garbage* garbage = safe_malloc(sizeof(t_garbage));
    garbage -> object = object;
    garbage -> object_cleaner_function = object_cleaner_function;

    list_add(garbage_collector, garbage);
}

void stop_considering_garbage(void* object){

    bool _are_equals(void* garbage){
        t_garbage* cast_garbage = (t_garbage*) garbage;
        void* object_to_compare = cast_garbage -> object;
        return object_to_compare == object;
    }

    t_garbage* garbage_found = list_remove_by_condition(garbage_collector, _are_equals);

    if(!garbage_found){
        log_garbage_to_stop_considering_that_not_found_error();
        free_system();
    }

    free(garbage_found);
}

void free_garbage(t_garbage* garbage){
    garbage -> object_cleaner_function (garbage -> object);
    free(garbage);
}

void free_garbage_collector(){
    list_destroy_and_destroy_elements(garbage_collector, (void (*)(void *)) free_garbage);
}