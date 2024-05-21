#define FUSE_USE_VERSION 28
#define _XOPEN_SOURCE 500
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

static const char *dirpath = "/home/rrayyaann/sisop/percobaan/m4s3/relics";

static int archeology_getattr(const char *path, struct stat *stbuf)
{
    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);
    int res = lstat(fpath, stbuf);
    if (res == -1)
        return -errno;
    return 0;
}

static int archeology_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);

    DIR *dp;
    struct dirent *de;

    dp = opendir(fpath);
    if (dp == NULL)
        return -errno;

    while ((de = readdir(dp)) != NULL) {
        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
        if (filler(buf, de->d_name, &st, 0))
            break;
    }

    closedir(dp);
    return 0;
}

static int archeology_open(const char *path, struct fuse_file_info *fi)
{
    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);

    int res = open(fpath, fi->flags);
    if (res == -1)
        return -errno;

    fi->fh = res;
    return 0;
}

static int archeology_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    int fd = fi->fh;
    int res = pread(fd, buf, size, offset);
    if (res == -1)
        res = -errno;

    return res;
}

static int archeology_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);

    int fd = open(fpath, O_WRONLY);
    if (fd == -1)
        return -errno;

    int res = pwrite(fd, buf, size, offset);
    if (res == -1)
        res = -errno;

    close(fd);

    if (res != -1) {
        char split_command[2048];
        sprintf(split_command, "split -b 10k -d --additional-suffix=.part %s %s.part", fpath, fpath);
        system(split_command);
        unlink(fpath);  
    }

    return res;
}

static int archeology_unlink(const char *path)
{
    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);

    char rm_command[1024];
    sprintf(rm_command, "rm %s.part*", fpath);
    system(rm_command);

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

int main(int argc, char *argv[])
{
    umask(0);
    return fuse_main(argc, argv, &archeology_oper, NULL);
}
