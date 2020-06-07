#include "../include/team_logs_manager.h"
#include "../../Utils/include/logger.h"
#include "../../Utils/include/free_system.h"
#include <commons/string.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <team_manager.h>
#include <trainer_threads.h>
#include <dispatching_reasons.h>

void initialize_team_logs_manager(){
    initialize_logger_for("Team");
    create_main_logger();
    create_process_execution_logger();
}

void log_failed_attempt_to_communicate_with_broker(){
    char* message = "Falló la comunicación con Broker. Se procederá a reintentar.";
    log_errorful_message(main_logger(), message);
    log_errorful_message(process_execution_logger(), message);
}

void log_initiating_communication_retry_process_with_broker(){
    char* message = "Inicio del proceso de reintento de comunicación con el Broker.";
    log_succesful_message(main_logger(), message);
    log_succesful_message(process_execution_logger(), message);
}

void log_succesful_retry_of_communication_with_broker(){
    char* message = "El proceso de reintento de comunicación con el Broker ha sido exitoso.";
    log_succesful_message(main_logger(), message);
    log_succesful_message(process_execution_logger(), message);
}

void log_failed_retry_of_communication_with_broker(){
    char* message = "Falló el reintento de comunicación con el Broker.";
    log_errorful_message(main_logger(), message);
    log_errorful_message(process_execution_logger(), message);
}

void log_thread_sleep_time_configuration_error(){
    char* message = string_new();
    string_append(&message, "Se produjo un error al intentar dormir el hilo: ");
    string_append(&message, strerror(errno));

    log_errorful_message(main_logger(), message);
    log_errorful_message(process_execution_logger(), message);

    free(message);
}

void log_queue_thread_create_error(){
    char* message = string_new();
    string_append(&message, "Falló la creación del hilo para escuchar una cola del Broker: ");
    string_append(&message, strerror(errno));

    log_errorful_message(process_execution_logger(), message);

    free(message);
    free_system();
}

void log_no_locations_found_for(char* pokemon_name){
    char* message = string_from_format("No se encontraron ubicaciones para %s\n", pokemon_name);

    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_trainer_thread_create_error(){
    log_errorful_message(process_execution_logger(), "Falló la creación de un hilo para un entrenador.");
    free_system();
}

void log_succesful_creation_of_thread_of_trainer(uint32_t sequential_number){
    char* message = string_from_format("Se creó exitosamente el hilo para el entrenador %u", sequential_number);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_query_performer_not_found_error_for(uint32_t operation){
    char* message = string_from_format("No se ha encontrado un query performer que maneje operaciones de código %u", operation);
    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void log_no_schedulable_threads_available_for(char* pokemon_name){
    char* message = string_from_format("No se encuentran disponibles hilos planificables para capturar %s", pokemon_name);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_synchronizable_trainer_not_found_error_for(uint32_t sequential_number){
    char* message = string_from_format("Se esperaba encontrar al entrenador %u en las colas de new o blocked.", sequential_number);
    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void log_scheduling_algorithm_not_found_error_for(char* scheduling_algorithm_name){
    char* message = string_from_format("No se ha implementado un algoritmo de planificación llamado %s", scheduling_algorithm_name);
    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void log_pokemon_not_belonging_to_global_goal_error_for(char* pokemon_name){
    char* message = string_from_format("El pokemon %s no forma parte del objetivo global.", pokemon_name);
    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void log_incorrent_pokemon_removed_error_for(char* pokemon_name_to_remove, char* pokemon_name_removed){
    char* message = string_from_format("Se esperaba remover %s pero en su lugar se encontraba %s",
            pokemon_name_to_remove, pokemon_name_removed);
    log_errorful_message(process_execution_logger(), message);
    free(message);
}


void log_trainer_dispatch_action_with_reason(t_localizable_object* localizable_trainer, char* action_name, char* state_structure_name, char* reason){
    t_trainer* trainer = localizable_trainer -> object;
    char* final_message;
    char* message =
            string_from_format("El entrenador %d, ubicado en (%d, %d) fue %s a la %s.",
                    trainer -> sequential_number,
                    localizable_trainer -> pos_x,
                    localizable_trainer -> pos_y,
                    action_name,
                    state_structure_name);

    if(reason != NULL){
        final_message = string_from_format("%s Motivo: %s", message, reason);
        free(reason);
    }else{
        final_message = string_duplicate(message);
    }

    free(message);

    log_succesful_message(main_logger(), final_message);
    log_succesful_message(process_execution_logger(), final_message);
    free(final_message);
}

void log_trainer_dispatch_action(t_localizable_object* localizable_trainer, char* action_name, char* state_structure_name){
    log_trainer_dispatch_action_with_reason(localizable_trainer, action_name, state_structure_name, NULL);
}

void log_trainer_added_to_new(t_localizable_object* localizable_trainer){
    log_trainer_dispatch_action(localizable_trainer, "agregado", "lista de nuevos");
}

void log_trainer_schedule(t_localizable_object* localizable_trainer, char* reason){
    log_trainer_dispatch_action_with_reason(localizable_trainer, "movido", "cola de listos", reason);
}

void log_trainer_movement(t_localizable_object* localizable_trainer){
    t_trainer* trainer = localizable_trainer -> object;
    char* message =
            string_from_format("El entrenador %d se desplazó a (%d, %d).",
                    trainer -> sequential_number,
                    localizable_trainer -> pos_x,
                    localizable_trainer -> pos_y);

    log_succesful_message(main_logger(), message);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_trainer_execution(t_localizable_object* localizable_trainer, char* reason){
    log_trainer_dispatch_action_with_reason(localizable_trainer, "movido", "ejecución", reason);
}

void log_trainer_has_accomplished_own_goal(t_localizable_object* localizable_trainer){
    char* reason = string_new();
    string_append(&reason, "Atrapó todos los pokemones que requería");

    log_trainer_dispatch_action_with_reason(localizable_trainer, "movido", "lista de finalizados", reason);
}

void log_unknown_thread_action_type_error(){
    log_errorful_message(main_logger(), "Se ha configurado una thread_action con un tipo inválido.");
}

void log_thread_action_to_perform_by(t_trainer_thread_context* trainer_thread_context){
    t_trainer* trainer = trainer_thread_context -> localizable_trainer -> object;
    char* action_to_perform = thread_action_as_string(trainer_thread_context);
    char* message =
            string_from_format("Acción a realizar por el entrenador %d: %s",
                    trainer -> sequential_number,
                    action_to_perform);

    log_succesful_message(main_logger(), message);
    log_succesful_message(process_execution_logger(), message);
    free(action_to_perform);
    free(message);

}

void free_team_logs_manager(){
    free_loggers();
}