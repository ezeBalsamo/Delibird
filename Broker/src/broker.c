#include "../../Utils/include/socket.h"
#include "../include/queue_message_manager.h"
#include "../../Utils/include/configuration_manager.h"

char* port(){
    return config_get_string_at("PUERTO_BROKER");
}

void main_thread_handler(void* serialized_request){

    deserialize(serialized_request);

}

int main() {

    initialize_message_manager();

    start_multithreaded_server(port(), main_thread_handler);

}
