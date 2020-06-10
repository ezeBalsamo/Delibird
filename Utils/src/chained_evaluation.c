#include "../include/chained_evaluation.h"
#include <stdlib.h>
#include <general_logs.h>

void chained_on_succesful_execution_function(t_chained_on_succesful_evaluation* chained_on_succesful_evaluation, void* object_to_evaluate){

    bool execution_result = chained_on_succesful_evaluation -> satisfy_function (object_to_evaluate);

    if(execution_result){
        execute_evaluation_for(chained_on_succesful_evaluation -> next_evaluation, object_to_evaluate);
    }else{
        chained_on_succesful_evaluation -> failure_function (object_to_evaluate);
    }
}

void chained_on_failure_execution_function(t_chained_on_failure_evaluation* chained_on_failure_evaluation, void* object_to_evaluate){

    bool execution_result = chained_on_failure_evaluation -> satisfy_function (object_to_evaluate);

    if(execution_result){
        chained_on_failure_evaluation -> success_function (object_to_evaluate);
    }else{
        execute_evaluation_for(chained_on_failure_evaluation -> next_evaluation, object_to_evaluate);
    }
}

void basic_execution_function(t_basic_evaluation* basic_evaluation, void* object_to_evaluate){

    bool execution_result = basic_evaluation -> satisfy_function (object_to_evaluate);

    if(execution_result){
        basic_evaluation -> success_function (object_to_evaluate);
    }else{
        basic_evaluation -> failure_function (object_to_evaluate);
    }
}

void execute_evaluation_for(t_identified_chained_evaluation* identified_chained_evaluation, void* object_to_evaluate){

    uint32_t evaluation_type = identified_chained_evaluation -> chained_evaluation_type;
    void* evaluation = identified_chained_evaluation -> evaluation;

    switch (evaluation_type) {
        case CHAINED_ON_SUCCESFUL:
            chained_on_succesful_execution_function(evaluation, object_to_evaluate);
            break;
        case CHAINED_ON_FAILURE:
            chained_on_failure_execution_function(evaluation, object_to_evaluate);
            break;
        case BASIC:
            basic_execution_function(evaluation, object_to_evaluate);
            break;
        default:
            log_unknown_chained_evaluation_type_error();
    }
}

void free_chained_on_failure_evaluation(t_chained_on_failure_evaluation* chained_on_failure_evaluation){
    free_chained_evaluation(chained_on_failure_evaluation -> next_evaluation);
    free(chained_on_failure_evaluation);
}

void free_chained_on_succesful_evaluation(t_chained_on_succesful_evaluation* chained_on_succesful_evaluation){
    free_chained_evaluation(chained_on_succesful_evaluation -> next_evaluation);
    free(chained_on_succesful_evaluation);
}

void free_basic_evaluation(t_basic_evaluation* basic_evaluation){
    free(basic_evaluation);
}

void free_chained_evaluation(t_identified_chained_evaluation* identified_chained_evaluation){

    uint32_t evaluation_type = identified_chained_evaluation -> chained_evaluation_type;
    void* evaluation = identified_chained_evaluation -> evaluation;

    switch (evaluation_type) {
        case CHAINED_ON_SUCCESFUL:
            free_chained_on_succesful_evaluation(evaluation);
            break;
        case CHAINED_ON_FAILURE:
            free_chained_on_failure_evaluation(evaluation);
            break;
        case BASIC:
            free_basic_evaluation(evaluation);
            break;
        default:
            log_unknown_chained_evaluation_type_error();
    }

    free(identified_chained_evaluation);
}