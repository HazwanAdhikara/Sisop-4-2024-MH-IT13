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
Adfi merupakan seorang CEO agency creative bernama Ini Karya Kita. Ia sedang melakukan inovasi pada manajemen project photography Ini Karya Kita. Salah satu ide yang dia kembangkan adalah tentang pengelolaan foto project dalam sistem arsip Ini Karya Kita. Dalam membangun sistem ini, Adfi tidak bisa melakukannya sendirian, dia perlu bantuan mahasiswa Departemen Teknologi Informasi angkatan 2023 untuk membahas konsep baru yang akan mengubah project fotografinya lebih menarik untuk dilihat. Adfi telah menyiapkan portofolio hasil project fotonya yang bisa didownload dan diakses di www.inikaryakita.id . Silahkan eksplorasi web Ini Karya Kita dan temukan halaman untuk bisa mendownload projectnya. Setelah kalian download terdapat folder gallery dan bahaya.
- Pada folder “gallery”:
Membuat folder dengan prefix "wm." Dalam folder ini, setiap gambar yang dipindahkan ke dalamnya akan diberikan watermark bertuliskan inikaryakita.id. 
			Ex: "mv ikk.jpeg wm-foto/" 
Output: 
Before: (tidak ada watermark bertuliskan inikaryakita.id)
After: (terdapat watermark tulisan inikaryakita.id)

- Pada folder "bahaya," terdapat file bernama "script.sh." Adfi menyadari pentingnya menjaga keamanan dan integritas data dalam folder ini. 

Mereka harus mengubah permission pada file "script.sh" agar bisa dijalankan, karena jika dijalankan maka dapat menghapus semua dan isi dari  "gallery"

Adfi dan timnya juga ingin menambahkan fitur baru dengan membuat file dengan prefix "test" yang ketika disimpan akan mengalami pembalikan (reverse) isi dari file tersebut.  

#### > Penyelesaian
```bash
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

void add_watermark(const char *input_image, const char *output_image) {
    char command[512];
    snprintf(command, sizeof(command),
             "convert %s -gravity SouthEast -pointsize 36 -fill white -annotate +10+10 'inikaryakita.id' %s",
             input_image, output_image);
    system(command);
}

void move_and_watermark(const char *source, const char *destination_folder) {
    // Construct the destination folder with prefix 'wm.'
    char wm_folder[512];
    snprintf(wm_folder, sizeof(wm_folder), "%s/wm-%s", destination_folder, "foto");

    // Create the wm_folder if it does not exist
    struct stat st = {0};
    if (stat(wm_folder, &st) == -1) {
        mkdir(wm_folder, 0700);
    }

    // Construct the destination file path
    char destination_file[512];
    snprintf(destination_file, sizeof(destination_file), "%s/%s", wm_folder, strrchr(source, '/') + 1);

    // Add watermark and move file
    add_watermark(source, destination_file);
    remove(source);
}

void reverse_test_files(const char *folder_path, const char *output_dir) {
    char command[512];
    snprintf(command, sizeof(command), "mkdir -p %s", output_dir);
    system(command);

    snprintf(command, sizeof(command),
             "find %s -name 'inilho-*.txt' -exec sh -c 'for file; do "
             "filename=$(basename \"$file\"); "
             "rev \"$file\" > \"%s/${filename%%.txt}_reversed.txt\" && echo \"Reversed $file to %s/${filename%%.txt}_reversed.txt\"; "
             "done' sh {} +", folder_path, output_dir, output_dir);
    printf("Executing command: %s\n", command);
    system(command);
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <source_image> <destination_folder> <text_files_folder> <output_text_folder>\n", argv[0]);
        return 1;
    }

    // Arguments
    const char *source_image = argv[1];
    const char *destination_folder = argv[2];
    const char *text_files_folder = argv[3];
    const char *output_text_folder = argv[4];

    // Perform tasks
    move_and_watermark(source_image, destination_folder);
    reverse_test_files(text_files_folder, output_text_folder);

    return 0;
}
```

#### > Penjelasan

