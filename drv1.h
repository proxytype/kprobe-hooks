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
