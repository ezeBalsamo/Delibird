#include "../include/team_logs_manager.h"
#include "../../Utils/include/logger.h"
#include "../../Utils/include/garbage_collector.h"
#include "../../Utils/include/queue_code_name_associations.h"
#include "../../Utils/include/pretty_printer.h"
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

void log_trainer_thread_context_not_found_error_for(uint32_t sequential_number){
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

    char* printable_localizable_pokemon = localizable_pokemon_as_string(localizable_pokemon);
    char* message = string_from_format(
            "No se ha encontrado cargado en las ocurrencias al pokemon %s.",
            printable_localizable_pokemon);

    log_errorful_message(process_execution_logger(), message);
    free(printable_localizable_pokemon);
    free(message);
}

void log_trainer_dispatch_action_with_reason(t_localizable_object* localizable_trainer, char* action_name, char* state_structure_name, char* reason){

    char* printable_localizable_trainer = localizable_trainer_as_string(localizable_trainer);
    char* final_message;
    char* message =
            string_from_format("El %s fue %s a la %s.",
                    printable_localizable_trainer,
                    action_name,
                    state_structure_name);

    if(reason != NULL){
        final_message = string_from_format("%s Motivo: %s", message, reason);
        free(reason);
    }else{
        final_message = string_duplicate(message);
    }

    free(printable_localizable_trainer);
    free(message);

    log_succesful_message(main_logger(), final_message);
    log_succesful_message(process_execution_logger(), final_message);
    free(final_message);
}

void log_trainer_dispatch_action(t_localizable_object* localizable_trainer, char* action_name, char* state_structure_name){
    log_trainer_dispatch_action_with_reason(localizable_trainer, action_name, state_structure_name, NULL);
}

void log_trainer_added_to_new(t_localizable_object* localizable_trainer){
    log_trainer_dispatch_action(localizable_trainer, "agregado", state_as_string(NEW));
}

void log_trainer_schedule(t_localizable_object* localizable_trainer, char* reason){
    log_trainer_dispatch_action_with_reason(localizable_trainer, "movido", state_as_string(READY), reason);
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
    log_trainer_dispatch_action_with_reason(localizable_trainer, "movido", state_as_string(EXECUTE), reason);
}

void log_trainer_blocked(t_trainer_thread_context* trainer_thread_context){
    t_localizable_object* localizable_trainer = trainer_thread_context -> localizable_trainer;
    char* reason = thread_action_reason_for(trainer_thread_context);

    log_trainer_dispatch_action_with_reason(localizable_trainer, "movido", state_as_string(BLOCKED), reason);
}

