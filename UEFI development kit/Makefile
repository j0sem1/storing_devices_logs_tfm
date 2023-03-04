all: get_devices

get_devices: main.o efi_library.o
	g++ -o get_devices main.o efi_library.o

efi_library.o: efi_library.cpp efi_library.h
	g++ -I/usr/include/efi -I/usr/include/efi/x86_64 -I/usr/include/efi/protocol -c efi_library.cpp

main.o: main.cpp
	g++ -c main.cpp