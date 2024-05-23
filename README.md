# PRAKTIKUM SISOP-4-2024-MH-IT13

**KELOMPOK IT13**
| Nama                      | NRP        |
|---------------------------|------------|
|Muhamad Arrayyan              | 5027231014 |
|Hazwan Adhikara Nasution      | 5027231017 |
|Muhammad Andrean Rizq Prasetio     | 5027231052 |

## Pengantar
Laporan resmi ini dibuat terkait dengan praktikum modul 4 sistem operasi yang telah dilaksanakan pada tanggal 15 Mei 2024 hingga tanggal 25 Mei 2024. Praktikum modul 4 terdiri dari 3 soal yang dikerjakan oleh kelompok praktikan yang terdiri dari 3 orang selama waktu tertentu.

Kelompok IT13 melakukan pengerjaan modul 4 ini dengan pembagian sebagai berikut:
  + Soal 1 dikerjakan oleh Muhammad Andrean Rizq Prasetio
  + Soal 2 dikerjakan oleh Hazwan Adhikara Nasution
  + Soal 3 dikerjakan oleh Muhamad Arrayyan

Sehingga dengan demikian, Pembagian bobot pengerjaan soal menjadi (Rayyan 33.3%, Hazwan 33.3%, Andre 33.3%).

Kelompok IT13 juga telah menyelesaikan tugas praktikum modul 4 yang telah diberikan dan telah melakukan demonstrasi kepada Asisten lab. Dari hasil praktikum yang telah dilakukan sebelumnya, maka diperoleh hasil sebagaimana yang dituliskan pada setiap bab di bawah ini.

## Ketentuan
Struktur Repository Seperti Berikut:
```bash
—soal_1:
  	— inikaryakita.c
                                    
—soal_2:
  	— pastibisa.c
				
—soal_3:
  	— archeology.c
```
---

### **`Soal 1`**
`> Andre`

#### > Isi Soal
#### > Penyelesaian
#### > Penjelasan
#### > Dokumentasi
#### > Revisi

---

### **`Soal 2`**
`> Hazwan`

#### > Isi Soal

Masih dengan Ini Karya Kita, sang CEO ingin melakukan tes keamanan pada folder sensitif Ini Karya Kita. Karena Teknologi Informasi merupakan departemen dengan salah satu fokus di Cyber Security, maka dia kembali meminta bantuan mahasiswa Teknologi Informasi angkatan 2023 untuk menguji dan mengatur keamanan pada folder sensitif tersebut. Untuk mendapatkan folder sensitif itu, mahasiswa IT 23 harus kembali mengunjungi website Ini Karya Kita pada www.inikaryakita.id/schedule . Silahkan isi semua formnya, tapi pada form subject isi dengan nama kelompok_SISOP24 , ex: IT01_SISOP24 . Lalu untuk form Masukkan Pesanmu, ketik “Mau Foldernya” . Tunggu hingga 1x24 jam, maka folder sensitif tersebut akan dikirimkan melalui email kalian. Apabila folder tidak dikirimkan ke email kalian, maka hubungi sang CEO untuk meminta bantuan.  

- Pada folder **"pesan"** Adfi ingin meningkatkan kemampuan sistemnya dalam mengelola berkas-berkas teks dengan menggunakan **fuse**.
 > Jika sebuah file memiliki prefix *"base64,"* maka sistem akan langsung mendekode isi file tersebut dengan algoritma Base64.

 > Jika sebuah file memiliki prefix *"rot13,"* maka isi file tersebut akan langsung di-decode dengan algoritma ROT13.

 > Jika sebuah file memiliki prefix *"hex,"* maka isi file tersebut akan langsung di-decode dari representasi heksadesimalnya.

 > Jika sebuah file memiliki prefix *"rev,"* maka isi file tersebut akan langsung di-decode dengan cara membalikkan teksnya.

- Pada folder **“rahasia-berkas”**, Adfi dan timnya memutuskan untuk menerapkan kebijakan khusus. Mereka ingin memastikan bahwa folder dengan prefix "rahasia" tidak dapat diakses tanpa izin khusus. 
 > Jika seseorang ingin mengakses folder dan file pada “rahasia”, mereka harus memasukkan sebuah password terlebih dahulu (password bebas). 

- Setiap proses yang dilakukan akan tercatat pada logs-fuse.log dengan format :

  `[SUCCESS/FAILED]::dd/mm/yyyy-hh:mm:ss::[tag]::[information]`
  
