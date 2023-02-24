#define PORT "8063"
#define CLIENT_BUFFER_SIZE 256
#define MAX_CONNECTIONS 10
#define CONNECTION_QUEUE 4

void *get_in_addr(struct sockaddr *sa);
int get_listener_socket(void);
void add_connection(struct pollfd pfds[], int newfd, int *connection_count);
void remove_connection(struct pollfd pfds[], char client_buffer[][CLIENT_BUFFER_SIZE], int i, int *connection_count);