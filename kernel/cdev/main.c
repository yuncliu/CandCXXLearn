#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "char.h"

int call_ioctl();
int call_read();
int call_write();

int main(int argc, char* argv)
{
    call_read();
    return 0;
}

int call_ioctl() {
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

int call_read() {
    int fd = open("/dev/mdev", O_CREAT, S_IRWXU);
    char buf[10] = {0};
    read(fd, buf, 10);
    printf("read [%s]\n", buf);
    return 0;
}

int call_write() {
    int fd = open("/dev/mdev", O_CREAT | O_APPEND | O_RDWR);
    if (fd < 0) {
        printf("%s\n", strerror(errno));
        return -1;
    }
    char buf[10] = {0};
    sprintf(buf, "%s", "hellow");
    ssize_t w = write(fd, buf, 10);
    if (w < 0) {
        printf("%s\n", strerror(errno));
    }
    close(fd);
    return 0;
}
