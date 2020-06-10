#ifndef DELIBIRD_GARBAGE_COLLECTOR_H
#define DELIBIRD_GARBAGE_COLLECTOR_H

typedef struct Garbage{
    void* object;
    void (*object_cleaner_function) (void*);
}t_garbage;

void initialize_garbage_collector();

void consider_as_garbage(void* object, void (*object_cleaner_function) (void*));
void stop_considering_garbage(void* object);

void free_system();
void free_garbage_collector();

#endif //DELIBIRD_GARBAGE_COLLECTOR_H
