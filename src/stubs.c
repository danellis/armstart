#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include "debug.h"

void *_sbrk(intptr_t increment) {
    return 0;
}

ssize_t _write(int fd, const void *buf, size_t count) {
    for (int c = 0; c < count; ++c) {
        debug_putc(((char *) buf)[c]);
    }
    return count;
}

ssize_t _read(int fd, void *buf, size_t count) {
    errno = EBADF;
    return -1;
}

off_t _lseek(int fd, off_t offset, int whence) {
    errno = EBADF;
    return -1;
}

int _close(int fd) {
    return 0;
}

int _fstat(int fd, struct stat *buf) {
    errno = EBADF;
    return -1;
}

int _isatty(int fd) {
    return 1;
}