Ex:
[SUCCESS]::01/11/2023-10:43:43::[moveFile]::[File moved successfully]

Catatan: 
- Tolong cek email kalian secara berkala untuk mendapatkan folder sensitifnya
- Jika ada pertanyaan, bisa menghubungi Adfi via WA aja, nanti dikasih hint

#### > Penyelesaian
`pastibisa.c`

```bash
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

void log_message(const char *status, const char *tag, const char *info)
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

void log_success(const char *tag, const char *info)
{
    log_message("SUCCESS", tag, info);
}

void log_failure(const char *tag, const char *info)
{
    log_message("FAILED", tag, info);
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
        log_failure("openFile", "Failed to open file");
        return -errno;
    }

    char read_buf[4096];
    memset(read_buf, 0, sizeof(read_buf));

    int res = pread(fd, read_buf, sizeof(read_buf) - 1, 0);
    if (res == -1)
    {
        log_failure("readFile", "Failed to read file");
        close(fd);
        return -errno;
    }
    else
    {
        log_success("readFile", "File read successfully");
    }

    close(fd);

    // Dekripsi isi file jika diperlukan
    if (strstr(fpath, "base64") != NULL || strstr(fpath, "rot13") != NULL || strstr(fpath, "hex") != NULL || strstr(fpath, "rev") != NULL)
    {
        res = decode_file(path, read_buf, sizeof(read_buf));
        if (res == -1)
        {
            log_failure("decodeFile", "Failed to decode file");
            return -errno;
        }
        else
        {
            log_success("decodeFile", "File decoded successfully");
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
        log_failure("openFile", "Failed to open file");
        return -errno;
    }

    int res = pwrite(fd, buf, size, offset);
    if (res == -1)
    {
        log_failure("writeFile", "Failed to write to file");
        close(fd);
        return -errno;
    }
    else
    {
        log_success("writeFile", "File written successfully");
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
```

#### > Penjelasan

1. Inisialisasi dan Konfigurasi FUSE

- Pada bagian awal program, terdapat inisialisasi dan konfigurasi FUSE menggunakan struct `xmp_oper`. Struct ini menghubungkan fungsi-fungsi yang akan dipanggil oleh FUSE saat operasi file dilakukan (seperti membaca, menulis, membuka, dan membaca direktori).
  
2. Fungsi `xmp_getattr`
- Fungsi ini digunakan untuk mendapatkan atribut (metadata) dari file atau direktori.
- Menggunakan `lstat` untuk mengambil informasi atribut file.

3. Fungsi `xmp_readdir`
- Fungsi ini membaca isi direktori.
- Jika path mengandung “rahasia” dan belum terotentikasi, maka user diminta untuk memasukkan password.
- Menggunakan `opendir` dan `readdir` untuk membaca isi direktori.

4. Fungsi `xmp_open`
- Fungsi ini membuka file.
- Menggunakan `open` untuk membuka file.

5. Fungsi `xmp_read`
- Fungsi ini membaca isi file.
- Jika path mengandung enkripsi (seperti “base64”, “rot13”, “hex”, atau “rev”), maka isi file didekripsi terlebih dahulu.
- Menggunakan `pread` untuk membaca isi file.

6. Fungsi `xmp_write`
- Fungsi ini menulis ke file.
- Menggunakan `pwrite` untuk menulis ke file.

7. Fungsi Dekripsi
- Terdapat beberapa fungsi dekripsi:
- `decode_base64`: Mendekripsi teks yang di-Base64.
- `decode_rot13`: Mendekripsi teks dengan algoritma ROT13.
- `decode_hex`: Mendekripsi teks yang di-Hexadecimal.
- `decode_rev`: Membalik urutan karakter dalam teks.

8. Logging
- Terdapat fungsi `log_message`, `log_success`, dan `log_failure` untuk mencatat pesan log ke file `logs-fuse.log`.

9. Autentikasi
- Fungsi `authenticate_user` meminta password dan memeriksa apakah benar “sisopmudah”.
- Fungsi `check_authentication` memeriksa status autentikasi.

10. Main Function
- Pada `main`, inisialisasi FUSE dan jalankan operasi FUSE.

