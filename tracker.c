#include "tracker.h"

// Functions

int handle_request(char * buffer, int size)	// output: login -> 1 | exit -> -1 | send message -> 0
{
	// int idx = 0;
	// int new_idx = 0;

	// char command[size];
	// char username[size];
	// char message[size];

	// while(buffer[idx] == ' ')
	// 	idx++;
	// while(buffer[idx] != ' ') {
	// 	command[new_idx] = buffer[idx];
	// 	idx++;
	// 	new_idx++;
	// }
	// command[new_idx] = '\0';
	
	// if (strcmp(command, "exit") == 0)
	// 	return -1;

	// while(buffer[idx] == ' ')
	// 	idx++;
	// new_idx = 0;
	// while(buffer[idx] != ' ') {
	// 	username[new_idx] = buffer[idx];
	// 	idx++;
	// 	new_idx++;
	// }	
	// username[new_idx] = '\0';

	// if (strcmp(command, "login") == 0) {
	// 	// login();
	// 	return 1;
	// }

	// while(buffer[idx] == ' ')
	// 	idx++;
	// new_idx = 0;
	// while(buffer[idx] != ' ') {
	// 	message[new_idx] = buffer[idx];
	// 	idx++;
	// 	new_idx++;
	// }	
	// message[new_idx] = '\0';

	// if (strcmp(message, "send") == 0) {
	// 	// send_message();
	// 	return 1;
	// }

	return 10;
}


void handle_heartbeat()
{
	beat = true;
	printf("beated :D\n");
}

