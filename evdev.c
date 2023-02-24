#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/uinput.h>

#include "evdev.h"
#include "utility.h"

int ev_fd;

int ev_close(int fd) {
    ioctl(fd, UI_DEV_DESTROY);
    return close(fd);
}

int ev_open()
{
    int fd = open(DEVICE_PATH, O_WRONLY | O_NONBLOCK);

    if (fd < 0)
    {
        fprintf(stderr, "Unable to open uinput device: %s\n", DEVICE_PATH);
        exit_with_error(1);
    }

    ioctl(fd, UI_SET_EVBIT, EV_KEY);
    for (int i=KEY_ESC; i<KEY_MAX; i++) {
        ioctl(fd, UI_SET_KEYBIT, i);
    }

    struct uinput_setup usetup;
    memset(&usetup, 0, sizeof(usetup));
    usetup.id.bustype = BUS_USB;
    usetup.id.vendor = 0x1234; /* fake vendor */
    usetup.id.product = 0x5678; /* fake product */
    strcpy(usetup.name, DEVICE_NAME);
    ioctl(fd, UI_DEV_SETUP, &usetup);
    ioctl(fd, UI_DEV_CREATE);

    return fd;
}

void ev_emit(int fd, int type, int code, int val) {
    struct input_event ie = {0,};
    ie.type = type;
    ie.code = code;
    ie.value = val;
    write(fd, &ie, sizeof(ie));
}

void emulate_key_press(int fd, int key) {
  msleep(INTER_KEY_WAIT);
  ev_emit(fd, EV_KEY, key, 1);
  ev_emit(fd, EV_SYN, SYN_REPORT, 0);
}

void emulate_key_release(int fd, int key) {
  msleep(INTER_KEY_WAIT);
  ev_emit(fd, EV_KEY, key, 0);
  ev_emit(fd, EV_SYN, SYN_REPORT, 0);
}

void emulate_key(int fd, int key) {
  emulate_key_press(fd, key);
  emulate_key_release(fd, key);
}
