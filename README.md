# 🧠 Embedded Linux Driver Learning Journey

A structured, hands-on project where I learn Linux kernel development by
building real drivers step-by-step — starting from a simple hello.ko and
progressing toward fully-featured character device drivers under /dev/.

All development is done inside a safe Ubuntu Virtual Machine, allowing
experimentation with kernel code without risking the host system.

====================================================================

PROJECT OUTCOME

By the end of this project, I aim to:

- Understand how Linux kernel modules work internally
- Design and implement real character device drivers
- Safely bridge user-space and kernel-space communication
- Debug kernel code using proper tooling and workflow
- Maintain a clean, professional, well-documented codebase

This repository documents both the technical implementation and the learning
process behind each phase.

====================================================================

REPOSITORY STRUCTURE

Each folder represents a focused milestone (phase) in the journey:

    1_hello/                 First Loadable Kernel Module (Hello World)
    2_simple_char_device/    Register a character device & VFS integration
    3_char_device_rw/        Implement real read() & write()
    4_ring_buffer_driver/    Add internal buffering
    5_thread_safe_driver/    Add locking & concurrency safety
    6_ioctl_control_driver/  Add IOCTL control interface
    7_sysfs_and_debug/       Expose config via sysfs + debugging
    8_blocking_driver/       Implement blocking I/O & wait queues
    9_interrupt_sim/         Simulated interrupt-driven behavior
    10_rpi_driver/           Optional: run on Raspberry Pi hardware

Each phase folder contains:

- C source code
- Makefile for kernel module builds
- README.md explaining the phase goals and implementation
- DOCUMENT.md capturing the learning process and challenges

====================================================================

COMPLETED PHASES

--------------------------------------------------------------------
PHASE 1 — 1_hello/
MY FIRST LINUX KERNEL MODULE

This phase introduced a minimal Loadable Kernel Module (LKM) that logs
messages when it is inserted into and removed from the kernel.

What was learned:

- What a kernel module is and how it differs from user-space programs
- Kernel-space vs user-space execution
- How to build .ko files using the kernel build system
- Basic kernel logging using printk / pr_info
- Core workflow using:
      insmod
      rmmod
      dmesg

This phase served as a safe introduction to kernel-space development.

--------------------------------------------------------------------
PHASE 2 — 2_simple_char_device/
MINIMAL CHARACTER DEVICE WITH VFS INTEGRATION

This phase implemented a real character device driver that registers with
the kernel and is accessible from user space via /dev/mydevice.

What was implemented:

- Dynamic allocation of major and minor device numbers
- Registration of a character device using cdev
- Wiring of open(), read(), and release() callbacks
- Manual creation of /dev/mydevice using mknod
- Minimal EOF-based read() implementation
- Kernel logging with filtered debugging via dmesg

What was learned:

- How the kernel identifies devices using major/minor numbers
- How /dev acts as a user-space abstraction over kernel device numbers
- How the Virtual File System (VFS) routes system calls to drivers
- Why read() returning 0 signals End-Of-File (EOF)
- How tools like cat interact with character devices
- How to debug kernel code methodically using logs

Phase 2 established the complete control path:

    user program → VFS → device number → driver callbacks

====================================================================

CURRENT FOCUS / NEXT PHASE

PHASE 3 — 3_char_device_rw/
REAL DATA TRANSFER

The next phase will introduce:

- Internal kernel buffers
- write() support (user → kernel)
- read() returning real data (kernel → user)
- copy_to_user() and copy_from_user()
- Basic error handling and state management

This phase will transition the driver from control-only to data-capable.

====================================================================

TOOLS USED

- Ubuntu Linux (Virtual Machine)
- GCC and Make
- Linux kernel headers
- VirtualBox
- Git and GitHub
- Kernel tools: dmesg, insmod, rmmod, lsmod

====================================================================

TESTING AND DEVELOPMENT PHILOSOPHY

Every phase must:

- Build cleanly
- Load and unload without warnings
- Log clearly and consistently
- Fail safely when misused
- Avoid kernel crashes or undefined behavior
- Document both implementation and learning

The emphasis is on understanding, not just making something work.

====================================================================

FINAL GOAL

By the end of this project, I want to confidently say:

“I understand how Linux kernel drivers are designed, implemented, and debugged.”

And this repository should demonstrate:

- Real kernel code
- Incremental progression
- Strong mental models
- Engineering discipline
- Clear documentation

====================================================================

WHY I’M DOING THIS

Linux runs critical infrastructure everywhere.
Understanding the kernel means understanding how computers actually work.

And it’s fun.
