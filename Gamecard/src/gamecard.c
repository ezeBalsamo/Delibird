#include "../../Utils/include/garbage_collector.h"
#include <stdlib.h>

int main() {
    initialize_garbage_collector();

    free_system();
	return EXIT_SUCCESS;
}
