//struct

struct user {
               char             ip[14];
               int              port;
               char             name[40];
               struct user      *next;
           };


//allocation & initializing
	  struct user * first_user = (struct user *) malloc(sizeof (struct user));
    struct user * second_user = (struct user *) malloc(sizeof (struct user));
    struct user * third_user = (struct user *) malloc(sizeof (struct user));
    first_user->port = 11;
    first_user->next = second_user;
    second_user->port = 12;
    second_user->next = third_user;
    third_user->port = 13;
    print_user_map(first_user);
    free_user_map(first_user);


// linked List

// Add User
void add_user(struct user* head, struct user * new_user)
{
    struct user * current = head;
    while (current->next != NULL) {
        current = current->next;
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
    printf("port: %d\n", head->port);
    if(head->next != NULL)
        print_all(head->next);
}
