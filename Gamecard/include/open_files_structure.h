/*
 * open_files_structure.h
 *
 *  Created on: 16 jul. 2020
 *      Author: utnso
 */

#ifndef GAMECARD_INCLUDE_OPEN_FILES_STRUCTURE_H_
#define GAMECARD_INCLUDE_OPEN_FILES_STRUCTURE_H_

typedef struct{
	char* metadata_path;
	unsigned int tid;
}open_file_by_tid;

void remove_from_open_files(char* metadata_path);
void add_to_open_files(char* metadata_path);
void close_open_files();
void initialize_open_files_list();

#endif /* GAMECARD_INCLUDE_OPEN_FILES_STRUCTURE_H_ */
