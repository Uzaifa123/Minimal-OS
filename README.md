# Minimal-OS



## ✅ STEP 1 — Set Up the Build Environment

Set up the build environment and install all necessary libraries on your Linux system.
You can use the following command to install all the tools:


        sudo apt install -y \
        build-essential \
        flex \
        bison \
        libncurses-dev \
        libelf-dev \
        libssl-dev \
        bc \
        qemu-system-x86 \
        gcc \
        make \
        git \
        wget \
        curl \
        libudev-dev \
        libpci-dev \
        libzstd-dev \
        cpio \
        zstd



## 🛠️ STEP 2 — Build the Linux Kernel

1. **Clone the Linux kernel source code:**

        git clone https://github.com/torvalds/linux.git
        
        cd linux


2. **Configure the Kernel:**

    Use a default configuration to start with:

        make defconfig

    (Optional) Launch a menu to customize configuration:

        make menuconfig


3. **Build the Kernel:**

    This will take a while and produce a `bzImage` file in `arch/x86/boot/`.

        make -j$(nproc)


## 📦 STEP 3 — Create initramfs

Create a minimalistic file system and package it into an initramfs archive. This is a temporary root filesystem that is loaded into memory by the kernel at boot time. It contains the /init program, which is the first user-space application run by the kernel.

    // Add a Minimal Root Filesystem

        mkdir -p initramfs/{bin,sbin,etc,proc,sys,usr/bin,usr/sbin}
        cd initramfs


    // Create an `init` script inside the `initramfs`. 
    // This script is the first userspace program executed by the kernel after booting.
    // 
    // Note: This file is very important. Its purpose is to mount necessary filesystems,
    // set up the environment, and launch a shell or other init process.
    // 
    // If you're writing `init` in C (instead of a shell script), 
    // it should be compiled statically because, at this stage,
    // no dynamic C runtime (like glibc or musl) is available.
    // The `init` program is responsible for setting up the runtime environment
    // for the rest of the system.


    // Create init.c file 
    // Use the following code sinppent as minimal init.c

        #include <stdio.h>
        #include <unistd.h>
        #include <sys/mount.h>

        int main() {
            printf("Minimal-OS init started!\n");

            // Mount virtual filesystems
            mount("none", "/proc", "proc", 0, "");
            mount("none", "/sys", "sysfs", 0, "");

            // Launch BusyBox shell
            execl("/bin/sh", "sh", NULL);

            // If execl fails
            perror("execl failed");
            return 1;
        }

        gcc -static -o init init.c

    // Copy it to your initramfs directory and make it executable

        cp init initramfs
        chmod +x initramfs/init

    // Compile and install BusyBox

        wget https://busybox.net/downloads/busybox-1.36.1.tar.bz2
        tar -xjf busybox-1.36.1.tar.bz2
        cd busybox-1.36.1
        make defconfig
        make -j$(nproc)
    
    // Create busybox nodes

        ln -s /bin/busybox sh
        ln -s /bin/busybox mount
        ln -s /bin/busybox ls
        ln -s /bin/busybox cat
        ln -s /bin/busybox echo
        ln -s /bin/busybox umount

    // Create initrd.img

       cd initrd
       find . | cpio -H newc -o > ../initrd.img

## 📦 STEP 4 — Add a Main Root Filesystem

Now we will build the main root filesystem, which lives on disk (or in a virtual disk image like ext4). This rootfs will contain:

The Linux kernel
The boot/ directory
GRUB bootloader and configuration
A minimal Linux userland (via BusyBox)

This filesystem will be mounted and used by the system after booting, unlike initramfs which is a temporary RAM-based system.

    // Create Root filesystem

        mkdir -p rootfs/{boot/grub,bin,sbin,etc,proc,sys,dev,usr/bin,usr/sbin,tmp,var,home}


    // Add the Linux Kernel and Initramfs

        Copy your compiled kernel (bzImage) and initramfs.cpio.gz into the /boot directory:

        cp path/to/bzImage rootfs/boot/vmlinuz
        cp path/to/initram.img rootfs/boot/initram.img


    // Install and Configure GRUB

    // Create a GRUB configuration file:
       
       nano rootfs/boot/grub/grub.cfg

    // Add this content:

        set timeout=5
        set default=0

        menuentry "minimal os" {

            set root=(cd,gpt3)
            linux /boot/bzimage root=/dev/sr0 init=init 
            initrd /boot/initrd.img
            boot
        }


    // Install GRUB on the disk (or image):

        If using a disk image (e.g., rootfs.img), use:

        sudo grub-install --root-directory=rootfs --target=i386-pc --boot-directory=rootfs/boot --modules="normal part_msdos ext2 multiboot" /dev/sdX
        Replace /dev/sdX with your real or loop device.


    // Compile and install BusyBox

        wget https://busybox.net/downloads/busybox-1.36.1.tar.bz2
        tar -xjf busybox-1.36.1.tar.bz2
        cd busybox-1.36.1
        make defconfig
        make -j$(nproc)
    
    // CREATE BUSYBOX NODES

        ln -s /bin/busybox sh
        ln -s /bin/busybox mount
        ln -s /bin/busybox ls
        ln -s /bin/busybox cat
        ln -s /bin/busybox echo
        ln -s /bin/busybox umount

    // Create /sbin/init
    // BusyBox provides an init, or you can create your own:

        ln -s /bin/busybox rootfs/sbin/init

    // Alternatively, write your own init.c, compile it statically, and place it at /sbin/init.


### INCOMPLETE
