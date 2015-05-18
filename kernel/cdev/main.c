#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "char.h"


int main(int argc, char* argv)
{
    int fd = open("/dev/mdev", O_CREAT, S_IRWXU);
    char hellow[10] = {0};
    snprintf( hellow, 10, "%s", "hellow");
    ioctl(fd, IOCTL_XSET_MSG, hellow);

    char buf[10] = {0};
    ioctl(fd, IOCTL_XGET_MSG, buf);
    printf("get [%s]\n", buf);

    close(fd);
    return 0;
}
