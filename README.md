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


## 📦 STEP 3 — Add a Minimal Root Filesystem

You can create a basic initramfs using [BusyBox](https://busybox.net/) or your custom implementation of busybox:

        // Create minimal filesystem

            mkdir -p initramfs/{bin,sbin,etc,proc,sys,usr/bin,usr/sbin}
            cd initramfs

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

        // Create an `init` script in `initramfs`:

            cd ..
            touch initramfs/init
            chmod +x initramfs/init


## 🚀 STEP 3 — Run the Kernel with QEMU

Once built, run your kernel image using QEMU:

```bash
qemu-system-x86_64 -kernel arch/x86/boot/bzImage
```

> This boots the kernel directly without a root filesystem.

---



**Sample `init` file content:**

```sh
#!/bin/sh
mount -t proc none /proc
mount -t sysfs none /sys
echo "Welcome to Minimal-OS!"
exec /bin/sh
```

Pack initramfs:

```bash
cd initramfs
find . -print0 | cpio --null -ov --format=newc | gzip -9 > ../initramfs.cpio.gz
```

---

## 💻 STEP 5 — Boot Kernel with Initramfs

Run the kernel with your initramfs:

```bash
qemu-system-x86_64 -kernel arch/x86/boot/bzImage -initrd ../initramfs.cpio.gz -nographic -append "console=ttyS0"
```

> This will boot your Minimal-OS and drop you into a shell inside QEMU.

---

## 📚 Files and Structure

| Path                    | Purpose                       |
| ----------------------- | ----------------------------- |
| `linux/`                | Kernel source directory       |
| `initramfs/`            | Your root filesystem          |
| `initramfs/init`        | Init script run by the kernel |
| `initramfs.cpio.gz`     | Compressed rootfs image       |
| `arch/x86/boot/bzImage` | Compiled Linux kernel image   |

