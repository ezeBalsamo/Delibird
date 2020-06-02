#include <team_serializable_objects.h>
#include <team_logs_manager.h>
#include <stdlib.h>
#include "../../Utils/include/pretty_printer.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/general_logs.h"
#include "free_team.h"

void free_system(){
    free_pretty_printer();
    free_team_serializable_objects();
    free_configuration_manager();

    log_successful_clean_up();
    free_team_logs_manager();

     exit(EXIT_SUCCESS);
}