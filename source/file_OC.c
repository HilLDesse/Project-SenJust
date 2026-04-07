#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include "../header/File_OC.h"
#include "../header/file_ec.h"
#include "../header/file_s.h"
#include "../header/screen.h"
#include "../header/menu.h" 

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

            printf("\n--- ISI FILE ---");
            printLayar(ed, ed->b_now, ed->k_now);

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

    ed->next = 1; // Default program terus berlanjut

    if (ed->isSaved == 0) { // Kondis file belum disave
        system("cls");
        printf("[PERINGATAN] File belum disave, Apakah anda ingin menyimpan? (y/n): ");
        char simpan = getch();
        
        if (simpan == 'y' || simpan == 'Y') {
            saveFile(ed);
        } else if (simpan == 'n' || simpan == 'N') {
            printf("Perubahan tidak disimpan\n");
        } else {
            printf("\nInput tidak valid.\n");
            getch();
            return;
        }

        printf("\nLanjut edit file atau kembali ke menu utama? (y/n): ");
        char lanjut = getch();

        if (lanjut == 'y' || lanjut == 'Y') {
            ed->next = 1;                       // Program terus berjalan
            printLayar(ed, ed->b_now, ed->k_now);
        } else if (lanjut == 'n' || lanjut == 'N') {
            ed->next = 0;
            printf("\nKembali ke menu utama...");
        } else {
            printf("\nInput tidak valid.\n");
            getch();
            ed->next = 1;
        }

    } else if (ed->isSaved == 1) {
            system("cls");
            printf("Lanjut edit file atau kembali ke menu utama? (y/n): ");
            char lanjut = getch();

            if (lanjut == 'y' || lanjut == 'Y') {
                ed->next = 1;
                system("cls");
                printLayar(ed, ed->b_now, ed->k_now);
            } else if (lanjut == 'n' || lanjut == 'N') {
                ed->next = 0;
                printf("\nKembali ke menu utama...");
            } else {
                printf("\nInput tidak valid.\n");
                getch();
                ed->next = 1;
            }
        }
}
    