```bash
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
```
#include <stdio.h>:
Digunakan untuk fungsi input/output standar seperti printf, fprintf, snprintf, dll.
Dalam kode ini, digunakan untuk menampilkan pesan ke terminal dan untuk membuat string command.

#include <stdlib.h>:
Digunakan untuk fungsi utilitas umum seperti system, malloc, free, exit, dll.
Dalam kode ini, system digunakan untuk menjalankan perintah sistem dari dalam program C.

#include <string.h>:
Digunakan untuk fungsi manipulasi string seperti strrchr, strlen, strcpy, strcat, dll.
Dalam kode ini, strrchr digunakan untuk mendapatkan pointer ke bagian terakhir dari string (khususnya, nama file dari path lengkap).

#include <unistd.h>:
Menyediakan akses ke API POSIX untuk operasi standar sistem seperti fork, exec, getpid, chdir, dll.
Dalam kode ini, digunakan untuk fungsi access dan unlink (jika dibutuhkan, meskipun unlink tidak digunakan secara eksplisit dalam kode ini, fungsi remove dari stdlib.h sudah cukup).

#include <sys/stat.h>:
Digunakan untuk mengakses informasi file dan fungsi manajemen file/direktori seperti stat, mkdir, dll.
Dalam kode ini, digunakan untuk mengecek keberadaan direktori (stat) dan membuat direktori (mkdir).

```bash
void add_watermark(const char *input_image, const char *output_image) {
    char command[512];
    snprintf(command, sizeof(command),
             "convert %s -gravity SouthEast -pointsize 36 -fill white -annotate +10+10 'inikaryakita.id' %s",
             input_image, output_image);
    system(command);
}
```
Deskripsi: Fungsi ini menambahkan watermark pada gambar menggunakan perintah convert dari ImageMagick.
Parameter:
input_image: Path ke gambar sumber yang akan diberi watermark.
output_image: Path ke gambar hasil dengan watermark.
Proses:
Membuat command string menggunakan snprintf yang berisi perintah untuk menambahkan watermark "inikaryakita.id" di sudut kanan bawah gambar.
Menjalankan perintah ini menggunakan system.

```bash
void move_and_watermark(const char *source, const char *destination_folder) {
    char wm_folder[512];
    snprintf(wm_folder, sizeof(wm_folder), "%s/wm-%s", destination_folder, "foto");

    struct stat st = {0};
    if (stat(wm_folder, &st) == -1) {
        mkdir(wm_folder, 0700);
    }

    char destination_file[512];
    snprintf(destination_file, sizeof(destination_file), "%s/%s", wm_folder, strrchr(source, '/') + 1);

    add_watermark(source, destination_file);
    remove(source);
}
```
Deskripsi: Fungsi ini memindahkan file gambar ke folder tujuan setelah menambahkan watermark.
Parameter:
source: Path ke file gambar sumber.
destination_folder: Path ke folder tujuan.
Proses:
Membuat path untuk folder tujuan dengan prefix wm-.
Mengecek apakah folder tujuan sudah ada atau belum, jika belum maka folder dibuat.
Menentukan path file tujuan dengan menggabungkan nama file sumber dengan folder tujuan.
Memanggil fungsi add_watermark untuk menambahkan watermark pada gambar.
Menghapus file sumber setelah proses watermark.

```bash
void reverse_test_files(const char *folder_path, const char *output_dir) {
    char command[512];
    snprintf(command, sizeof(command), "mkdir -p %s", output_dir);
    system(command);

    snprintf(command, sizeof(command),
             "find %s -name 'inilho-*.txt' -exec sh -c 'for file; do "
             "filename=$(basename \"$file\"); "
             "rev \"$file\" > \"%s/${filename%%.txt}_reversed.txt\" && echo \"Reversed $file to %s/${filename%%.txt}_reversed.txt\"; "
             "done' sh {} +", folder_path, output_dir, output_dir);
    printf("Executing command: %s\n", command);
    system(command);
}
```
Deskripsi: Fungsi ini membalik isi dari file teks yang memenuhi pola nama tertentu dan menyimpan hasilnya di direktori keluaran.
Parameter:
folder_path: Path ke folder yang berisi file-file teks.
output_dir: Path ke folder keluaran untuk menyimpan file hasil.
Proses:
Membuat folder keluaran jika belum ada.
Mencari file teks dengan nama yang sesuai pola inilho-*.txt dan membalik isi file tersebut menggunakan perintah rev.
Menyimpan hasil ke folder keluaran dengan menambahkan suffix _reversed.txt.