Kesimpulan Kode program ini memungkinkan pengguna untuk mengakses file dan direktori dengan enkripsi dan dekripsi tertentu berdasarkan path yang diberikan. FUSE memungkinkan integrasi sistem file kustom dengan sistem operasi, sehingga operasi file dapat dimodifikasi sesuai kebutuhan.

#### > Dokumentasi
<img width="1704" alt="image" src="https://github.com/HazwanAdhikara/Sisop-4-2024-MH-IT13/assets/151142830/9f3bfcfb-7679-4ce8-8fd6-87a5c8b11018">
<img width="1710" alt="image" src="https://github.com/HazwanAdhikara/Sisop-4-2024-MH-IT13/assets/151142830/1c5df479-b25c-48b8-ac3c-65d19126bce7">
<img width="1710" alt="image" src="https://github.com/HazwanAdhikara/Sisop-4-2024-MH-IT13/assets/151142830/67b8701f-3d0d-4947-a8ea-7aedab38db3c">
<img width="1710" alt="image" src="https://github.com/HazwanAdhikara/Sisop-4-2024-MH-IT13/assets/151142830/950631db-a0a4-4e2d-9d0d-c4d64bce56c6">
<img width="1710" alt="image" src="https://github.com/HazwanAdhikara/Sisop-4-2024-MH-IT13/assets/151142830/43b77369-1a37-4727-ac05-02b89d9532b5">
<img width="1710" alt="image" src="https://github.com/HazwanAdhikara/Sisop-4-2024-MH-IT13/assets/151142830/41de4782-8c43-4666-8f37-c799eb159fd3">
<img width="1710" alt="image" src="https://github.com/HazwanAdhikara/Sisop-4-2024-MH-IT13/assets/151142830/e3ecce33-ff2e-4bb0-99ec-23b2e7be62b1">
<img width="1710" alt="image" src="https://github.com/HazwanAdhikara/Sisop-4-2024-MH-IT13/assets/151142830/68be5229-1461-4093-b47a-a279bc4476df">
<img width="1710" alt="image" src="https://github.com/HazwanAdhikara/Sisop-4-2024-MH-IT13/assets/151142830/5e27b83c-fb2a-4b29-9237-8b58bcc53946">

---

### **`Soal 3`**
`> Rayyan`

#### > Isi Soal
Seorang arkeolog menemukan sebuah gua yang didalamnya tersimpan banyak relik dari zaman praaksara, sayangnya semua barang yang ada pada gua tersebut memiliki bentuk yang terpecah belah akibat bencana yang tidak diketahui. Sang arkeolog ingin menemukan cara cepat agar ia bisa menggabungkan relik-relik yang terpecah itu, namun karena setiap pecahan relik itu masih memiliki nilai tersendiri, ia memutuskan untuk membuat sebuah file system yang mana saat ia mengakses file system tersebut ia dapat melihat semua relik dalam keadaan utuh, sementara relik yang asli tidak berubah sama sekali.
Ketentuan :

a.) Buatlah sebuah direktori dengan ketentuan seperti pada tree berikut
-
<img heigth="300" alt="image" src=https://github.com/HazwanAdhikara/Sisop-4-2024-MH-IT13/assets/150534107/d8037590-d724-47f0-84ee-727c18842dca>

b.) Direktori **[nama_bebas]** adalah direktori FUSE dengan direktori asalnya adalah direktori relics. Ketentuan Direktori **[nama_bebas]** adalah sebagai berikut :
- Ketika dilakukan listing, isi dari direktori [nama_bebas] adalah semua relic dari relics yang telah tergabung.
- Ketika dilakukan copy (dari direktori **[nama_bebas]** ke tujuan manapun), file yang disalin adalah file dari direktori **relics** yang sudah tergabung.
- Ketika ada file dibuat, maka pada direktori asal (direktori **relics**) file tersebut akan dipecah menjadi sejumlah pecahan dengan ukuran maksimum tiap pecahan adalah 10kb.
- File yang dipecah akan memiliki nama [namafile].000 dan seterusnya sesuai dengan jumlah pecahannya.
- Ketika dilakukan penghapusan, maka semua pecahannya juga ikut terhapus.

c.) Direktori **report** adalah direktori yang akan dibagikan menggunakan Samba File Server. Setelah kalian berhasil membuat direktori **[nama_bebas]**, jalankan FUSE dan salin semua isi direktori **[nama_bebas]** pada direktori **report**.

