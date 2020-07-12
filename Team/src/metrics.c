#include <team_manager.h>
#include <commons/string.h>
#include <team_pretty_prints.h>
#include <stdlib.h>
#include <team_logs_manager.h>
#include <event_notifier.h>
#include "metrics.h"
#include "../../Utils/include/common_structures.h"
#include "../../Utils/include/garbage_collector.h"

int total_execution_cycles_quantity;
int context_switches_quantity;
t_list* execution_cycles_quantity_per_trainers;
int produced_and_resolved_deadlocks_quantity;
t_list* trades_made_for_deadlocks_resolution;

void initialize_execution_cycles_of(t_localizable_object* localizable_trainer){
    t_execution_cycles_quantity_per_trainer* execution_cycles_quantity_per_trainer = safe_malloc(sizeof(t_execution_cycles_quantity_per_trainer));

    execution_cycles_quantity_per_trainer -> trainer = localizable_trainer -> object;
    execution_cycles_quantity_per_trainer -> execution_cycles_quantity = 0;

    list_add(execution_cycles_quantity_per_trainers, execution_cycles_quantity_per_trainer);
}

void increment_total_execution_cycles_quantity(){
    total_execution_cycles_quantity++;
}

void increment_execution_cycles_per_trainers_quantity(t_trainer* trainer){

    bool _is_for(t_execution_cycles_quantity_per_trainer* execution_cycles_quantity_per_trainer){
        return are_equal_trainers(execution_cycles_quantity_per_trainer -> trainer, trainer);
    }

    t_execution_cycles_quantity_per_trainer* execution_cycles_quantity_per_trainer_found =
            list_find(execution_cycles_quantity_per_trainers, (bool (*)(void *)) _is_for);

    if(!execution_cycles_quantity_per_trainer_found){
        log_no_trainer_found_to_increment_its_execution_cycles_quantity_error_for(trainer);
        free_system();
    }

    execution_cycles_quantity_per_trainer_found -> execution_cycles_quantity++;
    increment_total_execution_cycles_quantity();
}

void increment_context_switches_quantity(){
    context_switches_quantity++;
}

void increment_produced_and_solved_deadlocks_quantity(){
    produced_and_resolved_deadlocks_quantity++;
}

void save_printable_trade_completed(char* printable_trade_completed){
    list_add(trades_made_for_deadlocks_resolution, printable_trade_completed);
}

static void subscribe_to_events(){
    subscribe_to_event_doing(EXECUTION_CYCLE_CONSUMED, increment_total_execution_cycles_quantity);

    subscribe_to_event_doing(EXECUTION_CYCLE_CONSUMED_BY_TRAINER,
                             (void (*)(void *)) increment_execution_cycles_per_trainers_quantity);

    subscribe_to_event_doing(CONTEXT_SWITCH_REALIZED, increment_context_switches_quantity);

    subscribe_to_event_doing(PRODUCED_AND_SOLVED_DEADLOCK,
                            (void (*)(void *)) increment_produced_and_solved_deadlocks_quantity);

    subscribe_to_event_doing(TRADE_COMPLETED, (void (*)(void *)) save_printable_trade_completed);
}

void initialize_metrics(){
    total_execution_cycles_quantity = 0;
    context_switches_quantity = 0;
    execution_cycles_quantity_per_trainers = list_create();
    produced_and_resolved_deadlocks_quantity = 0;
    trades_made_for_deadlocks_resolution = list_create();

    with_trainers_do(initialize_execution_cycles_of);

    subscribe_to_events();
}

char* metrics_report_line_for(char* metric_name, int metric_value){
    return string_from_format("* %s: %d\n", metric_name, metric_value);
}

char* execution_cycles_quantity_per_trainers_metrics_report_block(){
    char* metric_name = string_from_format("* %s\n", "Cantidad de ciclos de CPU realizados por entrenador");

    void _load_metric_of(t_execution_cycles_quantity_per_trainer* execution_cycles_quantity_per_trainer){
        char* printable_trainer = trainer_as_string(execution_cycles_quantity_per_trainer -> trainer);

        char* trainer_metric_report_line =
                metrics_report_line_for(printable_trainer,
                                        execution_cycles_quantity_per_trainer -> execution_cycles_quantity);

        char* indented_trainer_metric_report_line = string_from_format("\t%s", trainer_metric_report_line);

        string_append(&metric_name, indented_trainer_metric_report_line);

        free(printable_trainer);
        free(trainer_metric_report_line);
        free(indented_trainer_metric_report_line);
    }

    list_iterate(execution_cycles_quantity_per_trainers, (void (*)(void *)) _load_metric_of);
    return metric_name;
}

char* produced_and_resolved_deadlocks_metrics_report_block(){
    char* metric_report_block =
            metrics_report_line_for("Deadlocks producidos y resueltos",
                                    produced_and_resolved_deadlocks_quantity);

    void _load_metric_of(char* printable_trade_completed){
        char* indented_printable_trade_completed =
                string_from_format("\t* %s\n", printable_trade_completed);

        string_append(&metric_report_block, indented_printable_trade_completed);
        free(indented_printable_trade_completed);
    }

    list_iterate(trades_made_for_deadlocks_resolution, (void (*)(void *)) _load_metric_of);
    return metric_report_block;
}

void dump_metrics(){
    char* total_execution_cycles_quantity_metrics_report_line =
            metrics_report_line_for("Cantidad de ciclos de CPU totales", total_execution_cycles_quantity);

    char* context_switches_quantity_metrics_report_line =
            metrics_report_line_for("Cantidad de cambios de contexto realizados", context_switches_quantity);

    char* execution_cycles_quantity_per_trainers_metrics = execution_cycles_quantity_per_trainers_metrics_report_block();

    char* produced_and_resolved_deadlocks_metrics = produced_and_resolved_deadlocks_metrics_report_block();

    char* metrics_report =
            string_from_format("\nREPORTE DE MÉTRICAS\n%s%s%s%s",
                    total_execution_cycles_quantity_metrics_report_line,
                    context_switches_quantity_metrics_report_line,
                    execution_cycles_quantity_per_trainers_metrics,
                    produced_and_resolved_deadlocks_metrics);

    free(total_execution_cycles_quantity_metrics_report_line);
    free(context_switches_quantity_metrics_report_line);
    free(execution_cycles_quantity_per_trainers_metrics);
    free(produced_and_resolved_deadlocks_metrics);

    log_metrics_report(metrics_report);
}

void free_metrics(){
    list_destroy_and_destroy_elements(execution_cycles_quantity_per_trainers, free);
    list_destroy_and_destroy_elements(trades_made_for_deadlocks_resolution, free);
}