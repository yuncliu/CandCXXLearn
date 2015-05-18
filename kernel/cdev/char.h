#ifndef _CHAR_H_
#define _CHAR_H_
#include <linux/ioctl.h>

#define DEVICE_NAME "mycdev"

//extern int Major;
#define MAJOR_NUM 101

#define IOCTL_XSET_MSG _IOR(MAJOR_NUM, 0, char*)
#define IOCTL_XGET_MSG _IOW(MAJOR_NUM, 1, char*)




#endif