bool check_input(int argc, char *argv[])
{
	if (argc != 5) {
		return false;
	}
	if (strcmp(argv[1], "--server-broadcasts-to") != 0) {
		return false;
	}
	if (strcmp(argv[3], "--clients-broadcasts-to") != 0) {
		return false;
	}
	return true;
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void logout ()
{

}

void login ()
{
	// current->next = malloc(sizeof(struct user));
}


int heartbeat(struct addrinfo *hints, char* server_broadcast_port)
{
	int heartbeatfd;
	int rv;

	if ((rv = getaddrinfo("localhost", server_broadcast_port, hints, &udp_servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and make a socket
	for(udp_p = udp_servinfo; udp_p != NULL; udp_p = udp_p->ai_next) {
		if ((heartbeatfd = socket(udp_p->ai_family, udp_p->ai_socktype,
				udp_p->ai_protocol)) == -1) {
			perror("tracker: socket");
			continue;
		}
		break;
	}

	if (udp_p == NULL) {
		fprintf(stderr, "tracker: failed to create socket\n");
		return 2;
	}

	

	// Sending heartbeats every 1 seconds
	signal(SIGALRM, handle_heartbeat);
	printf("Server started broadcasting\n");
	alarm(1);

	return heartbeatfd;
}


int main(int argc, char *argv[])
{
	// Ports
	char* server_broadcast_port;
	char* clients_broadcast_port;

	// Sets for select
	fd_set master;                      // master file descriptor list
	fd_set read_fds;                    // temp file descriptor list for select()
	int fdmax;                          // maximum file descriptor number

	// File Descriptors
	int heartbeatfd;                    // socket file descriptor for server's heartbeat
	int listener;						// Listener File Descriptor
	int newfd;                          // newly accept()ed socket descriptor

	// Address infos
	struct addrinfo tcp_hints, *tcp_ai, *tcp_p;
	struct addrinfo udp_hints;

	struct sockaddr_storage remoteaddr; // client address
	socklen_t addrlen;

	// others
	int numbytes_hearbeat;
	char buf[256];                       // buffer for client data
	int nbytes;
	char remoteIP[INET6_ADDRSTRLEN];

	int yes=1;                           // for setsockopt() SO_REUSEADDR, below
	int i, j, rv;
	int req_type;						 // 1 -> login | -1 -> exit | 0 -> send message


	if (!check_input(argc, argv)) {
		fprintf(stderr,"usage: server --server-broadcasts-to \"PORT M\" --clients-broadcasts-to \"PORT N\"\n");
		exit(1);       
	}
	server_broadcast_port = argv[2];
	clients_broadcast_port = argv[4];


	// Create UDP and TCP Hints
	memset(&udp_hints, 0, sizeof udp_hints);
	udp_hints.ai_family = AF_UNSPEC;
	udp_hints.ai_socktype = SOCK_DGRAM;

	memset(&tcp_hints, 0, sizeof tcp_hints);
	tcp_hints.ai_family = AF_UNSPEC;
	tcp_hints.ai_socktype = SOCK_STREAM;
	tcp_hints.ai_flags = AI_PASSIVE;     // use my IP

	FD_ZERO(&master);                    // clear the master and temp sets
	FD_ZERO(&read_fds);


	// Create Users Linked List
	struct user * users = (struct user *) malloc(sizeof (struct user));


	// Create Heartbeat Socket
	heartbeatfd = heartbeat(&udp_hints, server_broadcast_port);
	printf("hearbeat socket: %d \n", heartbeatfd);


	// Start creating TCP socket connection
    if ((rv = getaddrinfo(NULL, TRACKERLISTENINGPORT, &tcp_hints, &tcp_ai)) != 0) {
        fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
        exit(1);
    }

    for(tcp_p = tcp_ai; tcp_p != NULL; tcp_p = tcp_p->ai_next) {
        if (listener = socket(tcp_p->ai_family, tcp_p->ai_socktype, tcp_p->ai_protocol) < 0) {
            continue;
        }
        
        // lose the pesky "address already in use" error message
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(listener, tcp_p->ai_addr, tcp_p->ai_addrlen) < 0) {
            close(listener);
            continue;
        }
        break;
    }

    // if we got here, it means we didn't get bound
    if (tcp_p == NULL) {
        fprintf(stderr, "selectserver: failed to bind\n");
        exit(2);
    }

    freeaddrinfo(tcp_ai); // all done with this
	freeaddrinfo(udp_servinfo);

    // // listen
    if (listen(listener, 10) == -1) {
        perror("listen");
        exit(3);
    }

    // // add the listener to the master set
    FD_SET(listener, &master);

    // keep track of the biggest file descriptor
    if (heartbeatfd > listener)
    	fdmax = heartbeatfd;
    else 
    	fdmax = listener; // so far, it's this one


	while (1) {
		if(beat == true) {
			if ((numbytes_hearbeat = sendto(heartbeatfd, HEARTBEATMESSAGE, strlen(HEARTBEATMESSAGE), 0,
					 udp_p->ai_addr, udp_p->ai_addrlen)) == -1) {
				perror("tracker: sendto");
				exit(1);
			}
			beat = false;
			alarm(1);
		}


		read_fds = master; // copy it
        struct sigaction new_action, old_action;
        sigaction (SIGALRM, NULL, &old_action);
            		
		if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
			perror("select");
			// exit(4);
			// printf("sag\n");
			continue;
		}

		// run through the existing connections looking for data to read
		for(i = 0; i <= fdmax; i++) {
			if (FD_ISSET(i, &read_fds)) { // we got one!!
				if (i == listener) {
					// handle new connections
					addrlen = sizeof remoteaddr;
					newfd = accept(listener,
						(struct sockaddr *)&remoteaddr,
						&addrlen);

					if (newfd == -1) {
						perror("accept");
					} else {
						FD_SET(newfd, &master); // add to master set
						if (newfd > fdmax) {    // keep track of the max
							fdmax = newfd;
						}
						printf("selectserver: new connection from %s on "
							"socket %d\n",
							inet_ntop(remoteaddr.ss_family,
								get_in_addr((struct sockaddr*)&remoteaddr),
								remoteIP, INET6_ADDRSTRLEN),
							newfd);
					}
				} else {
					// handle data from a client
					if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0) {
						// got error or connection closed by client
						if (nbytes == 0) {
							// connection closed
							printf("selectserver: socket %d hung up\n", i);
						} else {
							perror("recv");
						}
						close(i); // bye!
						FD_CLR(i, &master); // remove from master set
					} else {
						// we got some data from a client
						buf[nbytes] = '\0';
						printf("%s\n", buf);
						// write(0, buf, nbytes);
						// req_type = handle_request(buf, nbytes);
						// if (req_type == 1) {
						// 	// login
						// 	login(users, buf, nbytes);
						// } else if (req_type == -1) {
						// 	// Exit
						// 	logout(users, buf, nbytes);
						// } else if (req_type == 0){
						// 	// Sed Message
						// } else {
						// 	// Bad input
						// 	printf("bad input\n");
						// 	continue;
						// }
					}
				}
			}
		}
	}	// End of While 1

	printf("tracker: sent %d bytes to %s\n", numbytes_hearbeat, argv[1]);
	close(heartbeatfd);

	return 0;
}