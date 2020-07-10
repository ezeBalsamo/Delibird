#include <event_notifier.h>
#include <team_manager.h>
#include <stdlib.h>
#include "../../Utils/include/common_structures.h"

t_list* subscriptions;

void initialize_event_notifier(){
    subscriptions = list_create();
}

void subscribe_to_event_doing(int event_type, void (*callback_function) (void* event_argument)){
    t_subscription* subscription = safe_malloc(sizeof(t_subscription));
    subscription -> event_type = event_type;
    subscription -> callback_function = callback_function;

    list_add(subscriptions, subscription);
}

t_list* subscriptions_to(int event_type){

    bool _is_for(t_subscription* subscription){
        return subscription -> event_type == event_type;
    }

    return list_filter(subscriptions, (bool (*)(void *)) _is_for);
}

void perform_subscriptions_callback_with_argument(int event_type, void* event_argument){
    t_list* filtered_subscriptions = subscriptions_to(event_type);

    void _perform_callback_with_argument(t_subscription* subscription){
        subscription -> callback_function(event_argument);
    }

    list_iterate(filtered_subscriptions, (void (*)(void *)) _perform_callback_with_argument);
    list_destroy(filtered_subscriptions);
}

void notify(int event_type){
    perform_subscriptions_callback_with_argument(event_type, NULL);
}

void notify_with_argument(int event_type, void* event_argument){
    perform_subscriptions_callback_with_argument(event_type, event_argument);
}

void free_event_notifier(){
    list_destroy_and_destroy_elements(subscriptions, free);
}