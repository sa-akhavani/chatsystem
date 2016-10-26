#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MYPORT "4400"    // the port users will be connecting to
#define MAXBUFLEN 100


void *get_in_addr(struct sockaddr *sa);
bool check_input(int argc, char *argv[]);

#endif