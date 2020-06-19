#include "../include/team_logs_manager.h"
#include "../../Utils/include/logger.h"
#include "../../Utils/include/garbage_collector.h"
#include <commons/string.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <team_manager.h>
#include <trainer_threads.h>
#include <dispatching_reasons.h>
#include <team_pretty_prints.h>

void initialize_team_logs_manager(){
    initialize_logger_for("Team");
    create_main_logger();
    create_process_execution_logger();
}

void log_failed_attempt_to_communicate_with_broker(char* default_action){
    char* message = string_from_format("Falló la comunicación con Broker. Por defecto, %s.", default_action);
    log_warning_message(main_logger(), message);
    log_warning_message(process_execution_logger(), message);
    free(message);
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
    char* default_action = string_from_format("No se encontraron ubicaciones para %s\n", pokemon_name);
    log_failed_attempt_to_communicate_with_broker(default_action);
    free(default_action);
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

void log_targetable_pokemon_not_found_error_for(t_localizable_object* localizable_pokemon){

    char* message = string_from_format(
            "No se ha encontrado cargado en las ocurrencias al pokemon %s, ubicado en (%d, %d).",
            localizable_pokemon -> object,
            localizable_pokemon -> pos_x,
            localizable_pokemon -> pos_y);

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

void log_trainer_blocked(t_trainer_thread_context* trainer_thread_context){
    t_localizable_object* localizable_trainer = trainer_thread_context -> localizable_trainer;
    char* reason = thread_action_reason_for(trainer_thread_context);

    log_trainer_dispatch_action_with_reason(localizable_trainer, "movido", "cola de bloqueados", reason);
}

void log_trainer_has_accomplished_own_goal(t_localizable_object* localizable_trainer){
    char* reason = string_new();
    string_append(&reason, "Atrapó todos los pokemones que requería.");

    log_trainer_dispatch_action_with_reason(localizable_trainer, "movido", "lista de finalizados", reason);
}

void log_unknown_thread_action_type_error(){
    log_errorful_message(main_logger(), "Se ha configurado una thread_action con un tipo inválido.");
}

void log_thread_action_to_perform_by(t_trainer_thread_context* trainer_thread_context){
    t_trainer* trainer = trainer_thread_context -> localizable_trainer -> object;
    char* action_to_perform = thread_action_reason_for(trainer_thread_context);
    char* message =
            string_from_format("Acción a realizar por el entrenador %d: %s",
                    trainer -> sequential_number,
                    action_to_perform);

    log_succesful_message(main_logger(), message);
    log_succesful_message(process_execution_logger(), message);
    free(action_to_perform);
    free(message);

}

void log_failed_caught_of(t_localizable_object* localizable_pokemon){
    char* localizable_pokemon_string = localizable_pokemon_as_string(localizable_pokemon);
    char* message = string_from_format("Se denegó la captura de %s", localizable_pokemon_string);

    log_succesful_message(main_logger(), message);
    log_succesful_message(process_execution_logger(), message);

    free(localizable_pokemon_string);
    free(message);
}

char* succesfully_caught_description_for(t_localizable_object* localizable_pokemon){

    char* localizable_pokemon_string = localizable_pokemon_as_string(localizable_pokemon);
    char* message = string_from_format("%s, fue atrapado exitosamente", localizable_pokemon_string);

    free(localizable_pokemon_string);
    return message;
}

void log_succesfully_caught(t_localizable_object* localizable_pokemon){

    char* message = succesfully_caught_description_for(localizable_pokemon);
    log_succesful_message(main_logger(), message);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_succesfully_caught_due_to_failed_communication_with_broker(t_localizable_object* localizable_pokemon){
    char* default_action = succesfully_caught_description_for(localizable_pokemon);
    log_failed_attempt_to_communicate_with_broker(default_action);
    free(default_action);
}

void log_expected_to_be_empty_error_for(char* state_structure_name){
    char* message = string_from_format("Se esperaba que la %s estuviera vacía.", state_structure_name);
    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void log_expected_to_be_not_empty_error_for(char* state_structure_name){
    char* message = string_from_format("Se esperaba que la %s no estuviera vacía.", state_structure_name);
    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void log_expected_no_trainer_thread_executing_error_for(t_trainer_thread_context* trainer_thread_context){
    char* action_to_perform = thread_action_reason_for(trainer_thread_context);
    char* message = string_from_format("Se esperaba que ningún hilo estuviese ejecutando.\n"
                                       "Información del hilo actualmente ejecutando: %s", action_to_perform);

    log_errorful_message(process_execution_logger(), message);
    free(action_to_perform);
    free(message);
}

void log_message_id_not_required(uint32_t message_id){
    char* message = string_from_format("Ningún entrenador está esperando la respuesta de un CAUGHT_POKEMON con id de mensaje %d", message_id);
    log_succesful_message(main_logger(), message);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_not_matching_trainers_amount_with_finished_thread_contexts_amount_on_global_goal_accomplished_error(){
    char* message = "Se determinó que se cumplió el objetivo global "
                    "pero la cantidad de entrenadores y de hilos finalizados no coinciden.";

    log_errorful_message(process_execution_logger(), message);
}

void log_global_goal_accomplished(){
    char* message = "Team ha cumplido su objetivo global.";
    log_succesful_message(main_logger(), message);
    log_succesful_message(process_execution_logger(), message);
}

void free_team_logs_manager(){
    free_loggers();
}