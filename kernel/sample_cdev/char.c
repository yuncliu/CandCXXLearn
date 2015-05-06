#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#define DEVICE_NAME "mycdev"

static int Major = 0;
static int IsOpen = 0;

int my_init_module(void);
void my_cleanup_module(void);

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
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

module_init(my_init_module);
module_exit(my_cleanup_module);

MODULE_LICENSE("GPL");
