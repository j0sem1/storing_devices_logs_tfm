#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define UDEV_SOCKET "/run/udev/control"

int main() {
    // Create a socket to communicate with the udev daemon
    int udev_socket = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (udev_socket < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Create a message to request device information
    char udev_request[] = "get_property block:* ID_TYPE=disk ID_SERIAL_SHORT";

    // Send the message to the udev daemon
    struct sockaddr_un udev_address;
    memset(&udev_address, 0, sizeof(udev_address));
    udev_address.sun_family = AF_UNIX;
    strncpy(udev_address.sun_path, UDEV_SOCKET, sizeof(udev_address.sun_path) - 1);
    ssize_t bytes_sent = sendto(udev_socket, udev_request, sizeof(udev_request), 0,
                                 (struct sockaddr *) &udev_address, sizeof(udev_address));
    if (bytes_sent < 0) {
        perror("Error sending message to udev daemon");
        exit(EXIT_FAILURE);
    }

    // Read the response from the udev daemon
    char udev_response[1024];
    ssize_t bytes_received = recv(udev_socket, udev_response, sizeof(udev_response), 0);
    if (bytes_received < 0) {
        perror("Error receiving message from udev daemon");
        exit(EXIT_FAILURE);
    }

    // Parse the response to extract the serial numbers
    char *serial_number = strtok(udev_response, "\n");
    while (serial_number != NULL) {
        printf("%s\n", serial_number);
        serial_number = strtok(NULL, "\n");
    }

    // Close the socket and exit
    close(udev_socket);
    return 0;
}
