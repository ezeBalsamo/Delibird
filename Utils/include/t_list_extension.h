#ifndef DELIBIRD_T_LIST_EXTENSION_H
#define DELIBIRD_T_LIST_EXTENSION_H

#include <commons/collections/list.h>

t_list* list_flat(t_list* self);
t_list* list_difference(t_list* self, t_list* other, bool (*comparer) (void*, void*));
t_list* list_swap(t_list* self, int i ,int j);
void list_of_lists_destroy_and_destroy_elements(t_list* self,void(*element_destroyer)(void*));
bool list_contains(t_list* self, void* element_to_find, bool (*comparer) (void*, void*));
void list_add_as_set(t_list* self, void* element);

#endif //DELIBIRD_T_LIST_EXTENSION_H
