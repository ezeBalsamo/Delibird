#include <signal.h>
#include "../include/broker_signal_handler.h"
#include "../../Utils/include/common_structures.h"

void initialize_broker_signal_handler(){
    initialize_signal_handler();
    signal(SIGPIPE, SIG_IGN);
}
