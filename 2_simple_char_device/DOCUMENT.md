PHASE 2 — DOCUMENT.md (DETAILED TIMELINE + LEARNING LOG)

Purpose of this document:
This file is a timeline-style record of how Phase 2 was built, what was learned at
each step, what broke, how it was debugged, and how understanding improved
through the process.

Phase 2 theme:
“Make the kernel treat my code like a real device behind /dev.”

====================================================================

PHASE 2 GOAL (WHAT “DONE” MEANS)

By the end of Phase 2, the project should prove ALL of the following:

1) The kernel has a registered character device owned by our module
2) A device node exists under /dev (manual creation is fine for this phase)
3) User space can open the device, the kernel calls our callbacks, and we can
   see it in the kernel log
4) A minimal read path exists (EOF read) so cat can interact without errors
5) The module loads/unloads cleanly and logs clearly using a unique prefix

IMPORTANT:
Phase 2 is NOT about real data transfer or buffers yet.
Phase 2 is about wiring: numbers → VFS → driver callbacks.

====================================================================

PROJECT STRUCTURE DECISION (HOW WE ORGANIZED IT)

We decided each phase is its own folder to keep learning incremental and clean:

- One folder = one milestone
- Each folder has:
  - code (simple_char.c)
  - Makefile
  - README.md
  - DOCUMENT.md (this file)

This is portfolio-friendly because it shows progression and discipline.

Folder for this phase:
2_simple_char_device/

====================================================================

TIMELINE OF THE IMPLEMENTATION (STEP BY STEP)

--------------------------------------------------------------------
T0 — STARTING POINT (WHAT WE ALREADY KNEW FROM PHASE 1)

From Phase 1, we already understood:

- how to build a kernel module (.ko) using the kernel build system
- insmod loads modules silently when successful
- rmmod removes modules
- printk/pr_info logs show up in dmesg
- kernel development is noisy; dmesg is the primary feedback channel

So Phase 2 could focus on device registration rather than “how to compile.”

--------------------------------------------------------------------
T1 — CONCEPT CHECK (THE MENTAL MODEL BEFORE CODING)

Before writing code, we answered 3 core questions:

1) Why can’t the kernel just use /dev/mydevice directly?
   - Because /dev is a user-space naming layer.
   - The kernel identifies devices by (major, minor) numbers.

2) Why does open() live in the driver?
   - Because open() is a kernel syscall path.
   - The VFS dispatches file operations to the driver in kernel space.

3) Why is read() optional at first?
   - To validate the registration + lifecycle first.
   - Data paths add complexity (buffers, copy_to_user, EOF, concurrency).

This concept check was important because it prevented “copy-paste coding.”
It gave us a target mental model:

User open("/dev/mydevice") -> VFS -> device number -> file_operations -> driver

--------------------------------------------------------------------
T2 — SCAFFOLDING THE PROJECT (FOLDER + FILES)

We created the Phase 2 folder and prepared the core files:

- 2_simple_char_device/simple_char.c
- 2_simple_char_device/Makefile
- 2_simple_char_device/README.md
- 2_simple_char_device/DOCUMENT.md

Learning at this step:
- kernel projects benefit from structure
- documentation should be written as we go (not at the end)

--------------------------------------------------------------------
T3 — FIRST IMPLEMENTATION: REGISTER A CHAR DEVICE (THE “WIRING”)

We implemented the minimal “device registration driver”:

Key components added:

1) dev_t mydev
   - stores major/minor packed together

2) struct cdev my_cdev
   - kernel object that binds a device number to file_operations

3) struct file_operations my_fops
   - function pointer table for open/release (and later read)

4) alloc_chrdev_region(&mydev, 0, 1, "mydevice")
   - kernel assigns a major number dynamically
   - minor starts at 0
   - we log MAJOR(mydev), MINOR(mydev)

5) cdev_init(&my_cdev, &my_fops)
6) cdev_add(&my_cdev, mydev, 1)
   - makes the device “live” in the kernel

7) module_init / module_exit
   - correct cleanup: cdev_del + unregister_chrdev_region

What we learned here:
- device files are not “magic”; the kernel needs a (major, minor) registration
- cdev is the bridge between device numbers and driver callbacks
- logging major/minor is essential for debugging /dev mapping

--------------------------------------------------------------------
T4 — FIRST DEBUGGING LESSON: dmesg IS NOISY (FILTERING)

We ran dmesg -w and it printed too much system noise (VM drivers, graphics,
clipboard, etc.). We learned quickly that:

- The kernel log is global
- Without filtering, your driver messages get lost

Fix:
Use a unique prefix in log messages (“mydevice: ...”) and filter:

sudo dmesg | grep mydevice | tail -20

We also hit a security restriction:

dmesg: read kernel buffer failed: Operation not permitted

Meaning:
- On modern Ubuntu, reading dmesg requires sudo

Correct fix:
sudo dmesg | grep mydevice | tail -20

Learning at this step:
- kernel dev requires log hygiene
- “no output” doesn’t mean failure; it often means “wrong filter / wrong privilege”

--------------------------------------------------------------------
T5 — MODULE LOAD SUCCESS (THE FIRST REAL CHECKPOINT)

We inserted the module:

sudo insmod simple_char.ko

Then verified logs:

mydevice: allocated major=240 minor=0
mydevice: module loaded

Checkpoint achieved:
- registration works
- device numbers exist
- kernel accepted our cdev

