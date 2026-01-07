# 01_hello — My First Linux Kernel Module 🧠🐧

A simple **Hello World Linux Kernel Module** created as part of my Linux driver learning journey.

This example can be compiled and run on a normal **x86 Ubuntu machine** or on **embedded Linux (e.g. Raspberry Pi)** — as long as kernel headers are installed.

---

## 🎯 What this project is about

This is my **first Loadable Kernel Module (LKM)**.  
The goal is to understand:

✔ how code can be loaded into the Linux kernel at runtime  
✔ how it can be safely removed again  
✔ how kernel logging works using `printk()` / `pr_info()`  
✔ how to use `dmesg` to view kernel output  

Unlike normal programs that run in **user space**, a kernel module runs in **kernel space**.  
That means:

⚠ it has full control  
⚠ there is no protection if something goes wrong  
⚠ testing should *always* be done in a VM  

Which is exactly what I’m doing 🙂

---

## 🧩 The Hello World Kernel Module — in simple terms

Linux kernel modules define **two key functions**:

### 🔹 When the module loads
```c
int hello_init(void)
```

Returning `0` = success.  
Returning `< 0` = error during load.

---

### 🔹 When the module unloads
```c
void hello_exit(void)
```

Used to clean up anything allocated during init.

---

### 🖨 Logging from the kernel

Kernel modules **don’t use `printf()`**.  
Instead they use:

```c
printk()
```

(or helpers like `pr_info()`)

These messages appear in the **kernel log**, which we read using:

```
dmesg
```

---

### 📜 Declaring the module license

```c
MODULE_LICENSE("GPL");
```

This declares the module as **open-source GPL licensed**, which avoids kernel warnings and allows the module to load on systems that block proprietary drivers.

---

## 🏗 Building the module

The build process uses the Linux kernel build system.

The Makefile includes:

```make
obj-m += hello.o
```

Then we call the kernel build environment inside:

```
/lib/modules/$(uname -r)/build
```

Running:

```
make
```

produces:

✔ `hello.ko` — the actual kernel module

---

## ▶ Running the module

### 📡 Follow kernel logs live
```bash
sudo dmesg -w
```

---

### ➕ Insert the module
```bash
sudo insmod hello.ko
```

You should see a log message appear 🎉

---

### ➖ Remove the module
```bash
sudo rmmod hello
```

Another log message confirms unload.

---

### 🔍 View recent logs
```bash
dmesg | tail -20
```

---

## 📁 Repository structure

```
1_hello/
 ├── hello.c        # Kernel module source
 ├── Makefile       # Build instructions
```

Build files like `.ko`, `.o`, `.mod.*` are ignored using `.gitignore`.

---

## 🧠 What I learned so far

✔ Kernel space vs user space  
✔ How Loadable Kernel Modules work  
✔ How to build kernel modules using Makefiles  
✔ Using `printk()` + `dmesg` for debugging  
✔ Safely loading/unloading modules  
✔ Why `.gitignore` should exclude build outputs  

---

## 🚧 Safety First

Kernel bugs can:

❌ freeze your system  
❌ cause panics  
❌ corrupt memory  

So this project is always tested inside a **virtual machine**.

---

## 🚀 Next Steps

This project is **Phase 1** of my Linux driver learning path.

Next goals:

🔜 build a **character device driver**  
🔜 create `/dev/mydevice`  
🔜 implement read/write  
🔜 explore IOCTL + blocking I/O  

And of course… keep documenting everything 📓  

---

### ⭐ Personal Note

This repo exists so I can **learn by doing — not copying blindly.**  
Progress over perfection 🙂
