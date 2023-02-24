#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <unistd.h>

#include "evdev.h"
#include "keys.h"
#include "sockets.h"
#include "utility.h"

// #define DEBUG

void process_kbd_command(char* arg1, char* arg2)
{
    int keyCode;

    // if arg2 is empty then we have no modifiers
    if (arg2 == NULL)
    {
        keyCode = find_key(arg1);

        if (keyCode != -1)
          emulate_key(ev_fd, keyCode);
        else
          fprintf(stderr, "Unable to find keycode: %s\n", arg1);

        return;
    }

    // maximum amount of modifiers
    int modifierList[5];

    int m=0;

    char* modifier = strtok(arg1, "+");

    while ((modifier != NULL) && (m < 5))
    {
        keyCode = find_key(modifier);

        if (keyCode != -1)
          modifierList[m++] = keyCode;
        else
          fprintf(stderr, "Unable to find modifier keycode: %s\n", modifier);

        modifier = strtok(NULL, "+");
    }

    for(int x = 0; x < m; x++)
      emulate_key_press(ev_fd, modifierList[x]);

    keyCode = find_key(arg2);

    if (keyCode != -1)
    {
        emulate_key(ev_fd, keyCode);

        for(int x = 0; x < m; x++)
          emulate_key_release(ev_fd, modifierList[x]);
    }
    else
      fprintf(stderr, "Unable to find keycode: %s\n", arg2);
}

void process_command(char* cmdString)
{
    #ifdef DEFINE
    fprintf(stdout, "COMMAND: %s\r\n", cmdString);
    #endif

    char *cmd, *arg1, *arg2;

    cmd = strtok(cmdString, " ");

    if (cmd == NULL)
        return;

    arg1 = strtok(NULL, " ");

    if (arg1 != NULL)
        arg2 = strtok(NULL, " ");

    if (strcmp(cmd, "KBD") == 0)
    {
        process_kbd_command(arg1, arg2);
    }
}

void process_buffer(char* buffer)
{
    char* terminator = strchr(buffer, '\r');

    while(terminator != NULL)
    {
        *terminator = '\0';

        process_command(buffer);

        if (*(terminator + 1) == '\n')
            strcpy(buffer, terminator + 2);
        else
            strcpy(buffer, terminator + 1);

        terminator = strchr(buffer, '\r');
    }
}

int main(void)
{
    fprintf(stdout, "Remote Input Daemon starting...\r\n");

    fprintf(stdout, "Creating evdev device... ");
    ev_fd = ev_open();
    fprintf(stdout, "done.\r\n");

    fprintf(stdout, "Filling keys list array... ");
    fill_keylist();
    fprintf(stdout, "done.\r\n");

    int listener;     // Listening socket descriptor

    int newfd;        // Newly accept()ed socket descriptor
    struct sockaddr_storage remoteaddr; // Client address
    socklen_t addrlen;

    char remoteIP[INET6_ADDRSTRLEN];

    // includes the listener
    int connection_count = 0;
    struct pollfd pfds[MAX_CONNECTIONS];

    // listener doesn't need an buffer array but it's small and simplifies code
    char client_buffer[MAX_CONNECTIONS][CLIENT_BUFFER_SIZE];
    memset(&client_buffer, 0, sizeof client_buffer);

    fprintf(stdout, "Setting up listening socket... ");

    // Set up and get a listening socket
    listener = get_listener_socket();

    if (listener == -1) {
        fprintf(stderr, "error getting listening socket\n");
        exit_with_error(1);
    }
    
    fprintf(stdout, "done.\r\n");

    fprintf(stdout, "Listening on 0.0.0.0:%s...\r\n", PORT);

    // Add the listener to set
    pfds[0].fd = listener;
    pfds[0].events = POLLIN; // Report ready to read on incoming connection

    connection_count = 1; // For the listener

    // Main loop
    for(;;) {
        int poll_count = poll(pfds, connection_count, -1);

        if (poll_count == -1) {
            perror("poll");
            exit_with_error(1);
        }

        // Run through the existing connections looking for data to read
        for(int i = 0; i < connection_count; i++) {

            // Check if someone's ready to read
            if (pfds[i].revents & POLLIN) { // We got one!!

                if (pfds[i].fd == listener) {
                    // If listener is ready to read, handle new connection

                    addrlen = sizeof remoteaddr;
                    newfd = accept(listener, (struct sockaddr *)&remoteaddr, &addrlen);

                    if (newfd == -1) {
                        perror("accept");
                    } else {
                        fprintf(stdout, "new connection from %s on "
                            "socket %d\n",
                            inet_ntop(remoteaddr.ss_family,
                                get_in_addr((struct sockaddr*)&remoteaddr),
                                remoteIP, INET6_ADDRSTRLEN),
                            newfd);

                        if (connection_count == MAX_CONNECTIONS)
                        {
                            fprintf(stdout, "rejecting new connection because server is full\n");
                            char* msg = "Sorry, the maximum number of connections has been reached.\r\n";
                            send(newfd, msg, strlen(msg), 0);
                            close(newfd);
                            continue;
                        }

                        add_connection(pfds, newfd, &connection_count);
                    }
                } else {

                    // If not the listener, we're just a regular client

                    // We use the null character to indicate our current buffer position
                    char* bufferPosition = strchr(client_buffer[i], '\0');
                    char* endOfBuffer = (client_buffer[i] + CLIENT_BUFFER_SIZE);
                    int remainingBuffer = endOfBuffer - bufferPosition;

                    int nbytes = recv(pfds[i].fd, bufferPosition, remainingBuffer, 0);

                    if (nbytes <= 0) {
                        // Got error or connection closed by client
                        if (nbytes == 0) {
                            // Connection closed
                            fprintf(stdout, "socket %d hung up\n", pfds[i].fd);
                        } else {
                            perror("recv");
                        }

                        close(pfds[i].fd); // Bye!

                        remove_connection(pfds, client_buffer, i, &connection_count);

                    } else {
                        // We got some good data from a client

                        // Mark the end of the current buffer data with a null
                        *(bufferPosition + nbytes) = 0;

                        process_buffer(client_buffer[i]);
                    }
                } // END handle data from client
            } // END got ready-to-read from poll()
        } // END looping through file descriptors
    } // END for(;;)--and you thought it would never end!

    ev_close(ev_fd);

    return 0;
}