d.) Catatan:
- pada contoh terdapat 20 relic, namun pada zip file hanya akan ada 10 relic
- **[nama_bebas]** berarti namanya dibebaskan
- pada soal 3c, cukup salin secara manual. File Server hanya untuk membuktikan bahwa semua file pada direktori **[nama_bebas]** dapat dibuka dengan baik.

#### > Penyelesaian
### **`archeology.c`**
```bash
#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

static const char *folder_path = "/home/rrayyaann/sisop/percobaan/m4s3/relics";

static int archeology_getattr(const char *path, struct stat *stbuf) {
    memset(stbuf, 0, sizeof(struct stat));
    if (strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
    } else {
        char fpath[1000];
        snprintf(fpath, sizeof(fpath), "%s%s", folder_path, path);
        stbuf->st_mode = S_IFREG | 0444;
        stbuf->st_nlink = 1;
        stbuf->st_size = 0;

        int i = 0;
        char part_path[1100];
        FILE *fp;

        while (1) {
            snprintf(part_path, sizeof(part_path), "%s.%03d", fpath, i++);
            fp = fopen(part_path, "rb");
            if (!fp) break;

            fseek(fp, 0L, SEEK_END);
            stbuf->st_size += ftell(fp);
            fclose(fp);
        }

        if (i == 1) return -ENOENT;
    }
    return 0;
}

static int archeology_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
    (void) offset;
    (void) fi;

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);

    DIR *dp;
    struct dirent *de;
    dp = opendir(folder_path);
    if (dp == NULL) return -errno;

    while ((de = readdir(dp)) != NULL) {
        if (strstr(de->d_name, ".000") != NULL) {
            char base_name[256];
            strncpy(base_name, de->d_name, strlen(de->d_name) - 4);
            base_name[strlen(de->d_name) - 4] = '\0';
            filler(buf, base_name, NULL, 0);
        }
    }
    closedir(dp);
    return 0;
}

static int archeology_open(const char *path, struct fuse_file_info *fi) {
    return 0;
}

static int archeology_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
    (void) fi;
    size_t len;
    char fpath[1000];
    snprintf(fpath, sizeof(fpath), "%s%s", folder_path, path);

    int i = 0;
    char part_path[1100];
    size_t read_size = 0;

    while (size > 0) {
        snprintf(part_path, sizeof(part_path), "%s.%03d", fpath, i++);
        FILE *fp = fopen(part_path, "rb");
        if (!fp) break;

        fseek(fp, 0L, SEEK_END);
        size_t part_size = ftell(fp);
        fseek(fp, 0L, SEEK_SET);

        if (offset >= part_size) {
            offset -= part_size;
            fclose(fp);
            continue;
        }

        fseek(fp, offset, SEEK_SET);
        len = fread(buf, 1, size, fp);
        fclose(fp);

        buf += len;
        size -= len;
        read_size += len;
        offset = 0;
    }
    return read_size;
}

static int archeology_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
    (void) fi;
    char fpath[1000];
    snprintf(fpath, sizeof(fpath), "%s%s", folder_path, path);

    int part_num = offset / 10000;
    size_t part_offset = offset % 10000;
    size_t written_size = 0;
    char part_path[1100];

    while (size > 0) {
        snprintf(part_path, sizeof(part_path), "%s.%03d", fpath, part_num++);
        FILE *fp = fopen(part_path, "r+b");
        if (!fp) {
            fp = fopen(part_path, "wb");
            if (!fp) return -errno;
        }

        fseek(fp, part_offset, SEEK_SET);
        size_t write_size = size > (10000 - part_offset) ? (10000 - part_offset) : size;
        fwrite(buf, 1, write_size, fp);
        fclose(fp);

        buf += write_size;
        size -= write_size;
        written_size += write_size;
        part_offset = 0;
    }
    return written_size;
}

static int archeology_unlink(const char *path) {
    char fpath[1000];
    snprintf(fpath, sizeof(fpath), "%s%s", folder_path, path);

    int part_num = 0;
    char part_path[1100];
    int res = 0;

    while (1) {
        snprintf(part_path, sizeof(part_path), "%s.%03d", fpath, part_num++);
        res = unlink(part_path);
        if (res == -1 && errno == ENOENT) break;
        else if (res == -1) return -errno;
    }
    return 0;
}

static int archeology_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
    (void) fi;
    char fpath[1000];
    snprintf(fpath, sizeof(fpath), "%s%s.000", folder_path, path);

    int res = creat(fpath, mode);
    if (res == -1) return -errno;

    close(res);
    return 0;
}

static int archeology_truncate(const char *path, off_t size) {
    char fpath[1000];
    snprintf(fpath, sizeof(fpath), "%s%s", folder_path, path);

    int part_num = 0;
    char part_path[1100];
    off_t remaining_size = size;

    while (remaining_size > 0) {
        snprintf(part_path, sizeof(part_path), "%s.%03d", fpath, part_num++);
        size_t part_size = remaining_size > 10000 ? 10000 : remaining_size;
        int res = truncate(part_path, part_size);
        if (res == -1) return -errno;
        remaining_size -= part_size;
    }

    while (1) {
        snprintf(part_path, sizeof(part_path), "%s.%03d", fpath, part_num++);
        int res = unlink(part_path);
        if (res == -1 && errno == ENOENT) break;
        else if (res == -1) return -errno;
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
    .create = archeology_create,
    .truncate = archeology_truncate,
};

int main(int argc, char *argv[]) {
    umask(0);
    return fuse_main(argc, argv, &archeology_oper, NULL);
}
```
#### > Penjelasan
### archeology.c

