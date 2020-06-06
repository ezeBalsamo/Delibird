#include "../include/chained_evaluation.h"

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

    if(evaluation_type == CHAINED_ON_SUCCESFUL){
        chained_on_succesful_execution_function(evaluation, object_to_evaluate);
    }
    if(evaluation_type == CHAINED_ON_FAILURE){
        chained_on_failure_execution_function(evaluation, object_to_evaluate);
    }
    if(evaluation_type == BASIC){
        basic_execution_function(evaluation, object_to_evaluate);
    }
}