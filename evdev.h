#define DEVICE_NAME "Remote Input Daemon Device"
#define DEVICE_PATH "/dev/uinput"

#define INTER_KEY_WAIT 40

int ev_close(int fd);
int ev_open();
void emulate_key_press(int fd, int key);
void emulate_key_release(int fd, int key);
void emulate_key(int fd, int key);

extern int ev_fd;