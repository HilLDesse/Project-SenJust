#include <stdio.h>
#include <string.h>
#include <conio.h>
#include "../header/File_OC.h"

void openFile(Buffer *ed) {
    printf("Masukkan nama file yang ingin dibuka: ");
    fgets(ed->namaFile, 100, stdin);
    ed->namaFile[strcspn(ed->namaFile, "\n")] = 0;

    FILE *file = fopen(ed->namaFile, "r");
    if (file != NULL) {
        ed -> total_baris = 0;

        while (ed->total_baris < MAX_LINE && 
            fgets(ed->teks[ed->total_baris], MAX_COL, file)) {
            ed->teks[ed->total_baris][strcspn(ed->teks[ed->total_baris], "\n")] = 0;
            ed->total_baris++;
            }

            fclose(file);
            ed->isSaved = 1; // 1 artinya file sudah di save
            printf("[Berhasil] file %s dibuka.\n", ed->namaFile); 

            printf("\n--ISI FILE---\n");
            for(int i = 0;i < ed->total_baris; i++) {
                printf("\n%s\n", ed->teks[i]); // Menampilkan isi dari file 
            }
            printf("---------------\n");

    } else {
        printf("[ERROR] File tidak ditemukan!\n");
        ed->namaFile[0] = '\0';  // untuk stop membaca file 
    }
}