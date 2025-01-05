#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <uk_comm/comm_ioctl.h>

const char devFile[] = "/dev/virtdev";

int main() {
    int fd;

    fd = open(devFile, O_RDWR);
    if (fd < 0)
	{
        perror("Failed to open device");
        return fd;
    }
	
    if (ioctl(fd, VIRT_IOCTL_GETSBINFO, NULL) == 0)
        printf("VIRT_IOCTL_GETSBINFO End\n");
	else
        perror("Failed to execute VIRT_IOCTL_GETSBINFO");

    close(fd);
    return 0;
}