1. Fungsi `*folder_path` untuk mendefinisikan path folder tempat file disimpan
```bash
static const char *folder_path = "/home/rrayyaann/sisop/percobaan/m4s3/relics";
```

2. Fungsi `archeology_getattr` untuk mendapatkan atribut file seperti mode, ukuran, dan lain-lain.
```bash
static int archeology_getattr(const char *path, struct stat *stbuf) {
    memset(stbuf, 0, sizeof(struct stat));
    if (strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
    } else {
        char fpath[1000];
        snprintf(fpath, sizeof(fpath), "%s%s", folder_path, path);
        stbuf->st_mode = S_IFREG | 0444;
        stbuf->st_nlink = 1;
        stbuf->st_size = 0;

        int i = 0;
        char part_path[1100];
        FILE *fp;

        while (1) {
            snprintf(part_path, sizeof(part_path), "%s.%03d", fpath, i++);
            fp = fopen(part_path, "rb");
            if (!fp) break;

            fseek(fp, 0L, SEEK_END);
            stbuf->st_size += ftell(fp);
            fclose(fp);
        }

        if (i == 1) return -ENOENT;
    }
    return 0;
}
```

3. Fungsi `archeology_readdir` untuk membaca isi direktori.
```bash
static int archeology_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
    (void) offset;
    (void) fi;

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);

    DIR *dp;
    struct dirent *de;
    dp = opendir(folder_path);
    if (dp == NULL) return -errno;

    while ((de = readdir(dp)) != NULL) {
        if (strstr(de->d_name, ".000") != NULL) {
            char base_name[256];
            strncpy(base_name, de->d_name, strlen(de->d_name) - 4);
            base_name[strlen(de->d_name) - 4] = '\0';
            filler(buf, base_name, NULL, 0);
        }
    }
    closedir(dp);
    return 0;
}
```

4. Fungsi `archeology_open` untuk membuka file.
```bash
static int archeology_open(const char *path, struct fuse_file_info *fi) {
    return 0;
}
```

5. Fungsi `archeology_read` untuk membaca data dari file
```bash
static int archeology_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
    (void) fi;
    size_t len;
    char fpath[1000];
    snprintf(fpath, sizeof(fpath), "%s%s", folder_path, path);

    int i = 0;
    char part_path[1100];
    size_t read_size = 0;

    while (size > 0) {
        snprintf(part_path, sizeof(part_path), "%s.%03d", fpath, i++);
        FILE *fp = fopen(part_path, "rb");
        if (!fp) break;

        fseek(fp, 0L, SEEK_END);
        size_t part_size = ftell(fp);
        fseek(fp, 0L, SEEK_SET);

        if (offset >= part_size) {
            offset -= part_size;
            fclose(fp);
            continue;
        }

        fseek(fp, offset, SEEK_SET);
        len = fread(buf, 1, size, fp);
        fclose(fp);

        buf += len;
        size -= len;
        read_size += len;
        offset = 0;
    }
    return read_size;
}
```

