#include "drv1.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Proxytype");

static int preHandler(struct kprobe *p, struct pt_regs *regs)
{
   printk(KERN_INFO "pre_handler: p->addr = 0x%p, ip = %lx,"
			" flags = 0x%lx\n",
		p->addr, regs->ip, regs->flags);
    
    //change result for different response
    return 0;
}

void postHandler(struct kprobe *p, struct pt_regs *regs, unsigned long flags){
	printk(KERN_INFO "post_handler: p->addr = 0x%p, flags = 0x%lx\n",
		p->addr, regs->flags);
}

static int signHooks(unsigned long syscall_open, unsigned long syscall_write, long syscall_read) {

    int ret = 0;

    hookOpen.pre_handler = preHandler;
    hookOpen.post_handler = postHandler;
    hookOpen.addr = (kprobe_opcode_t *)syscall_open;

    ret = register_kprobe(&hookOpen);
    if (ret < 0) {
        printk(KERN_ERR "Failed to register kprobe for \"OpenAt\" call\n%d", ret);
        return ret;
    }

    hookWrite.pre_handler = preHandler;
    hookWrite.post_handler = postHandler;
    hookWrite.addr = (kprobe_opcode_t *)syscall_write;

    ret = register_kprobe(&hookWrite);
    if (ret < 0) {
        printk(KERN_ERR "Failed to register kprobe for \"Write\" call\n%d", ret);
        return ret;
    }

    hookRead.pre_handler = preHandler;
    hookRead.post_handler = postHandler;
    hookRead.addr = (kprobe_opcode_t *)syscall_read;

    ret = register_kprobe(&hookRead);
    if (ret < 0) {
        printk(KERN_ERR "Failed to register kprobe for \"Read\" call\n%d", ret);
        return ret;
    }

    return ret;

}

static int setPointers(unsigned long * syscall_open, unsigned long * syscall_write, unsigned long * syscall_read) 
{

    unsigned long *sys_call_table = kallsyms_lookup_name(SYS_CALL_TABLE);

    if(sys_call_table == 0) {

        printk(KERN_ERR "Failed to get system call table\n");
        return -1;
    }

    (*syscall_open) = (unsigned long)sys_call_table[__NR_openat];
    (*syscall_write) = (unsigned long)sys_call_table[__NR_write];
    (*syscall_read) = (unsigned long)sys_call_table[__NR_read];

    if(syscall_open <= 0) {
        printk(KERN_ERR "Failed to get system open call\n");
        return -1;
    } 

    if(syscall_write <= 0) {
        printk(KERN_ERR "Failed to get system write call\n");
        return -1;
    }

    if(syscall_read <= 0) {
        printk(KERN_ERR "Failed to get system read call\n");
        return -1;
    }

    return 0;
}

static int __init moduleInit(void)
{
    int ret;

    // initialize the kprobe instance
    memset(&hookOpen, 0, sizeof(struct kprobe));
    memset(&hookWrite, 0, sizeof(struct kprobe));
    memset(&hookRead, 0, sizeof(struct kprobe));

    unsigned long syscall_open;
    unsigned long syscall_write;
    unsigned long syscall_read;

    ret = setPointers(&syscall_open, &syscall_write, &syscall_read);

    if(ret < 0) {
        return ret;
    }

    ret = signHooks(syscall_open, syscall_write, syscall_read);

    return ret;
}

static void __exit moduleExit(void)
{
    unregister_kprobe(&hookOpen);
    unregister_kprobe(&hookWrite);
    unregister_kprobe(&hookRead);

    printk(KERN_INFO "module unhooked\n");
}

module_init(moduleInit);
module_exit(moduleExit);
