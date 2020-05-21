#ifndef TP_2020_1C_EL_KUELGUE_BROKER_DESERIALIZATION_H
#define TP_2020_1C_EL_KUELGUE_BROKER_DESERIALIZATION_H

#include "../../Utils/include/serialization_interface.h"

void* deserialize(void* serialized_request);

uint32_t get_operation_from(void* serialized_request);

t_connection_request * create_connection_request(int connection_fd, void* serialized_structure);

#endif //TP_2020_1C_EL_KUELGUE_BROKER_DESERIALIZATION_H
