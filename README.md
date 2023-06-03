# Kprobe Hooks
Kprobes is a mechanism used to hook system call functions within the kernel using a defined structure, making it versatile and flexible. To enhance the explanation, let's rephrase and expand upon the steps involved:

Kprobes is a powerful mechanism in the Linux kernel that enables us to intercept system call functions using a standardized structure, making it universally applicable. The first step in using kprobes is to locate the table that contains the addresses of all the system calls. Luckily, Linux provides us with a convenient header file called "unistd.h" which defines all the known exported functions.

Once we have identified the system call table, we can proceed to attach two functions: a pre-call function and a post-call function. The pre-call function is executed just before the actual system call, giving us the opportunity to manipulate or modify the parameters or any other relevant data. On the other hand, the post-call function is executed immediately after the system call has completed its execution. This allows us to monitor and analyze the results, collect data, or perform any necessary post-processing.

By combining these pre-call and post-call functions, we gain the ability to both manipulate and monitor system calls effectively, providing us with valuable insights and control over the kernel's behavior. Kprobes serves as a robust tool for debugging, tracing, and analyzing the Linux kernel, making it an essential component for various system-level tasks and development activities.

```
#ifndef __HEADERS__
#define __HEADERS__
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/syscalls.h>
#include <linux/kallsyms.h>
#endif

const char *SYS_CALL_TABLE = "sys_call_table";

static struct kprobe hookOpen;
static struct kprobe hookWrite;
static struct kprobe hookRead;

static int preHandler(struct kprobe *p, struct pt_regs *regs);
void postHandler(struct kprobe *p, struct pt_regs *regs, unsigned long flags);
static int signHooks(unsigned long syscall_open, unsigned long syscall_write, long syscall_read);
static int setPointers(unsigned long * syscall_open, unsigned long * syscall_write, unsigned long * syscall_read);
```

## Usage:
develop under ubuntu 18.04. 
kernel version: 5.4.0

use makefile to compile the module,
installing the module:
```
sudo insmod drv1.ko
```

remove the module
```
sudo rmmod drv1
```
