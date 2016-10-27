#include "linkedlist.h"


// linked List

// Add User
void add_user(struct user* head, char* username, char* ip, char* port)
{
    struct user* new_user = (struct user *) malloc(sizeof (struct user));
    new_user->username = username;
    new_user->ip;
    new_user->port;

    // printf("uname: %s\n", new_user->username);

    struct user * current = head;
    while (current->next != NULL) {
        current = current->next;
        // printf("current: %s\n", current->username);
    }

    current->next = new_user;
    current->next->next = NULL;
}

// // Delete User
// void remove_by_index(struct user ** head, int n) {
//     int i = 0;
//     struct user * current = *head;
//     struct user * temp_node = NULL;

//     if (n == 0) {
//         pop(head);
//         return;
//     }

//     for (int i = 0; i < n-1; i++) {
//         if (current->next == NULL) {
//             return;
//         }
//         current = current->next;
//     }

//     temp_node = current->next;
//     retval = temp_node->val;
//     current->next = temp_node->next;
//     free(temp_node);
// }

// Find User
int find_user(struct user* head, char* username)
{
    int idx = 0;
    struct user * current = head;
    while (current != NULL) {
        if (strcmp(current->username, username) == 0) {
            // FOUND
            return idx;
        }
        idx++;
        current = current->next;
    }
    printf("user not found\n");
    return -1;
}

// Clear All
void clear_all(struct user* head)
{
    struct user * next = head->next;
    free(head);
    if(next != NULL)
        clear_all(next);
}

// print all nodes
void print_all(struct user* head)
{
    if (head == NULL)
        return;
    printf("user: %s\n", head->username);
    printf("ip: %s\n", head->ip);
    printf("port: %s\n", head->port);
    if(head->next != NULL)
        print_all(head->next);
}
