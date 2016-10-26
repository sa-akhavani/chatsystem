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

//free
void free_user_map(struct user * first) {
    struct user * next = first->next;
    free(first);

    if(next != NULL)
        free_user_map(next);
}

//test & print
void print_user_map(struct user * first){
    printf("%d\n", first->port);
    if(first->next != NULL)
        print_user_map(first->next);
}