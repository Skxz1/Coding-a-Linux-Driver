01_hello
A simple Hello World Linux Kernel Module created as part of my Linux driver learning journey.

This example can be compiled and run on a normal x86 Ubuntu machine or on embedded Linux (e.g. Raspberry Pi) as long as kernel headers are installed.

What this project is about
--------------------------
This is my first Loadable Kernel Module (LKM). The goal is to understand how code can be loaded into the Linux kernel at runtime and removed again — safely — while also learning how printk + dmesg logging works.

Unlike normal programs which run in user space, a kernel module runs inside the kernel. This means it has full control and no protection if something goes wrong — which is why this work is always done inside a VM.

The Hello World Kernel Module
-----------------------------
Two events matter for a kernel module:

1. when it is loaded into the kernel
2. when it is unloaded from the kernel

We register functions for these events using the macros:

module_init()
module_exit()

The function called when the module is loaded normally looks like this:

int my_init(void)

It returns an int. Returning 0 means the module was loaded successfully. Returning a negative value signals that initialization failed.

The function called when the module is removed looks like this:

void my_exit(void)

This function has no arguments and no return value. Its job is to clean up anything allocated during initialization.

Inside both functions we use printk (or pr_info). printk works like printf, except it prints to the kernel log rather than the terminal.

To declare the module license we use:

MODULE_LICENSE("GPL");

This tells the kernel that the module is open-source licensed, which avoids warnings and is required by some Linux distributions.

Makefile for building the module
--------------------------------
Kernel modules are built using the kernel build system. Our Makefile declares the object to build and then calls into the kernel's build environment.

The module object is declared like this:

obj-m += hello.o

Then `make` calls the kernel build system in:

/lib/modules/$(uname -r)/build

which compiles hello.c into hello.o and then hello.ko (the actual kernel module).

Commands to manage the module
-----------------------------

Build the module:
make

Follow the kernel log in real time:
sudo dmesg -w

Insert the module:
sudo insmod hello.ko

Remove the module:
sudo rmmod hello

View the most recent kernel log lines:
dmesg | tail -20

What I learned from this
------------------------
• Difference between kernel space and user space  
• How kernel modules are built using Makefiles  
• How printk sends messages to the kernel log  
• How to use dmesg -w to stream kernel messages  
• How to safely load and unload modules  
• Why build files should be ignored using .gitignore  

Repository structure
--------------------
Only source code is tracked:

hello.c
Makefile

Build artifacts such as .ko, .o, .mod.* etc are excluded via .gitignore.

Next steps
----------
This is Phase 1 of my embedded Linux driver project.

Next I will:
• create a character device driver  
• expose /dev/mydevice  
• implement read and write handling  
• continue learning and documenting the process  


