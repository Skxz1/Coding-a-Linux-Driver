PHASE 2 — CHARACTER DEVICE (LEARNING LOG & ENGINEERING NOTES)

This document records what I did during Phase 2, how I approached the work,
the challenges I ran into, and how my understanding of Linux character devices
developed through hands-on experimentation.

This phase was focused on understanding how the kernel sees devices and how
user-space programs interact with kernel code through the Virtual File System
(VFS).

====================================================================

INITIAL GOAL

The primary goal of Phase 2 was to build a minimal character device driver that:

- Registers with the Linux kernel
- Is reachable through /dev/mydevice
- Responds to basic file operations
- Does NOT yet transfer real data

At this stage, the device does not represent real hardware. It exists purely
to understand kernel registration, lifecycle management, and VFS routing.

====================================================================

CONCEPT CHECK

WHY CAN’T THE KERNEL JUST USE /dev/mydevice DIRECTLY WITHOUT NUMBERS?

The Linux kernel does not identify devices by filenames.

"/dev/mydevice" is only a user-space name and has no meaning to the kernel
by itself.

Internally, the kernel identifies devices using a device number composed of:

- Major number: identifies which driver handles the device
- Minor number: identifies which instance of that driver

When a user program calls:

    open("/dev/mydevice", O_RDONLY);

the kernel performs the following steps:

1. Looks up the inode for /dev/mydevice
2. Extracts the major and minor numbers from the inode
3. Finds the driver registered for that major number
4. Calls the driver’s file operation callbacks

Using numbers instead of filenames allows /dev to remain a flexible naming
layer. Device nodes can be renamed, removed, or recreated without affecting
the driver logic.

--------------------------------------------------------------------

WHY DOES open() LIVE IN THE DRIVER, NOT THE USER PROGRAM?

open() is a system call that executes inside the kernel.

When a user program calls open(), execution transitions from user space into
kernel space. The Virtual File System (VFS) then forwards the request to the
appropriate driver based on the major number.

open() lives in the driver because:

- Devices are owned and managed by the kernel
- Hardware access must be privileged
- Multiple processes may open the same device simultaneously
- Shared device state must be synchronised safely

Placing open() in the driver ensures that access control, state management,
and hardware interaction are handled securely and consistently.

--------------------------------------------------------------------

WHY IS IT USEFUL THAT read() IS OPTIONAL AT FIRST?

Making read() optional allows the driver to be developed incrementally.

The main goals of Phase 2 were:

- Registering a character device
- Allocating major and minor numbers
- Connecting the device to the VFS
- Verifying correct open() and release() behaviour

Implementing read() introduces additional complexity, including:

- Kernel memory buffers
- copy_to_user() logic
- EOF handling
- Partial reads
- Error handling
- Concurrency concerns

By deferring read(), it became easier to confirm that the device lifecycle
and registration logic worked correctly before adding data transfer features.

====================================================================

STEP-BY-STEP LEARNING PROCESS

STEP 1 — REGISTERING THE CHARACTER DEVICE

I used dynamic allocation to request a major number from the kernel rather
than hardcoding one. This avoided conflicts and reflected how real drivers
are written.

Seeing the allocated major and minor numbers printed in dmesg confirmed that
the kernel accepted the registration.

--------------------------------------------------------------------

STEP 2 — IMPLEMENTING open() AND release()

I implemented open() and release() callbacks and added logging using pr_info().

This allowed me to confirm that:

- /dev/mydevice was correctly mapped to my driver
- The VFS was invoking the correct callbacks
- The device lifecycle was functioning as expected

Watching open() and release() appear in dmesg was the first clear confirmation
of kernel ↔ user interaction.

--------------------------------------------------------------------

STEP 3 — MANUAL DEVICE NODE CREATION

Instead of automatically creating /dev/mydevice, I manually created it using
mknod.

This step was intentional and important because it made the relationship
between:

- major number
- minor number
- /dev entry
- driver

explicit and visible.

This removed the “magic” behind device nodes and clarified how /dev is only
a bridge between user space and the kernel.

--------------------------------------------------------------------

STEP 4 — ADDING A MINIMAL read() IMPLEMENTATION

Initially, attempting to read from the device resulted in an "Invalid argument"
error. This was expected because read() had not yet been implemented.

I then added a minimal read() implementation that simply returns 0.

Returning 0 signals End-Of-File (EOF) to user-space programs.

This allowed tools like cat to interact with the device cleanly without
introducing buffers or data transfer logic.

Seeing log output such as:

    read(131072)

revealed that user programs often request large amounts of data at once and
rely on the driver to control how much is actually returned.

====================================================================

CHALLENGES ENCOUNTERED

- Build failures caused by missing forward declarations
- Confusion when cat failed due to unimplemented read()
- Overwhelming dmesg output that required filtering
- Understanding why read() was being called with large sizes
- Learning to trust logs instead of assumptions

Each issue forced me to slow down, inspect kernel output, and reason about
what the kernel was actually doing rather than what I expected it to do.

====================================================================

PREDICT-BEFORE-YOU-RUN CHECK

Before loading the module, I answered the following:

1. When the module loads, what two numbers should it print?
2. Why does /dev/mydevice not appear automatically?

ANSWERS:

1. It prints a major number (which driver handles the device) and a minor
   number (which instance of that driver)

2. Because automatic device node creation requires a device class and udev
   integration, which was intentionally not implemented in this phase

Making predictions before running the code helped solidify understanding
and made debugging more systematic.

====================================================================

KEY TAKEAWAYS

- /dev filenames are a user-space abstraction
- The kernel identifies devices using major/minor numbers
- The VFS is the bridge between user programs and device drivers
- Drivers should be developed incrementally
- dmesg is the primary debugging tool for kernel development

====================================================================

PHASE STATUS

Phase 2 completed successfully.

The driver now registers correctly, responds to user-space system calls, and
provides a stable foundation for adding real data transfer and stateful
behaviour in Phase 3.
