#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

#include "../header/File_OC.h"
#include "../header/file_s.h"
#include "../header/file_ec.h"
#include "../header/screen.h"

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

            printf("Apakah anda ingin mengedit file ini? (y/n): ");
            char edit = getch();
            if (edit == 'y' || edit == 'Y') {
                editFile(ed);
            }

    } else {
        printf("[ERROR] File tidak ditemukan!\n");
        ed->namaFile[0] = '\0';  // untuk stop membaca file 
    }
}

void closeFile(Buffer *ed) {
    if (ed->input == 27 && ed->isSaved == 0) { // Kondisi user input 27 (ESC) dan file belum disave
        system("cls");
        printf("[PERINGATAN] File belum disave, Apakah anda ingin menyimpan? (y/n): ");
        char simpan = getch();
        if (simpan == 'y' || simpan == 'Y') {
            saveFile(ed);
            printf("\nLanjut edit file atau kembali menu utama? (y/n): ");
            char lanjut = getch();
            if (lanjut == 'y' || lanjut == 'Y') {
                printLayar(ed, ed->b_now, ed->k_now);
                editFile(ed);
            }
        }
    }
}