void log_trainer_has_accomplished_own_goal(t_localizable_object* localizable_trainer){
    char* reason = string_new();
    string_append(&reason, "Atrapó todos los pokemones que requería.");

    log_trainer_dispatch_action_with_reason(localizable_trainer, "movido", state_as_string(FINISHED), reason);
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

void log_expected_global_goal_to_be_accomplished_error(){
    char* message = string_from_format("Se esperaba que el objetivo global se haya cumplido.");
    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void log_message_id_not_required(uint32_t queue_code, uint32_t message_id){
    char* message =
            string_from_format("Ningún entrenador está esperando la respuesta de un %s con id de mensaje %d",
                    queue_name_of(queue_code), message_id);

    log_succesful_message(main_logger(), message);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_appeared_pokemon_not_necessary_for_global_goal(char* pokemon_name){
    char* message = string_from_format("El pokemon %s no es requerido para completar el objetivo global.", pokemon_name);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_global_goal_not_consistent_with_trainers_requirements_error(){
    log_errorful_message(process_execution_logger(), "Los requerimientos globales no coinciden con los requerimientos de los entrenadores.");
}

void log_invalid_transition_error(){
    log_errorful_message(process_execution_logger(), "Se intentó hacer una transición de estados inválida.");
}

void log_invalid_state_error(){
    log_errorful_message(process_execution_logger(), "La cola es inválida.");
}

void log_more_than_one_trainer_thread_context_executing_error_for(t_list* trainer_thread_contexts){

    char* message = string_from_format("%s", "Se esperaba encontrar un único hilo de entrenador ejecutado. Se encontraron:\n");

    void _add_printable_trainers(t_trainer_thread_context* trainer_thread_context){
        char* printable_localizable_trainer = localizable_trainer_as_string(trainer_thread_context -> localizable_trainer);
        char* action_to_perform = thread_action_reason_for(trainer_thread_context);

        string_append(&message, printable_localizable_trainer);
        string_append(&message, "- Acción: ");
        string_append(&message, action_to_perform);
        string_append(&message, "\n");

        free(printable_localizable_trainer);
        free(action_to_perform);
    }

    list_iterate(trainer_thread_contexts, (void (*)(void *)) _add_printable_trainers);
    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void log_not_matching_pokemon_name_between_get_and_localized_error(char* get_response_pokemon_name, char* localized_pokemon_name){
    char* message =
            string_from_format("No coinciden los pokemones para las operaciones correlativas get y localized. GET -> %s, LOCALIZED -> %s\n",
                                get_response_pokemon_name,
                                localized_pokemon_name);

    log_errorful_message(process_execution_logger(), message);
    free(message);
}

void log_message_ignored_due_to_previous_existing_occurrences_for(t_localized_pokemon* localized_pokemon){
    char* printable_localized_pokemon = pretty_print_of(LOCALIZED_POKEMON, localized_pokemon);
    char* message = string_from_format("Se procederá a ignorar %s debido a ocurrencias previas del pokemon\n", printable_localized_pokemon);

    log_succesful_message(main_logger(), message);
    log_succesful_message(process_execution_logger(), message);
    free(printable_localized_pokemon);
    free(message);
}

void log_no_suitable_exchange_inferrer_found_error_for(t_trainer_thread_context* trainer_thread_context, t_trainer_thread_context* another_trainer_thread_context){
    char* printable_trainer = localizable_trainer_as_string(trainer_thread_context -> localizable_trainer);
    char* printable_another_trainer = localizable_trainer_as_string(another_trainer_thread_context -> localizable_trainer);

    char* message =
            string_from_format("No se ha encontrado un inferidor de intercambio válido entre %s y %s",
                    printable_trainer, printable_another_trainer);

    log_errorful_message(process_execution_logger(), message);
    free(printable_trainer);
    free(printable_another_trainer);
    free(message);
}

void log_invalid_localizable_object_type_error(){
    log_errorful_message(process_execution_logger(), "Se intentó imprimir un objeto localizable inválido.");
}

void log_deadlock_detection_algorithm_has_begun(){
    char* message = "El algoritmo de detección de Deadlock ha comenzado.";
    log_succesful_message(main_logger(), message);
    log_succesful_message(process_execution_logger(), message);
}

void log_deadlock_detection_algorithm_has_finished_explaning_that(char* explanation){
    char* message = string_from_format("El algoritmo de detección de Deadlock ha finalizado. Resultado: %s", explanation);
    log_succesful_message(main_logger(), message);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_deadlock_detection_algorithm_has_finished_with_deadlock_detected(){
    log_deadlock_detection_algorithm_has_finished_explaning_that("El sistema se encuentra en una situación de interbloqueo. Se procederá a resolverlo.");
}

void log_deadlock_detection_algorithm_has_finished_with_no_deadlock_detected(){
    log_deadlock_detection_algorithm_has_finished_explaning_that("El sistema puede continuar operando.");
}

void log_deadlock_solver_algorithm_has_begun_for(t_list* trainer_thread_contexts){
    char* message = string_from_format("%s", "El algoritmo de recuperación de Deadlock ha comenzado.\nEntrenadores involucrados:\n");

    for(int i = 0; i < list_size(trainer_thread_contexts); i++) {
        t_trainer_thread_context* trainer_thread_context = list_get(trainer_thread_contexts, i);
        t_localizable_object* localizable_trainer = trainer_thread_context -> localizable_trainer;
        char* printable_localizable_trainer = localizable_trainer_as_string(localizable_trainer);

        string_append(&message, "* ");
        string_append(&message, printable_localizable_trainer);

        if(i + 1 != list_size(trainer_thread_contexts)){
            string_append(&message, "\n");
        }

        free(printable_localizable_trainer);
    }
    log_succesful_message(main_logger(), message);
    log_succesful_message(process_execution_logger(), message);
    free(message);
}

void log_exchange_action_with(t_identified_exchange* identified_exchange, char* summary, char* concrete_action){

    t_trainer* first_party_trainer = first_party_localizable_trainer_in(identified_exchange) -> object;
    char* printable_first_party_trainer = trainer_as_string(first_party_trainer);
    char* first_party_pokemon_name = first_party_pokemon_name_in(identified_exchange);

    t_trainer* second_party_trainer = second_party_localizable_trainer_in(identified_exchange) -> object;
    char* printable_second_party_trainer = trainer_as_string(second_party_trainer);
    char* second_party_pokemon_name = second_party_pokemon_name_in(identified_exchange);

    char* message =
            string_from_format(
                    "%s: %s %s su %s con el %s de %s.",
                    summary,
                    printable_first_party_trainer,
                    concrete_action,
                    first_party_pokemon_name,
                    second_party_pokemon_name,
                    printable_second_party_trainer);

    log_succesful_message(main_logger(), message);
    log_succesful_message(process_execution_logger(), message);

    free(printable_first_party_trainer);
    free(printable_second_party_trainer);
    free(message);
}

void log_exchange_to_realize_according_to(t_identified_exchange* identified_exchange){
    log_exchange_action_with(identified_exchange, "A punto de realizar un intercambio", "intercambiará");
}

void log_exchange_realized_according_to(t_identified_exchange* identified_exchange){
    log_exchange_action_with(identified_exchange, "Intercambio realizado exitosamente", "intercambió");
}

void log_global_goal_accomplished(){
    char* message = "Team ha cumplido su objetivo global.";
    log_succesful_message(main_logger(), message);
    log_succesful_message(process_execution_logger(), message);
}

void free_team_logs_manager(){
    free_loggers();
}