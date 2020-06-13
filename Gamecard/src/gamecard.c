#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/common_structures.h"
#include <stdlib.h>

int main() {
    initialize_signal_handler();
    initialize_garbage_collector();

    free_system();
	return EXIT_SUCCESS;
}