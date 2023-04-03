all: get_devices

get_devices: main.o usb.o hard_disk.o
	g++ -o get_devices.out main.o usb.o hard_disk.o -static

usb.o: usb.cpp usb.h
	g++ -c usb.cpp

hard_disk.o: hard_disk.cpp hard_disk.h
	g++ -c hard_disk.cpp

main.o: main.cpp
	g++ -c main.cpp