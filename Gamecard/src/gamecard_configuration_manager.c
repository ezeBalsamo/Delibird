#include "gamecard_configuration_manager.h"
#include "../../Utils/include/configuration_manager.h"

char* ip_broker;
char* port_broker;
int timeout_in_seconds;
int operation_delay_time;
int operation_retry_time;
char* mount_point;

void initialize_gamecard_configuration_manager(){
    initialize_configuration_manager_named("gamecard");

    ip_broker = config_get_string_at("IP_BROKER");
    port_broker = config_get_string_at("PUERTO_BROKER");
    timeout_in_seconds = config_get_int_at("ACK_TIMEOUT");
    operation_delay_time = config_get_int_at("TIEMPO_RETARDO_OPERACION");
    operation_retry_time = config_get_int_at("TIEMPO_DE_REINTENTO_OPERACION");
    mount_point = config_get_string_at("PUNTO_MONTAJE_TALLGRASS");
}

char* broker_ip(){
    return ip_broker;
}

char* broker_port(){
    return port_broker;
}

int ack_timeout(){
    return timeout_in_seconds;
}

int op_delay_time(){
    return operation_delay_time;
}

int op_retry_time(){
    return operation_retry_time;
}

char* tallgrass_mount_point(){
    return mount_point;
}