Learning at this step:
- insmod success is usually silent
- dmesg is the “real output” of kernel-space work

--------------------------------------------------------------------
T6 — MANUAL /dev NODE CREATION (MAKING THE MAPPING REAL)

Even though the module registered, /dev/mydevice didn’t automatically appear.
That was expected in Phase 2.

So we manually created the device node:

sudo mknod /dev/mydevice c 240 0
sudo chmod 666 /dev/mydevice

Learning at this step:
- /dev is just a filesystem node that stores (major, minor)
- “c” means character device
- permissions matter for user-space testing

Important clarification:
Not having /dev appear automatically does NOT mean the driver failed.
It means we haven’t implemented the class/device + udev path yet.

--------------------------------------------------------------------
T7 — FIRST INTERACTION FAIL: “Invalid argument” (WHY IT HAPPENED)

We tested with:

cat /dev/mydevice > /dev/null

It returned:
cat: /dev/mydevice: Invalid argument

This was an expected and important failure.

Reason:
- cat calls open() then read()
- we had open/release but no read callback
- therefore the kernel returned an error on read

Learning at this step:
- user programs like cat are “read-driven”
- open() alone is not enough to interact with file-like tools
- missing file_operations entries produce real user-space errors

This failure proved the wiring was correct (open happened), but the interface
was incomplete for cat.

--------------------------------------------------------------------
T8 — ADDING A MINIMAL EOF READ (AN “EXTRA EXPERIMENT” INSIDE PHASE 2)

We decided to keep Phase 2 simple while improving testing:

Add read() that returns EOF immediately:

- returning 0 from read() means End-Of-File
- cat stops cleanly when it gets 0 bytes

This is still Phase 2 style because it avoids buffers and copy_to_user complexity.

--------------------------------------------------------------------
T9 — BUILD ERROR: my_read UNDECLARED (REAL C COMPILER LESSON)

When adding .read = my_read, the build failed:

error: 'my_read' undeclared here (not in a function)

Cause:
- C compiles top-to-bottom
- file_operations referenced my_read before the compiler saw its definition
- the function existed later, but there was no prototype

Fix options:
- Move my_read above my_fops OR
- Add a forward declaration:

static ssize_t my_read(struct file *file, char __user *buf, size_t len, loff_t *offset);

We applied the forward declaration solution.

Learning at this step:
- kernel development is still C development
- many “kernel bugs” are actually normal C compilation rules
- prototypes matter when using function pointers

--------------------------------------------------------------------
T10 — VERIFIED SUCCESS: open(), read(), release() ALL CALLED

After rebuild + reload, we tested again:

cat /dev/mydevice > /dev/null

Kernel log showed:

mydevice: open()
mydevice: read(131072)
mydevice: release()

Major learning moment:
- cat requested 131072 bytes (128 KiB) in a single read call
- read(len) means “user requested this many bytes”
- driver controls how many bytes it actually returns
- returning 0 is the clean EOF signal

Checkpoint achieved:
- VFS routing works
- /dev mapping works
- file_operations table is wired correctly
- read path exists (EOF-only)
- logging confirms kernel/user interaction end-to-end

--------------------------------------------------------------------
T11 — OPTIONAL EXTENSION (OPTION A): AUTO-CREATE /dev (PLANNED EXPERIMENT)

We decided to treat automatic /dev creation as an additional experiment for Phase 2:

Goal:
- Use class_create + device_create
- Trigger udev to create /dev/mydevice automatically
- Learn /sys/class role

Reason for deferring:
- manual mknod taught the major/minor mapping clearly first
- automation is easier once the fundamentals are understood

This experiment will be added later as a Phase 2 extension.

====================================================================

PROGRESS CHECKPOINTS (WHAT WE CAN PROVE NOW)

- Module loads successfully (insmod)
- Module unloads cleanly (rmmod)
- alloc_chrdev_region works and prints major/minor
- /dev node created manually using mknod with correct major/minor
- open/release are called from user-space access
- read is called by cat and returns EOF correctly
- dmesg filtering is used correctly to isolate driver logs
- we debugged both kernel-level errors and pure C compile errors

====================================================================

WHAT WE NEEDED TO LEARN AT EACH STEP (SUMMARY)

- Before coding:
  - kernel uses major/minor numbers, not filenames
  - VFS dispatch flow: open -> driver callback

- During registration:
  - alloc_chrdev_region, dev_t, MAJOR/MINOR macros
  - cdev_init and cdev_add glue device numbers to file_operations

- During testing:
  - dmesg permission restrictions (sudo needed)
  - why dmesg is noisy and how to filter it

- During /dev creation:
  - what mknod does and why /dev isn't automatic yet

- During debugging:
  - cat triggers read; missing read leads to errors
  - adding read means respecting C compilation rules (prototypes)

====================================================================

NEXT STEPS (PHASE 3 READY)

Phase 3 will introduce real data flow:

- write(): accept data from user space
- read(): return data back to user space
- internal buffer management
- copy_to_user / copy_from_user
- error handling and edge cases

Optional (Phase 2 extension):
- class_create/device_create to auto-create /dev/mydevice without manual mknod

====================================================================

PHASE STATUS

Phase 2 is complete.

We achieved the phase goal: a registered character device that can be accessed
from user space via /dev and confirmed via kernel logs, including a minimal EOF
read path for clean testing.
