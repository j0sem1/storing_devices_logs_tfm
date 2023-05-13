CFLAGS = -Wall -Werror -Wextra
#CFLAGS = -Wall -Wextra
CC = g++

all: get_devices

get_devices: main.o output.o usb.o hard_disk.o
	$(CC) -o get_devices.out output.o main.o usb.o hard_disk.o -static

output.o: output.cpp output.h
	$(CC) $(CFLAGS) -c output.cpp

usb.o: usb.cpp usb.h
	$(CC) $(CFLAGS) -c usb.cpp

hard_disk.o: hard_disk.cpp hard_disk.h
	$(CC) $(CFLAGS) -c hard_disk.cpp

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp