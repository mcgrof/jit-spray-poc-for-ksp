#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

// When userspace writes a pointer to /proc/jump, jump to that address in
// kernel mode.
ssize_t jump_write(struct file *file, const char *buf, size_t len, loff_t *data) {
    void (*fun)(void);
    uint32_t *pcontent;
    uint32_t content;

    if (len < sizeof(fun))
        return -EINVAL;

    if (copy_from_user(&fun, buf, sizeof(fun)))
        return -EFAULT;

    pcontent = (uint32_t*) fun;
    content = *pcontent;
    printk("jump.ko: Jumping to %p\n", fun);
    printk("jump.ko: content %x\n", content);
    fun();

    return len;
}

static const struct proc_ops proc_file_fops = {
 .proc_write  = jump_write,
};

// Create a file /proc/jump, with writes handled by jump_write.
int init_jump(void) {
    struct proc_dir_entry *ent = proc_create("jump", 0666, NULL, &proc_file_fops);
    if (!ent) {
	printk("jump.ko: failed to allocate jump proc entry\n");
	return -ENOMEM;
    }
    printk("jump.ko: Loaded incredibly insecure kernel module\n");
    return 0;
}

void exit_jump(void) {
    remove_proc_entry("jump", NULL);
}

module_init(init_jump);
module_exit(exit_jump);

MODULE_AUTHOR("Keegan McAllister");
MODULE_DESCRIPTION("Incredibly insecure kernel module for testing exploitation techniques");
MODULE_LICENSE("Dual BSD/GPL");
