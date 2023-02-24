#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <poll.h>

#include "sockets.h"
#include "utility.h"

// Get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// Return a listening socket
int get_listener_socket(void)
{
    int listener;     // Listening socket descriptor
    int yes=1;        // For setsockopt() SO_REUSEADDR, below
    int rv;

    struct addrinfo hints, *ai, *p;

    // Get us a socket and bind it
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
        fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
        exit_with_error(1);
    }

    for(p = ai; p != NULL; p = p->ai_next) {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0) {
            continue;
        }

        // Lose the pesky "address already in use" error message
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
            close(listener);
            continue;
        }

        break;
    }

    // If we got here, it means we didn't get bound
    if (p == NULL) {
        return -1;
    }

    freeaddrinfo(ai); // All done with this

    // Listen
    if (listen(listener, CONNECTION_QUEUE) == -1) {
        return -1;
    }

    return listener;
}

void add_connection(struct pollfd pfds[], int newfd, int *connection_count)
{
    pfds[*connection_count].fd = newfd;
    pfds[*connection_count].events = POLLIN; // Check ready-to-read

    (*connection_count)++;
}

void remove_connection(struct pollfd pfds[], char client_buffer[][CLIENT_BUFFER_SIZE], int i, int *connection_count)
{
    // Copy the one from the end over this one
    pfds[i] = pfds[*connection_count-1];
    memcpy(&client_buffer[i], &client_buffer[*connection_count], sizeof(*client_buffer));

    // clean the buffer for the new empty slot
    memset(&client_buffer[*connection_count-1], 0, sizeof(*client_buffer));

    (*connection_count)--;
}