6. Fungsi `archeology_write`
```bash
static int archeology_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
    (void) fi;
    char fpath[1000];
    snprintf(fpath, sizeof(fpath), "%s%s", folder_path, path);

    int part_num = offset / 10000;
    size_t part_offset = offset % 10000;
    size_t written_size = 0;
    char part_path[1100];

    while (size > 0) {
        snprintf(part_path, sizeof(part_path), "%s.%03d", fpath, part_num++);
        FILE *fp = fopen(part_path, "r+b");
        if (!fp) {
            fp = fopen(part_path, "wb");
            if (!fp) return -errno;
        }

        fseek(fp, part_offset, SEEK_SET);
        size_t write_size = size > (10000 - part_offset) ? (10000 - part_offset) : size;
        fwrite(buf, 1, write_size, fp);
        fclose(fp);

        buf += write_size;
        size -= write_size;
        written_size += write_size;
        part_offset = 0;
    }
    return written_size;
}
```

7. Fungsi `archeology_unlink` untuk menghapus file
```bash
static int archeology_unlink(const char *path) {
    char fpath[1000];
    snprintf(fpath, sizeof(fpath), "%s%s", folder_path, path);

    int part_num = 0;
    char part_path[1100];
    int res = 0;

    while (1) {
        snprintf(part_path, sizeof(part_path), "%s.%03d", fpath, part_num++);
        res = unlink(part_path);
        if (res == -1 && errno == ENOENT) break;
        else if (res == -1) return -errno;
    }
    return 0;
}
```

8. Fungsi `archeology_create` untuk membuat file baru
```bash
static int archeology_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
    (void) fi;
    char fpath[1000];
    snprintf(fpath, sizeof(fpath), "%s%s.000", folder_path, path);

    int res = creat(fpath, mode);
    if (res == -1) return -errno;

    close(res);
    return 0;
}
```

9. Fungsi `archeology_truncate` Untuk memangkas/memotong (truncate) file.
```bash
static int archeology_truncate(const char *path, off_t size) {
    char fpath[1000];
    snprintf(fpath, sizeof(fpath), "%s%s", folder_path, path);

    int part_num = 0;
    char part_path[1100];
    off_t remaining_size = size;

    while (remaining_size > 0) {
        snprintf(part_path, sizeof(part_path), "%s.%03d", fpath, part_num++);
        size_t part_size = remaining_size > 10000 ? 10000 : remaining_size;
        int res = truncate(part_path, part_size);
        if (res == -1) return -errno;
        remaining_size -= part_size;
    }

    while (1) {
        snprintf(part_path, sizeof(part_path), "%s.%03d", fpath, part_num++);
        int res = unlink(part_path);
        if (res == -1 && errno == ENOENT) break;
        else if (res == -1) return -errno;
    }

    return 0;
}
```

10. Fungsi `fuse_operations` merupakan struktur operasi FUSE
```bash
static struct fuse_operations archeology_oper = {
    .getattr = archeology_getattr,
    .readdir = archeology_readdir,
    .open = archeology_open,
    .read = archeology_read,
    .write = archeology_write,
    .unlink = archeology_unlink,
    .create = archeology_create,
    .truncate = archeology_truncate,
};
```

11. Fungsi `int main` dalam kode FUSE ini bertanggung jawab untuk menginisialisasi dan menjalankan sistem file FUSE.
```bash
int main(int argc, char *argv[]) {
    umask(0);
    return fuse_main(argc, argv, &archeology_oper, NULL);
}
```

12. Isi file configurasi samba yang menuju ke folder report
<img width="450" alt="image" src=https://github.com/HazwanAdhikara/Sisop-4-2024-MH-IT13/assets/150534107/08c3949c-4592-4a6e-aeb9-b49623448871>

#### > Dokumentasi
<img width="1710" alt="image" src=https://github.com/HazwanAdhikara/Sisop-4-2024-MH-IT13/assets/150534107/c9462e65-beec-40e0-809d-d3cb234adc7a>
<img width="1710" alt="image" src=https://github.com/HazwanAdhikara/Sisop-4-2024-MH-IT13/assets/150534107/1409a8d9-1da1-4de7-b836-f79d9d3c39f6>
<img width="1710" alt="image" src=https://github.com/HazwanAdhikara/Sisop-4-2024-MH-IT13/assets/150534107/847a92db-0e06-47b3-ac5c-d878cdb40ccf>
<img width="1710" alt="image" src=https://github.com/HazwanAdhikara/Sisop-4-2024-MH-IT13/assets/150534107/0f2d24e7-4b9f-41c4-a24e-5eb2302d0e88>

---