```bash
int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <source_image> <destination_folder> <text_files_folder> <output_text_folder>\n", argv[0]);
        return 1;
    }

    const char *source_image = argv[1];
    const char *destination_folder = argv[2];
    const char *text_files_folder = argv[3];
    const char *output_text_folder = argv[4];

    move_and_watermark(source_image, destination_folder);
    reverse_test_files(text_files_folder, output_text_folder);

    return 0;
}
```
Deskripsi: Fungsi utama yang mengendalikan alur program.
Parameter: Menerima 4 argumen dari command line.
source_image: Path ke gambar sumber.
destination_folder: Path ke folder tujuan untuk gambar yang sudah diberi watermark.
text_files_folder: Path ke folder yang berisi file-file teks.
output_text_folder: Path ke folder keluaran untuk menyimpan file teks yang sudah dibalik isinya.
Proses:
Memeriksa jumlah argumen yang diberikan. Jika tidak sesuai, menampilkan pesan penggunaan dan keluar dengan kode status 1.
Memanggil move_and_watermark untuk memindahkan dan menambahkan watermark pada gambar.
Memanggil reverse_test_files untuk membalik isi file teks yang sesuai dan menyimpan hasilnya di folder keluaran.
Program ini menggabungkan fungsi dari sistem operasi (seperti system, stat, mkdir, remove) dan utilitas eksternal (ImageMagick dan rev) untuk melakukan operasi yang diinginkan.


