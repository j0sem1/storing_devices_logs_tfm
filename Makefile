all: get_devices

get_devices: main.o hard_disks.o
	g++ -o a.out main.o hard_disks.o

hard_disks.o: hard_disks.cpp hard_disks.h
	g++ -c hard_disks.cpp

main.o: main.cpp
	g++ -c main.cpp