PHASE 2 — SIMPLE CHARACTER DEVICE (VFS, open, read, release)

This phase builds directly on Phase 1 by introducing a minimal Linux character
device driver. The focus is on understanding how a device is registered with
the kernel and how user-space programs interact with kernel code through the
Virtual File System (VFS).

No real hardware is involved yet. This phase is about control flow, not data.

====================================================================

PHASE GOALS

By completing this phase, the driver is able to:

- Register a character device with the Linux kernel
- Dynamically allocate a major and minor device number
- Expose a /dev/mydevice interface
- Handle open() and release() system calls
- Implement a minimal read() that returns EOF
- Log kernel activity using pr_info() and inspect it with dmesg

This establishes a solid foundation for real data transfer in later phases.

====================================================================

WHAT THIS DRIVER DOES

On module load:
- The driver registers itself as a character device
- The kernel assigns a major and minor number
- The driver logs the assigned numbers using dmesg

On user interaction:
- open("/dev/mydevice") triggers the driver’s open() callback
- read() is called and immediately returns 0 (EOF)
- release() is called when the file descriptor is closed

This confirms correct VFS → driver → kernel interaction.

====================================================================

FILES IN THIS DIRECTORY

- simple_char.c
  Minimal character device driver implementation

- Makefile
  Kernel module build configuration

- README.md
  Phase documentation

All build artifacts (*.o, *.ko, *.mod.*, Module.symvers, etc.)
are excluded via .gitignore.

====================================================================

BUILD AND RUN INSTRUCTIONS

Build the module:
    make

Load the module:
    sudo insmod simple_char.ko

Check kernel logs:
    sudo dmesg | grep mydevice

Expected output includes:
- allocated major/minor numbers
- module loaded message

====================================================================

CREATING THE DEVICE NODE (MANUAL)

In this phase, the device node is created manually to reinforce understanding
of how /dev maps to kernel drivers.

    sudo mknod /dev/mydevice c <major> <minor>
    sudo chmod 666 /dev/mydevice

The major and minor numbers are printed in dmesg when the module loads.

Automatic device creation via udev is intentionally deferred.

====================================================================

TESTING THE DEVICE

    cat /dev/mydevice > /dev/null

This causes the following sequence inside the kernel:
- open()
- read() (returns EOF)
- release()

Verify with:
    sudo dmesg | grep mydevice

====================================================================

ADDITIONAL EXPERIMENT — EOF READ

As an extension to this phase, a minimal read() implementation was added that
returns 0, signaling End-Of-File (EOF).

This allows tools like cat to interact with the device without errors while
avoiding buffer management or data copying logic.

This confirms correct wiring of the read() callback without introducing
unnecessary complexity.

====================================================================

KEY CONCEPTS LEARNED

- The kernel identifies devices using major/minor numbers, not filenames
- /dev is a user-space abstraction layered on top of kernel device numbers
- The Virtual File System (VFS) routes system calls to drivers
- Device drivers can be developed incrementally
- dmesg is essential for kernel-space debugging

====================================================================

PHASE STATUS

Phase 2 complete.

The driver now registers correctly, responds to user-space system calls,
and provides a stable base for implementing real data transfer in Phase 3.
