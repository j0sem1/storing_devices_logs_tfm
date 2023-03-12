#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <linux/nvme_ioctl.h>

int main(int argc, char** argv) {
    int fd = open("dev/nvme0n1", O_RDWR);
    if (fd == 0) {
        perror("open: ");
        exit(1);
    }

    char buf[4096] = {0};
    struct nvme_admin_cmd mib = {0};
    mib.opcode = 0x06; // identify
    mib.nsid = 0;
    mib.addr = (__u64) buf;
    mib.data_len = sizeof(buf);
    mib.cdw10 = 1; // controller

    int ret = ioctl(fd, NVME_IOCTL_ADMIN_CMD, &mib);
    if (ret) {
        perror("ioctl: ");
        exit(1);
    }

    printf("SN: %.20s\n", &buf[4]);
    printf("SN: %.40s\n", &buf[24]);
    printf("FW: %.8s\n", &buf[64]);
    return 0;
}