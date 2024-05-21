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
