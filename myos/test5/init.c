#include <unistd.h>
#include <fcntl.h>
#include <sys/mount.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

#define _PATH_PROC "/proc"
#define _PATH_SYS "/sys"

void mount_filesystems() {
    // Mount /proc
    if (mount("proc", _PATH_PROC, "proc", 0, NULL) < 0) {
        perror("Failed to mount /proc");
        _exit(1);
    }

    // Mount /sys
    if (mount("sysfs", _PATH_SYS, "sysfs", 0, NULL) < 0) {
        perror("Failed to mount /sys");
        _exit(1);
    }
}

void start_busybox() {
    // Execute BusyBox as the shell or init system
    const char *busybox = "/bin/busybox";
    char *const args[] = { (char *)busybox, "sh", NULL };
    
    execv(busybox, args);  // Load BusyBox shell
    perror("Failed to execute BusyBox");
    _exit(1);  // Exit if BusyBox cannot be started
}

int main() {

    printf("\033[H\033[J");
    // Mount essential filesystems
    mount_filesystems();
    
    // Now run BusyBox shell as init
    start_busybox();
    
    return 0;
}
