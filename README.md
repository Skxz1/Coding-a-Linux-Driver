# Embedded Linux Driver Development — Learning Project

This repository tracks my journey into **Linux kernel and device driver development**, starting from a simple “Hello World” Loadable Kernel Module (LKM) and progressing toward real character-device drivers and kernel-space/user-space communication.

The focus of this project is not only to build working drivers, but to understand **how and why** they work — documenting key lessons along the way.

---

## 🎯 Goals

Through this project I aim to develop:

- a practical understanding of **Linux kernel internals**
- confidence writing and debugging **kernel modules**
- experience working safely inside **kernel space**
- knowledge of **device file interfaces (/dev/)** 
- professional engineering habits such as:
  - incremental development
  - testing in VMs
  - clear documentation
  - clean Git history & repo structure

---

## 🧭 Roadmap

### ✅ Phase 1 — Hello World Kernel Module (`1_hello/`)
- Implements `module_init()` and `module_exit()`
- Logs messages with `printk()` / `pr_info()`
- Built using kernel build system + Makefile
- Loaded via `insmod`, removed via `rmmod`
- Debug output viewed using `dmesg`

📄 Documented in: `1_hello/README.md`

---

### 🟡 Phase 2 — Character Device Driver
Planned:
- Register `/dev/mydevice`
- Implement `open`, `release`, `read`, `write`
- Exchange data between kernel & user space
- Provide test program

---

### 🟡 Phase 3 — Real-World Behaviour & Safety
Planned:
- Internal buffering
- IOCTL control paths
- Blocking I/O
- Concurrency + locking
- Multiple process interaction

---

### 🟡 Phase 4 — Extensions (Optional)
Possible exploration:
- sysfs attributes
- debug flags
- virtual interrupts
- embedded Linux / Raspberry Pi support

---

## 🛠 Tools & Environment

- Ubuntu Linux (inside VirtualBox VM)
- GCC / `make`
- Linux kernel headers
- `insmod`, `rmmod`, `lsmod`, `dmesg`
- Git + GitHub

All development is performed inside a **virtual machine** to avoid host system instability — since kernel-space bugs can crash the OS.

---

## 📂 Repository Structure

```
.
├── 1_hello/            # First kernel module
│   ├── hello.c
│   ├── Makefile
│   └── README.md
├── .gitignore          # excludes kernel build artefacts
└── README.md           # project overview (this file)
```

Common kernel build outputs (e.g., `.ko`, `.o`, `.mod.*`) are intentionally **not tracked** in Git.

---

## ▶ Building & Running Modules

Example (inside a module directory):

```bash
make
sudo insmod module.ko
sudo dmesg -w     # view kernel logs
sudo rmmod module
```

---

## 🧠 Learning Highlights So Far

- Kernel space vs user space
- How Loadable Kernel Modules work
- How the kernel build system compiles modules
- Using `printk()` and `dmesg` for debugging
- Best practices for ignoring build artefacts
- Working safely inside a VM
- Handling GitHub repos for systems projects

---

## 🛡 Safety Notes

Kernel code executes with **full system privilege**.  
Bugs can:

- freeze the system
- panic the kernel
- corrupt memory

Therefore:

✔ always develop inside a VM  
✔ make small, incremental changes  
✔ test carefully  
✔ avoid copying code blindly  

---

## 📌 Status

| Phase | Description | Status |
|------|-------------|--------|
| 1 | Hello World kernel module | ✅ Complete |
| 2 | Character device driver | 🟡 In progress |
| 3 | Advanced behaviour | ⏳ Planned |
| 4 | Extensions | ⏳ Planned |

---

## 💬 About This Repo

This project reflects a **learn-by-doing approach** — understanding each layer before moving on. The intent is to build lasting intuition rather than just compiling working code.

Feedback, discussion, and suggestions are always welcome 🙂
