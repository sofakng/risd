#include <stdlib.h>
#include <time.h>

#include "evdev.h"

void msleep(long ms) {
  struct timespec w;
  w.tv_sec = ms / 1000;
  w.tv_nsec = (ms % 1000 ) * 1000000;
  while (nanosleep(&w, &w));
}

void exit_with_error(int exitCode)
{
    if (ev_fd > 0)
        ev_close(ev_fd);

    exit(exitCode);
}
