# 🧠 Embedded Linux Driver Learning Journey

A structured, hands-on project where I learn **Linux kernel development** by building real drivers step-by-step — starting from a simple `hello.ko` and progressing to a fully-functioning **character device under `/dev/`**.

This project is built inside a **safe Ubuntu Virtual Machine** so I can experiment without risking my host system.

---

## 🎯 Project Outcome

✔ Understand **how Linux kernel modules work**  
✔ Create a **real character device driver (/dev/mydevice)**  
✔ Safely pass data between **user-space & kernel-space**  
✔ Learn debugging & kernel development workflow  
✔ Publish clean, professional code & documentation  

---

## 🗂 Repository Structure

Each folder is a small focused milestone:

```
1_hello/                 First Loadable Kernel Module (Hello World)
2_simple_char_device/    Register a device & create /dev entry
3_char_device_rw/        Implement read() & write()
4_ring_buffer_driver/    Add internal buffering
5_thread_safe_driver/    Add locking & concurrency safety
6_ioctl_control_driver/  Add IOCTL control interface
7_sysfs_and_debug/       Expose config via sysfs + debugging
8_blocking_driver/       Implement blocking I/O & wait queues
9_interrupt_sim/         Simulated interrupt-driven behavior
10_rpi_driver/           Optional: run on Raspberry Pi hardware
```

I update each folder with:

📝 README.md explaining the *concepts*  
💻 Source code  
🧪 Testing notes  
🐛 Issues & debugging log  

---

## ✅ Completed So Far – Phase 1

### `1_hello/` — My First Linux Kernel Module 🎉

This module prints to the kernel log when:

✔ it is **inserted** (`init` function)  
✔ it is **removed** (`exit` function)  

This taught me:

- what a **Loadable Kernel Module (LKM)** is  
- how kernel vs user-space differ  
- how to build `.ko` files with a Makefile  
- how to use:

```
sudo insmod module.ko
sudo rmmod module
sudo dmesg
```

This was my **“hello world from kernel-space”** milestone 🧠

---

## 🚀 Current Work — Phase 2

### `2_simple_char_device/`

Goal:

> Create a **minimal character device driver** that appears under `/dev/`

I will learn:

- major & minor device numbers
- registering a `cdev`
- wiring file operations
- how `/dev` works internally

Once complete:

✔ `/dev/mydevice` will exist  
✔ I will be able to **open the device from user-space**

(Then later phases will add read/write, buffers, IOCTL, threads, etc.)

---

## 🛠 Tools Used

- Ubuntu Linux (inside VM)
- GCC + Make
- Linux kernel headers
- VirtualBox
- Git & GitHub
- `dmesg`, `insmod`, `rmmod`, `lsmod`

---

## 🧪 Testing Philosophy

Every module must:

✔ load without warnings  
✔ log clearly to dmesg  
✔ unload cleanly  
✔ handle invalid input safely  
✔ avoid kernel crashes (panic)  
✔ document behaviour + lessons learned  

---

## ⭐ Final Goal

By the end of this project I want to confidently say:

> “I can design, build and debug Linux kernel drivers.”

And my repo should reflect:

✔ clean code  
✔ strong understanding  
✔ engineering discipline  
✔ progression over time  

---

### 🧠 Why I’m Doing This

Because Linux runs the world — and understanding the kernel means understanding **how computers really work.**

And it’s fun 😎
