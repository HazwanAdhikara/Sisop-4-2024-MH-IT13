#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

#define PART_SIZE 10240  // 10 KB
#define MAX_PATH 1000

static const char *dirpath = "/home/rrayyaann/sisop/percobaan/m4s3/relics";

static int archeology_getattr(const char *path, struct stat *stbuf) {
    char fpath[MAX_PATH];
    snprintf(fpath, sizeof(fpath), "%s%s", dirpath, path);
    int res = lstat(fpath, stbuf);
    if (res == -1)
        return -errno;
    return 0;
}

static int archeology_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
    char fpath[MAX_PATH];
    snprintf(fpath, sizeof(fpath), "%s%s", dirpath, path);

    DIR *dp = opendir(fpath);
    if (!dp)
        return -errno;

    struct dirent *de;
    while ((de = readdir(dp)) != NULL) {
        struct stat st = {0};
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
        if (filler(buf, de->d_name, &st, 0))
            break;
    }

    closedir(dp);
    return 0;
}

static int archeology_open(const char *path, struct fuse_file_info *fi) {
    char fpath[MAX_PATH];
    snprintf(fpath, sizeof(fpath), "%s%s", dirpath, path);

    int res = open(fpath, fi->flags);
    if (res == -1)
        return -errno;

    fi->fh = res;
    return 0;
}

static int archeology_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
    char fpath[MAX_PATH];
    snprintf(fpath, sizeof(fpath), "%s%s", dirpath, path);

    size_t total_bytes_read = 0;
    size_t part_size = PART_SIZE;
    int part_number = offset / part_size;

    while (size > 0) {
        char part_file[MAX_PATH];
        snprintf(part_file, sizeof(part_file), "%s.part%03d", fpath, part_number);

        int fd = open(part_file, O_RDONLY);
        if (fd == -1)
            break;

        size_t part_offset = offset % part_size;
        size_t bytes_to_read = part_size - part_offset < size ? part_size - part_offset : size;
        ssize_t bytes_read = pread(fd, buf + total_bytes_read, bytes_to_read, part_offset);
        if (bytes_read == -1) {
            close(fd);
            return -errno;
        }

        total_bytes_read += bytes_read;
        size -= bytes_read;
        offset += bytes_read;
        part_number++;
        close(fd);
    }

    return total_bytes_read;
}

static int archeology_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
    char fpath[MAX_PATH];
    snprintf(fpath, sizeof(fpath), "%s%s", dirpath, path);

    size_t part_size = PART_SIZE;
    int part_number = offset / part_size;
    size_t total_bytes_written = 0;

    while (size > 0) {
        char part_file[MAX_PATH];
        snprintf(part_file, sizeof(part_file), "%s.part%03d", fpath, part_number);

        size_t part_offset = offset % part_size;
        size_t bytes_to_write = part_size - part_offset < size ? part_size - part_offset : size;
        int fd = open(part_file, O_WRONLY | O_CREAT, 0644);
        if (fd == -1)
            return -errno;

        ssize_t bytes_written = pwrite(fd, buf + total_bytes_written, bytes_to_write, part_offset);
        if (bytes_written == -1) {
            close(fd);
            return -errno;
        }

        total_bytes_written += bytes_written;
        size -= bytes_written;
        offset += bytes_written;
        part_number++;
        close(fd);
    }

    return total_bytes_written;
}

static int archeology_unlink(const char *path) {
    char fpath[MAX_PATH];
    snprintf(fpath, sizeof(fpath), "%s%s", dirpath, path);

    int res = 0;
    char part_file[MAX_PATH];
    for (int i = 0;; i++) {
        snprintf(part_file, sizeof(part_file), "%s.part%03d", fpath, i);
        res = unlink(part_file);
        if (res == -1) {
            if (errno == ENOENT)
                break;
            else
                return -errno;
        }
    }

    return 0;
}

static struct fuse_operations archeology_oper = {
    .getattr = archeology_getattr,
    .readdir = archeology_readdir,
    .open = archeology_open,
    .read = archeology_read,
    .write = archeology_write,
    .unlink = archeology_unlink,
};

int main(int argc, char *argv[]) {
    umask(0);
    return fuse_main(argc, argv, &archeology_oper, NULL);
}
