#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>
#include "char.h"


//static int Major = 0;
static int Major = 0;
static int IsOpen = 0;

int my_init_module(void);
void my_cleanup_module(void);
static char msg[10] = {0};

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);
static long device_ioctl(struct file *, unsigned int , unsigned long);

static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
	.unlocked_ioctl = device_ioctl,
    .open = device_open,
    .release = device_release
};


int my_init_module() {
    printk("init module\n");
    Major = register_chrdev(0, DEVICE_NAME, &fops);
    printk("Mator = [%d]\n", Major);
    return 0;
}

void my_cleanup_module() {
    unregister_chrdev(Major, DEVICE_NAME);
}

static int device_open(struct inode* node, struct file* file) {
    if (IsOpen) {
        printk("already opened [%d]\n", IsOpen);
        return -1;
    }
    printk("open\n");
    IsOpen++;

    return 0;
}

static int device_release(struct inode* node, struct file* file) {
    printk("close\n");
    IsOpen--;
    return 0;
}

static ssize_t device_read(struct file* file, char* buf, size_t size, loff_t* offset) {
    memset(buf, 0xAA, size);
    return size;
}

static ssize_t device_write(struct file* file, const char* buf, size_t size, loff_t* offset) {
    return size;
}

long device_ioctl(struct file *file, /* ditto */
        unsigned int ioctl_num,    /* number and param for ioctl */
        unsigned long ioctl_param)
{
#if 1
    char* p = NULL;
    switch (ioctl_num) {
        case IOCTL_XSET_MSG:
            p = (char*)ioctl_param;
            printk("IOCTL_SET_MSG [%s]\n", p);
            copy_from_user(msg, p, 10);
            break;
        case IOCTL_XGET_MSG:
            p = (char*)ioctl_param;
            printk("IOCTL_GET_MSG\n");
            copy_to_user(p, msg,10);
            break;
        default:
            printk("wrong number [%d]\n", ioctl_num);
    }
#endif
    return 0;
}

module_init(my_init_module);
module_exit(my_cleanup_module);

MODULE_LICENSE("GPL");
