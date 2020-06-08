#include <commons/collections/list.h>
#include <fifo_scheduling_algorithm.h>
#include <round_robin_scheduling_algorithm.h>
#include <preemptive_sjf_scheduling_algorithm.h>
#include <non_preemptive_sjf_scheduling_algorithm.h>
#include <team_logs_manager.h>
#include "scheduling_algorithm_finder.h"
#include "../../Utils/include/configuration_manager.h"
#include "../../Utils/include/free_system.h"
#include <stdlib.h>

t_list* scheduling_algorithms;
char* scheduling_algorithm_name;

void initialize_scheduling_algorithms(){
    initialize_fifo_scheduling_algorithm();
    initialize_round_robin_scheduling_algorithm();
    initialize_preemptive_sjf_scheduling_algorithm();
    initialize_non_preemptive_sjf_scheduling_algorithm();

    scheduling_algorithms = list_create();
    list_add(scheduling_algorithms, fifo_scheduling_algorithm());
    list_add(scheduling_algorithms, round_robin_scheduling_algorithm());
    list_add(scheduling_algorithms, non_preemptive_sjf_scheduling_algorithm());
    list_add(scheduling_algorithms, preemptive_sjf_scheduling_algorithm());
}

bool can_handle(t_scheduling_algorithm* scheduling_algorithm){
    return scheduling_algorithm -> can_handle_function (scheduling_algorithm_name);
}

void free_scheduling_algorithms(){
    list_destroy_and_destroy_elements(scheduling_algorithms, free);
}

t_scheduling_algorithm* chosen_scheduling_algorithm(){
    scheduling_algorithm_name = config_get_string_at("ALGORITMO_PLANIFICACION");

    initialize_scheduling_algorithms();

    t_scheduling_algorithm* scheduling_algorithm_found =
            list_remove_by_condition(scheduling_algorithms, (bool (*)(void *)) can_handle);

    if(!scheduling_algorithm_found){
        log_scheduling_algorithm_not_found_error_for(scheduling_algorithm_name);
        free_system();
    }

    free_scheduling_algorithms();

    return scheduling_algorithm_found;
}