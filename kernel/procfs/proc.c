#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#define FILE_NAME "hellow"
#define BUF_SIZE 1024


int my_init_module(void);
void my_cleanup_module(void);

static int proc_open(struct inode *, struct file *);
static int proc_release(struct inode *, struct file *);
static ssize_t proc_read(struct file *, char *, size_t, loff_t *);
static ssize_t proc_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops = {
    .read = proc_read,
    .write = proc_write,
    .open = proc_open,
    .release = proc_release
};

static char _buf[BUF_SIZE];
static struct proc_dir_entry* proc_entry;

int __init my_init_module() {
    printk("init module\n");
    proc_entry = proc_create(FILE_NAME, S_IWUSR, NULL, &fops);
    //proc_entry = create_proc_entry("hellow", 0644, NULL);
    memset(_buf, 0x00, sizeof(_buf));
    return 0;
}

void my_cleanup_module() {
    proc_remove(proc_entry);
}

static int proc_open(struct inode* node, struct file* file) {
    printk("open\n");
    return 0;
}

static int proc_release(struct inode* node, struct file* file) {
    printk("close\n");
    return 0;
}

static ssize_t proc_read(struct file* file, char* buf, size_t size, loff_t* offset) {
    printk("read, offset = %lld\n", *offset);
    if (*offset > 0) {
        return 0;
    }
    snprintf(buf, size, "%s\n%s", "hellow", _buf);
    *offset += strlen(buf);
    return *offset;
}

static ssize_t proc_write(struct file* file, const char* buf, size_t size, loff_t* offset) {
    size_t should_copy = size > BUF_SIZE ? BUF_SIZE : size;
    copy_from_user(_buf, buf, should_copy);
    printk("get input:%s\n", buf);
    return should_copy;
}

module_init(my_init_module);
module_exit(my_cleanup_module);

MODULE_LICENSE("GPL");
