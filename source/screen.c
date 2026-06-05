#include <stdio.h>
#include "../header/screen.h"
#include "../header/buffer.h"

#define MAX_ROWS 20

void printLayar(Buffer *buff, int baris_sekarang, int kolom_sekarang) {
    if (buff == NULL) {
        return;
    }

    if (baris_sekarang < buff->row_offset) {
        buff->row_offset = baris_sekarang; 
    }
    else if (baris_sekarang >= buff->row_offset + MAX_ROWS) {
        buff->row_offset = baris_sekarang - MAX_ROWS + 1; 
    }

    printf("\033[H\033[J");


    if (buff->head == NULL) {
        puts("(buffer kosong)");
        for(int i = 1; i < MAX_ROWS; i++) {
            printf("\n");
        }
    } else {
        Node *node = buff->head;
        for (int i = 0; i < buff->row_offset && node != NULL; i++) {
            node = node->next;
        }

        for (int i = 0; i < MAX_ROWS; i++) {
            if (node != NULL) {
                printf("%s\033[K\n", node->teks ? node->teks : "");
                node = node->next;
            } else {
                printf("\033[K\n"); 
            }
        }
    }

    printf("\n------------------------------------------------------------------------------------------------------\n");
    printf("|CTRL+S:Save | CTRL+Q:Copy | CTRL+B:Paste | CTRL+Z:Undo | CTRL+Y:Redo | CTRL+F:Find | CTRL+T:Autosave|\n");
    printf("------------------------------------------------------------------------------------------------------\n");

    const char *autoSaveStatus = buff->autoSaveOn ? "[AutoSave: ON ]" : "[AutoSave: OFF]";
    printf("Baris: %-4d | Kolom: %-4d | File: %-20s | %s | Tekan ESC untuk keluar.\n", baris_sekarang + 1, kolom_sekarang + 1, buff->namaFile, autoSaveStatus);
}