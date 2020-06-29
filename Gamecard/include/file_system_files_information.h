#ifndef DELIBIRD_FILE_SYSTEM_FILES_INFORMATION_H
#define DELIBIRD_FILE_SYSTEM_FILES_INFORMATION_H

#include <stdint.h>

typedef struct File_information{
    uint32_t file_type;
    void* (*reader_function) (char* file_path);
}t_file_information;

void initialize_files_information();
void free_files_information();
t_file_information* file_information_with_code(uint32_t file_type_code);

#endif //DELIBIRD_FILE_SYSTEM_FILES_INFORMATION_H