#### > Dokumentasi
![Pasted Graphic](https://github.com/HazwanAdhikara/Sisop-4-2024-MH-IT13/assets/137234298/f6f05832-e426-44fd-898f-c832b75bd08d)
<img width="243" alt="image" src="https://github.com/HazwanAdhikara/Sisop-4-2024-MH-IT13/assets/137234298/0d792ad9-17b3-4160-baad-fe323a6a3e22">
<img width="582" alt="image" src="https://github.com/HazwanAdhikara/Sisop-4-2024-MH-IT13/assets/137234298/28c311dc-6fd0-4ce3-b926-51cf4ee7bfd9">
<img width="438" alt="image" src="https://github.com/HazwanAdhikara/Sisop-4-2024-MH-IT13/assets/137234298/2383adeb-6a0e-4876-abd4-9ce9789179cc">

#### > Revisi
Program yang saya buat masih belum menggunakan FUSE namun fungsi" dari code-nya berjalan sesuai yang diminta dengan soal. Sebelumnya saya juga sudah membuat program dengan menggunakan FUSE namun fungsi dari programnya masih belum bisa dan error. jadi saya membuat program baru dari awal dan jadinya seperti ini:
```bash
#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

static const char *base_path = "portofolio";
static const char *gallery_path = "portofolio/gallery";
static const char *watermark_path = "portofolio/wm";
static const char *script_path = "portofolio/bahaya/script.sh";
static const char *test_folder = "portofolio/bahaya";
static const char *reversed_path = "portofolio/reversed";
static const char *mountpoint_path = "portofolio/mountpoint";
static const char *watermark_text = "inikaryakita.id";

// Function to watermark images using ImageMagick
void watermark_images(const char *input_dir, const char *output_dir, const char *watermark_text) {
    char command[512];
    snprintf(command, sizeof(command), "mkdir -p %s", output_dir);
    system(command);

    snprintf(command, sizeof(command), "for img in %s/*.jpeg; do "
                                       "filename=$(basename \"$img\"); "
                                       "height=$(identify -format %%h \"$img\"); "
                                       "fontsize=$(($height / 40)); "
                                       "convert \"$img\" -gravity south -fill white -pointsize $fontsize -annotate +0+10 \"%s\" \"%s/$filename\"; "
                                       "echo \"Watermarked $img to %s/$filename\"; "
                                       "done", input_dir, watermark_text, output_dir, output_dir);
    printf("Executing command: %s\n", command);
    system(command);
}

// Function to set script permissions
void set_script_permissions(const char *script_path) {
    if (chmod(script_path, S_IRWXU) != 0) {
        perror("chmod failed");
        exit(EXIT_FAILURE);
    }
}

// Function to reverse the content of test files and create new files
void reverse_test_files(const char *folder_path, const char *output_dir) {
    char command[512];
    snprintf(command, sizeof(command), "mkdir -p %s", output_dir);
    system(command);

    snprintf(command, sizeof(command), "find %s -name 'test-*.txt' -exec sh -c 'for file; do "
                                       "filename=$(basename \"$file\"); "
                                       "rev \"$file\" > \"%s/${filename%%.txt}_reversed.txt\" && echo \"Reversed $file to %s/${filename%%.txt}_reversed.txt\"; "
                                       "done' sh {} +", folder_path, output_dir, output_dir);
    printf("Executing command: %s\n", command);
    system(command);
}

static int do_getattr(const char *path, struct stat *st) {
    memset(st, 0, sizeof(struct stat));
    printf("do_getattr: %s\n", path);
    if (strcmp(path, "/") == 0) {
        st->st_mode = S_IFDIR | 0755;
        st->st_nlink = 2;
    } else if (strcmp(path, "/wm") == 0) {
        st->st_mode = S_IFDIR | 0755;
        st->st_nlink = 2;
    } else if (strcmp(path, "/reversed") == 0) {
        st->st_mode = S_IFDIR | 0755;
        st->st_nlink = 2;
    } else if (strncmp(path, "/wm/", 4) == 0) {
        st->st_mode = S_IFREG | 0644;
        st->st_nlink = 1;
        char full_path[512];
        snprintf(full_path, sizeof(full_path), "%s/wm%s", base_path, path + 3); // Adjust path to base path
        printf("Checking file: %s\n", full_path);
        FILE *file = fopen(full_path, "r");
        if (file) {
            fseek(file, 0, SEEK_END);
            st->st_size = ftell(file);
            fclose(file);
        } else {
            printf("File not found: %s\n", full_path);
            return -ENOENT;
        }
    } else if (strncmp(path, "/reversed/", 10) == 0) {
        st->st_mode = S_IFREG | 0644;
        st->st_nlink = 1;
        char full_path[512];
        snprintf(full_path, sizeof(full_path), "%s/reversed%s", base_path, path + 9); // Adjust path to base path
        printf("Checking file: %s\n", full_path);
        FILE *file = fopen(full_path, "r");
        if (file) {
            fseek(file, 0, SEEK_END);
            st->st_size = ftell(file);
            fclose(file);
        } else {
            printf("File not found: %s\n", full_path);
            return -ENOENT;
        }
    } else {
        return -ENOENT;
    }
    return 0;
}

static int do_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
    (void) offset;
    (void) fi;

    printf("do_readdir: %s\n", path);
    if (strcmp(path, "/") == 0) {
        filler(buf, ".", NULL, 0);
        filler(buf, "..", NULL, 0);
        filler(buf, "wm", NULL, 0);
        filler(buf, "reversed", NULL, 0);
    } else if (strcmp(path, "/wm") == 0) {
        filler(buf, ".", NULL, 0);
        filler(buf, "..", NULL, 0);

        printf("Reading directory: %s\n", "portofolio/wm");
        system("ls portofolio/wm > wm_files.txt");
        FILE *fp = fopen("wm_files.txt", "r");
        if (fp == NULL) return -errno;

        char filename[256];
        while (fscanf(fp, "%255s", filename) == 1) {
            printf("Adding wm file: %s\n", filename);
            filler(buf, filename, NULL, 0);
        }
        fclose(fp);
        system("rm wm_files.txt");
    } else if (strcmp(path, "/reversed") == 0) {
        filler(buf, ".", NULL, 0);
        filler(buf, "..", NULL, 0);

        printf("Reading directory: %s\n", "portofolio/reversed");
        system("ls portofolio/reversed > reversed_files.txt");
        FILE *fp = fopen("reversed_files.txt", "r");
        if (fp == NULL) return -errno;

        char filename[256];
        while (fscanf(fp, "%255s", filename) == 1) {
            printf("Adding reversed file: %s\n", filename);
            filler(buf, filename, NULL, 0);
        }
        fclose(fp);
        system("rm reversed_files.txt");
    } else {
        return -ENOENT;
    }

    return 0;
}

static int do_open(const char *path, struct fuse_file_info *fi) {
    printf("do_open: %s\n", path);
    if (strncmp(path, "/wm/", 4) == 0 || strncmp(path, "/reversed/", 10) == 0) {
        return 0;
    }
    return -ENOENT;
}

static int do_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
    size_t len;
    (void) fi;
    printf("do_read: %s\n", path);
    if (strncmp(path, "/wm/", 4) == 0) {
        char full_path[512];
        snprintf(full_path, sizeof(full_path), "%s/wm%s", base_path, path + 3); // Adjust path to base path
        printf("Reading file: %s\n", full_path);
        FILE *file = fopen(full_path, "r");
        if (!file) {
            perror("fopen failed");
            return -ENOENT;
        }
        fseek(file, 0, SEEK_END);
        len = ftell(file);
        fseek(file, 0, SEEK_SET);
        if (offset < len) {
            if (offset + size > len)
                size = len - offset;
            fread(buf, 1, size, file);
        } else {
            size = 0;
        }
        fclose(file);
        return size;
    } else if (strncmp(path, "/reversed/", 10) == 0) {
        char full_path[512];
        snprintf(full_path, sizeof(full_path), "%s/reversed%s", base_path, path + 9); // Adjust path to base path
        printf("Reading file: %s\n", full_path);
        FILE *file = fopen(full_path, "r");
        if (!file) {
            perror("fopen failed");
            return -ENOENT;
        }
        fseek(file, 0, SEEK_END);
        len = ftell(file);
        fseek(file, 0, SEEK_SET);
        if (offset < len) {
            if (offset + size > len)
                size = len - offset;
            fread(buf, 1, size, file);
        } else {
            size = 0;
        }
        fclose(file);
        return size;
    }
    return -ENOENT;
}

static struct fuse_operations operations = {
    .getattr = do_getattr,
    .readdir = do_readdir,
    .open = do_open,
    .read = do_read,
};

int main(int argc, char *argv[]) {
    // Create the mountpoint directory
    mkdir(mountpoint_path, 0755);

    // Watermark images
    watermark_images(gallery_path, watermark_path, watermark_text);

    // Set script permissions
    set_script_permissions(script_path);

    // Reverse test files
    reverse_test_files(test_folder, reversed_path);

    // Run FUSE main loop
    const char *fuse_argv[] = { argv[0], mountpoint_path };
    int fuse_argc = 2;
    return fuse_main(fuse_argc, (char **) fuse_argv, &operations, NULL);
}
``` 
- Perbedaan antara Program Pertama dan Program Kedua
1. Tujuan dan Struktur:

- Program Pertama: Program ini berfokus pada pemrosesan gambar dengan menambahkan watermark dan memindahkannya ke folder baru. Selain itu, program ini juga membalik isi file teks dan membuat file baru dengan isi yang dibalik. Fungsi utama mengorkestrasi tugas-tugas ini dengan memanggil fungsi spesifik menggunakan argumen yang diberikan.
- Program Kedua: Program ini mengintegrasikan dengan sistem file FUSE untuk membuat sistem file virtual. Meskipun tugas-tugas yang dilakukannya mirip (menambahkan watermark pada gambar dan membalik isi file teks), tujuan utamanya adalah untuk mengatur sistem file virtual yang menampilkan gambar yang sudah diberi watermark dan file teks yang isinya sudah dibalik.
2. Operasi File:

- Program Pertama: Menggunakan perintah sistem langsung untuk menangani operasi file, seperti memindahkan file, menambahkan watermark, dan membalik isi file teks.
- Program Kedua: Menerapkan operasi file dalam konteks sistem file virtual FUSE. Ini mendefinisikan callback FUSE (getattr, readdir, open, dan read) untuk menangani atribut file, daftar direktori, membuka file, dan membaca file.
3. Penanganan Kesalahan dan Logging:

- Program Pertama: Memiliki penanganan kesalahan dan logging yang minimal. Utamanya menggunakan printf untuk logging dan pengecekan dasar (misalnya, keberadaan direktori).
- Program Kedua: Menggunakan logging yang lebih detail (misalnya, printf untuk melacak operasi) dan penanganan kesalahan yang lebih baik, terutama dalam operasi FUSE untuk menangani kesalahan file tidak ditemukan dan pengaturan izin.
4. Integrasi Sistem File:

- Program Pertama: Tidak mengintegrasikan dengan sistem file selain operasi file dasar. Ini adalah program mandiri yang memproses file berdasarkan argumen baris perintah.
- Program Kedua: Mengintegrasikan dengan sistem file FUSE, memungkinkan pengguna untuk berinteraksi dengan sistem file virtual yang secara dinamis merepresentasikan gambar yang sudah diberi watermark dan file teks yang isinya sudah dibalik. Ini memerlukan operasi mounting dan unmounting khas aplikasi berbasis FUSE.
* Kesalahan pada Program Pertama
1. Penanganan Path yang Tidak Lengkap:

-Program Pertama membangun path tanpa memastikan semua direktori ada atau menangani masalah path dengan kuat. Misalnya, membangun wm_folder mungkin gagal jika direktori menengah tidak ada.
2. Pemanggilan Sistem Langsung:

- Program Pertama sangat bergantung pada pemanggilan system() untuk operasi seperti membuat direktori, yang kurang efisien dan berpotensi tidak aman dibandingkan menggunakan pemanggilan sistem khusus (misalnya, mkdir, stat).
3. Tidak Ada Pengecekan Kesalahan untuk Pemanggilan system():

- Program Pertama tidak memeriksa nilai kembali dari pemanggilan system(), yang dapat menyebabkan kegagalan diam-diam jika suatu perintah gagal.
4. Menghapus File Sumber:

- Program Pertama memanggil remove(source) tanpa memverifikasi apakah operasi watermark berhasil, yang mungkin mengakibatkan kehilangan data jika proses watermark gagal.
* Peningkatan dalam Program Kedua
1. Integrasi FUSE:

- Program Kedua menyediakan solusi yang lebih kuat dengan mengintegrasikan dengan sistem file FUSE, memungkinkan representasi virtual dari file yang sudah diproses.
2. Penanganan Kesalahan dan Logging yang Lebih Baik:

- Program Kedua menyertakan logging dan penanganan kesalahan yang lebih detail, terutama dalam operasi sistem file, yang membantu dalam mendiagnosis masalah.
3. Operasi File yang Efisien:

- Program Kedua menggunakan callback FUSE untuk menangani operasi file secara efisien tanpa bergantung pada pemanggilan system() yang berlebihan. Pendekatan ini lebih aman dan efisien.
4. Organisasi Sistem File:

- Program Kedua memastikan bahwa direktori seperti mountpoint_path dibuat dan dikelola dengan benar, meningkatkan kekuatan penanganan path file.

- Kesimpulannya, Program Kedua menyediakan pendekatan yang lebih terstruktur, aman, dan efisien dengan memanfaatkan sistem file FUSE, memastikan penanganan kesalahan yang lebih baik, dan mengorganisasikan interaksi sistem file dengan lebih efektif.









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
- ![image](https://github.com/HazwanAdhikara/Sisop-4-2024-MH-IT13/assets/150534107/027a559e-b907-4503-9106-b951f4ce427b)

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
- terdapat inisialisasi dan konfigurasi FUSE menggunakan struct `archeology_oper`. Struct ini menghubungkan fungsi-fungsi yang akan dipanggil oleh FUSE saat operasi file dilakukan (seperti membaca, menulis, membuka, menghapus, dan membuat direktori).
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
