#CC = gcc
CC = /usr/local/src/arm-linux-musleabihf-cross/bin/arm-linux-musleabihf-gcc
CFLAGS = -std=c99 -static -D_XOPEN_SOURCE=700

all: risd

risd: main.o keys.o evdev.o sockets.o utility.o
	$(CC) $(CFLAGS) -o risd main.o keys.o evdev.o sockets.o utility.o

main.o: main.c
	$(CC) $(CFLAGS)  -c main.c

keys.o: keys.c
	$(CC) $(CFLAGS) -c keys.c

evdev.o: evdev.c
	$(CC) $(CFLAGS) -c evdev.c

sockets.o: sockets.c
	$(CC) $(CFLAGS) -c sockets.c

utility.o: utility.c
	$(CC) $(CFLAGS) -c utility.c

clean:
	rm risd main.o keys.o evdev.o sockets.o utility.o

upload:
	curl --insecure --user root:1 -T risd sftp://mister/root/risd
