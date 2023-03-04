#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <scsi/sg.h>

#define INQUIRY_CMD 0x12
#define INQUIRY_CMD_LEN 6
#define INQUIRY_REPLY_LEN 96
#define VENDOR_ID_OFFSET 8
#define PRODUCT_ID_OFFSET 16
#define SERIAL_NUMBER_OFFSET 36
#define SERIAL_NUMBER_LEN 20

int main(int argc, char *argv[]) {
    int fd, result;
    unsigned char inquiry_cmd[INQUIRY_CMD_LEN] = { INQUIRY_CMD, 0x00, 0x00, 0x00, INQUIRY_REPLY_LEN, 0 }; // SCSI INQUIRY command
    unsigned char inquiry_reply[INQUIRY_REPLY_LEN];
    sg_io_hdr_t io_hdr;
    char serial_number[SERIAL_NUMBER_LEN + 1];

    // Open the device file
    fd = open("/dev/sdb", O_RDWR | O_NONBLOCK);
    if (fd < 0) {
        perror("open");
        exit(1);
    }

    // Initialize the SCSI command buffer
    memset(&io_hdr, 0, sizeof(io_hdr));
    // Guard field. Current implementations only accept " (int)'S' ". If not set, the sg driver sets errno to ENOSYS while the block layer sets it to EINVAL.
    io_hdr.interface_id = 'S';
    // Limits command length to 255 bytes. No SCSI commands (even variable length ones in OSD) are this long (yet)
    io_hdr.cmd_len = sizeof(inquiry_cmd);
    // Maximum number of bytes of sense data that the driver can output via the sbp pointer
    io_hdr.mx_sb_len = 32;
    io_hdr.dxfer_direction = SG_DXFER_FROM_DEV;
    // Number of bytes of data to transfer to or from the device. Upper limit for block devices related to /sys/block/<device>/queue/max_sectors_kb
    // 1280 from 'cat /sys/block/sda/queue/max_sectors_kb'
    io_hdr.dxfer_len = sizeof(inquiry_reply);
    // Pointer to (user space) data to transfer to (if reading from device) or transfer from (if writing to device).
    // Further level of indirection in the sg driver when iovec_count is greater than 0.
    io_hdr.dxferp = inquiry_reply;
    // Pointer to SCSI command. The SG_IO ioctl in the sg drive fails with errno set to  EMSGSIZE if cmdp is NULL and EFAULT if it is invalid;
    // the block layer sets errno to EFAULT  in both cases.
    io_hdr.cmdp = inquiry_cmd;
    // time in milliseconds that the SCSI mid-level will wait for a response. If that timer expires before the command finishes,
    // then the command may be aborted, the device (and maybe others on the same interconnect) may be reset depending on error handler settings.
    // Dangerous stuff, the SG_IO ioctl has no control (through this interface) of exactly what happens.
    // In the sg driver a timeout value of 0 means 0 milliseconds, in the block layer (currently) it means 60 seconds.
    io_hdr.timeout = 20000;

    // Send the SCSI command
    result = ioctl(fd, SG_IO, &io_hdr);
    if (result < 0) {
        perror("ioctl");
        exit(1);
    }

    // Extract the serial number from the response
    strncpy(serial_number, (char *)(inquiry_reply + SERIAL_NUMBER_OFFSET), SERIAL_NUMBER_LEN);
    serial_number[SERIAL_NUMBER_LEN] = '\0';

    // Print the results
    printf("Vendor ID:     %.*s\n", VENDOR_ID_OFFSET, inquiry_reply);
    printf("Product ID:    %.*s\n", PRODUCT_ID_OFFSET - VENDOR_ID_OFFSET, inquiry_reply + VENDOR_ID_OFFSET);
    printf("Serial Number: %s\n", serial_number);

    for(int i = 0; i < sizeof(inquiry_reply); i++){
        printf("Character %d: %c\n", i, inquiry_reply[i]);    
    }
    printf("All data obtained: %s\n", inquiry_reply);

    // Close the device file
    close(fd);

    return 0;
}
