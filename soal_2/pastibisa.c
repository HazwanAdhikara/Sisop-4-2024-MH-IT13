#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <ctype.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <time.h>

static const char *dirpath = "/home/zwaneee/sisop/modul4/sensitif";
static int authenticated = 0;

void logMessage(const char *status, const char *tag, const char *info)
{
    time_t rawtime;
    struct tm *info_time;
    char buffer[80];

    time(&rawtime);
    info_time = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%d/%m/%Y-%H:%M:%S", info_time);

    FILE *log_file = fopen("/home/zwaneee/sisop/modul4/sensitif/logs-fuse.log", "a");
    if (log_file != NULL)
    {
        fprintf(log_file, "[%s]::%s::[%s]::[%s]\n", status, buffer, tag, info);
        fclose(log_file);
    }
}

void logSuccess(const char *tag, const char *info)
{
    logMessage("SUCCESS", tag, info);
}

void logFailure(const char *tag, const char *info)
{
    logMessage("FAILED", tag, info);
}
void authenticate_user()
{
    char password[100];
    printf("KODENYA APA DULU NGAB: ");
    scanf("%s", password);

    if (strcmp(password, "sisopmudah") == 0)
    {
        authenticated = 1;
        printf("SILAHKAN NGAB!\n");
    }
    else
    {
        printf("SALAH BRO, PUTAR BALIK DIDEPAN!\n");
    }
}

int check_authentication()
{
    return authenticated;
}

void decode_base64(const char *input, char *output)
{
    BIO *bio, *b64;
    int input_length = strlen(input);

    bio = BIO_new_mem_buf((void *)input, input_length);
    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    bio = BIO_push(b64, bio);

    int decoded_length = BIO_read(bio, output, input_length);
    output[decoded_length] = '\0';

    BIO_free_all(bio);
}

void decode_rot13(const char *input, char *output)
{
    for (int i = 0; input[i] != '\0'; i++)
    {
        char c = input[i];
        if ('a' <= c && c <= 'z')
        {
            c = ((c - 'a' + 13) % 26) + 'a';
        }
        else if ('A' <= c && c <= 'Z')
        {
            c = ((c - 'A' + 13) % 26) + 'A';
        }
        output[i] = c;
    }
    output[strlen(input)] = '\0';
}

void decode_hex(char *buf, size_t size)
{
    size_t decoded_size = size / 2;
    char *decoded_text = (char *)malloc(decoded_size + 1);
    if (decoded_text == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    for (int i = 0, j = 0; i < size; i += 2, j++)
    {
        char hex[3] = {buf[i], buf[i + 1], '\0'};
        decoded_text[j] = strtol(hex, NULL, 16);
    }

    decoded_text[decoded_size] = '\0';
    memcpy(buf, decoded_text, decoded_size);
    buf[decoded_size] = '\0';
    free(decoded_text);
}

void decode_rev(char *str)
{
    int length = strlen(str);
    for (int i = 0; i < length / 2; i++)
    {
        char temp = str[i];
        str[i] = str[length - i - 1];
        str[length - i - 1] = temp;
    }
}

static int decode_file(const char *path, char *buf, size_t size)
{
    if (strstr(path, "base64") != NULL)
    {
        decode_base64(buf, buf);
    }
    else if (strstr(path, "rot13") != NULL)
    {
        decode_rot13(buf, buf);
    }
    else if (strstr(path, "hex") != NULL)
    {
        decode_hex(buf, size);
    }
    else if (strstr(path, "rev") != NULL)
    {
        decode_rev(buf);
    }

    return strlen(buf);
}

static int xmp_getattr(const char *path, struct stat *stbuf)
{
    int res;
    char fpath[1000];

    sprintf(fpath, "%s%s", dirpath, path);

    res = lstat(fpath, stbuf);

    if (res == -1)
        return -errno;

    return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];

    if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
    {
        sprintf(fpath, "%s%s", dirpath, path);
    }
    if (strstr(fpath, "rahasia") != NULL && !check_authentication())
    {
        authenticate_user();
        if (!check_authentication())
        {
            printf("Butuh Password, Sorry.\n");
            return -EACCES;
        }
    }

    DIR *dp;
    struct dirent *de;

    dp = opendir(fpath);
    if (dp == NULL)
        return -errno;

    int res = 0;

    while ((de = readdir(dp)) != NULL)
    {
        struct stat st;
        memset(&st, 0, sizeof(st));

        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;

        if (filler(buf, de->d_name, &st, 0) != 0)
        {
            res = -ENOMEM;
            break;
        }
    }

    closedir(dp);

    return res;
}

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
    int res;
    char fpath[1000];

    sprintf(fpath, "%s%s", dirpath, path);
    res = open(fpath, fi->flags);
    if (res == -1)
        return -errno;

    close(res);
    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);

    int fd = open(fpath, O_RDONLY);
    if (fd == -1)
    {
        logFailure("openFile", "Failed to open file");
        return -errno;
    }

    char read_buf[4096];
    memset(read_buf, 0, sizeof(read_buf));

    int res = pread(fd, read_buf, sizeof(read_buf) - 1, 0);
    if (res == -1)
    {
        logFailure("readFile", "Failed to read file");
        close(fd);
        return -errno;
    }
    else
    {
        logSuccess("readFile", "File read successfully");
    }

    close(fd);

    // Dekripsi isi file jika diperlukan
    if (strstr(fpath, "base64") != NULL || strstr(fpath, "rot13") != NULL || strstr(fpath, "hex") != NULL || strstr(fpath, "rev") != NULL)
    {
        res = decode_file(path, read_buf, sizeof(read_buf));
        if (res == -1)
        {
            logFailure("decodeFile", "Failed to decode file");
            return -errno;
        }
        else
        {
            logSuccess("decodeFile", "File decoded successfully");
        }
    }

    size_t read_len = strlen(read_buf);
    if (offset < read_len)
    {
        if (offset + size > read_len)
            size = read_len - offset;
        memcpy(buf, read_buf + offset, size);
    }
    else
    {
        size = 0;
    }

    return size;
}

static int xmp_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);

    int fd = open(fpath, O_WRONLY);
    if (fd == -1)
    {
        logFailure("openFile", "Failed to open file");
        return -errno;
    }

    int res = pwrite(fd, buf, size, offset);
    if (res == -1)
    {
        logFailure("writeFile", "Failed to write to file");
        close(fd);
        return -errno;
    }
    else
    {
        logSuccess("writeFile", "File written successfully");
    }

    close(fd);
    return res;
}

static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .open = xmp_open,
    .read = xmp_read,
    .write = xmp_write,
};

int main(int argc, char *argv[])
{
    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
}
