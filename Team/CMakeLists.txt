include_directories(include)
set(SOURCES
        src/team_broker_connection_handler.c
        src/team_gameboy_connection_handler.c
        src/pokemon_occurrences.c
        src/team.c
        src/team_logs_manager.c
        src/team_configuration_manager.c
        src/team_manager.c
        src/team_serializable_objects.c
        src/trainers_parser.c
        src/goal_calculator.c
        src/team_query_performers.c
        src/appeared_query_performer.c
        src/localized_query_performer.c
        src/caught_query_performer.c
        src/team_sanitizer.c
        src/pokemon_occurrence_trigger.c
        src/dispatcher.c
        src/trainer_threads.c
        src/scheduling_algorithm.c
        src/scheduling_algorithm_finder.c
        src/fifo_scheduling_algorithm.c
        src/round_robin_scheduling_algorithm.c
        src/preemptive_sjf_scheduling_algorithm.c
        src/non_preemptive_sjf_scheduling_algorithm.c
        src/movement_action.c
        src/trainer_thread_context_execution_cycle.c
        src/catch_action.c
        src/distance_calculator.c
        src/trainer_thread_context_state_chained_evaluation.c
        src/dispatching_reasons.c
        src/waiting_actions.c
        src/team_pretty_prints.c
        src/state_transitions.c
        src/dispatcher_queues.c
        src/main_threads_executor.c
        src/deadlock_detection_and_recovery_algorithm.c
        src/deadlock_detecter.c
        src/deadlock_solver.c
        src/exchanges_inferrer.c
        src/benefitial_exchanges_for_both_parties.c
        src/benefitial_exchanges_only_for_first_party.c
        src/benefitial_exchanges_only_for_second_party.c
        src/non_benefitial_exchange.c
        src/exchange_provider.c)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/target")
add_executable(team ${SOURCES})

target_link_libraries(team utils)