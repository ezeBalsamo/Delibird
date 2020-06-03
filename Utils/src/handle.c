#include <commons/collections/list.h>
#include <stdint.h>
#include <common_structures.h>
#include "handle.h"

void* object_can_handle(t_list* objects_list, uint32_t operation){

    bool _can_handle(void* object){
        t_manageable_object* cast_object = (t_manageable_object*) object;
        return (*(cast_object -> can_handle_function)) (operation);
    }

    void* handler_object = list_remove_by_condition(objects_list, _can_handle);

    return handler_object;
}
