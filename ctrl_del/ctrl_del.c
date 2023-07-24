#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int main(int argc, char* argv[]) {

    if (argc > 1) {

        if (argc > 2) {

            char* dir = (char*)malloc(sizeof(char) * 128);
            char* file_dir ="";
            char* file_name = argv[1];
            char* mode = argv[2];
            FILE *file;

            strcpy(dir, file_dir);
            strcat(dir, file_name);

            //printf("Mode: %c\n", *mode);

            if ((*mode == 'w') && strlen(mode) < 2) {

                file = fopen(dir, mode);
                fclose(file);
                printf("File created: %s\n", dir);
                
            }
            else if ((*mode == 'd') && strlen(mode) < 2) {

                remove(dir);
                printf("File deleted: %s\n", dir);

            }
            else
            printf("Allowed modes: \"w\" to create and \"d\" to remove.\n");

        }
        else {
            printf("Mode is not specified.\n");
            return -1;
        }
    }
    else {
        printf("File name is not specified.\n");
            return -1;
    }
    return 0;