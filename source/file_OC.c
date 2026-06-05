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
    printf("Masukkan nama file yang ingin di buka: "); 
    fgets(ed->namaFile, 100, stdin);
    ed->namaFile[strcspn(ed->namaFile, "\n")] = 0;

    FILE *file = fopen(ed->namaFile, "r");
    if (file != NULL) {
        ed->total_baris = 0;
        ed->head = NULL;
        ed->tail = NULL;

        char temp_teks[1024];
        while (fgets(temp_teks, sizeof(temp_teks), file)) {
            temp_teks[strcspn(temp_teks, "\n")] = 0; // Buang Newline dari fgets
            // 1. Alokasi Node Baru
            Node *newNode = buatNode();

            // 2. Alokasi memori teks secara dinamis
            // strdup otomatis melakukan malloc(strlen(temp_teks) + 1) dan menyalin isinya.
            newNode->teks = strdup(temp_teks);

            // 3. Sambungkan Node ke List
            if (ed->head == NULL) {
                newNode->prev = NULL;
                ed->head = newNode;
                ed->tail = newNode;
                ed->current = newNode; // Set current ke baris pertama
            } else {
                newNode->prev = ed->tail;
                ed->tail->next = newNode;
                ed->tail = newNode;
            }
            ed->total_baris++;
        }

        fclose(file);
        ed->isSaved = 1;
        printf("[Berhasil] file %s dibuka.\n", ed->namaFile);

        printf("\n---ISI FILE---");
        printLayar(ed, ed->b_now, ed->k_now);

        printf("Apakah anda ingin mengedit file ini? (y/n): ");
        char edit = getch();
        if (edit == 'y' || edit == 'Y') {
            editFile(ed); // MEMANGGIL FUNGSI EDIT 
        }
        
    } else {
        printf("[Error] File tidak ditemukan!\n");
        ed->namaFile[0] = '\0';
    }
}

void closeFile(Buffer *ed) {

    ed->next = 1; // Default program terus berlanjut

    if (ed->isSaved == 0) { // Kondisi file belum disave
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
            freeList(&ed->head); // Bebaskan memori sebelum kembali ke menu
            ed->head = NULL;
            ed->tail = NULL;
            ed->current = NULL;
            ed->total_baris = 0;

        } else {
            printf("\nInput tidak valid.\n");
            getch();
            ed->next = 1;
        }

    } else if (ed->isSaved == 1) {
            printf("Lanjut edit file atau kembali ke menu utama? (y/n): ");
            char lanjut = getch();

            if (lanjut == 'y' || lanjut == 'Y') {
                ed->next = 1;
                printLayar(ed, ed->b_now, ed->k_now);
            } else if (lanjut == 'n' || lanjut == 'N') {
                ed->next = 0;
                printf("\nKembali ke menu utama...");
                freeList(&ed->head); // Bebaskan memori sebelum kembali ke menu
                ed->head = NULL;
                ed->tail = NULL;
                ed->current = NULL;
                ed->total_baris = 0;
                
            } else {
                printf("\nInput tidak valid.\n");
                getch();
                ed->next = 1;
            }
        }
}
    
