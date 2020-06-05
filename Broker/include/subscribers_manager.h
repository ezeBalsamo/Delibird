#ifndef DELIBIRD_SUBSCRIBERS_MANAGER_H
#define DELIBIRD_SUBSCRIBERS_MANAGER_H

void initialize_subscribers_manager();

t_list* get_subscribers_of_a_queue(uint32_t queue);
void subscribe_process(int subscriber,uint32_t operation_queue);
bool equal_subscribers(int subscriber, int another_subscriber);
void move_subscriber_to_ACK(t_message_status* message_status, int subscriber);
void send_all_messages(int subscriber, uint32_t operation_queue);

#endif //DELIBIRD_SUBSCRIBERS_MANAGER_H
