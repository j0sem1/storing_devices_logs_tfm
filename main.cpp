#include "hard_disk.h"
#include "usb.h"
#include "output.h"
#include <unistd.h> // For geteuid()


int main(){

    /*
    if (argc != 2) {
		fprintf(stderr, "Usage: %s <devname>\n", program_invocation_short_name);
		return 1;
	}
    */

	if (geteuid() > 0) {
		fprintf(stderr, "ERROR: Must be root to use\n");
		return 1;
	}

    printUSBsMapInTerminal(getUSBsInfo());

    printDisksMapInTerminal(getDisksInfo());

    printHTML(getUSBsInfo(), getDisksInfo());

    return 0;
}