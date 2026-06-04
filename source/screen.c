#include <stdio.h>
#include "../header/screen.h"
#include "../header/buffer.h"

void printLayar(Buffer *buff, int baris_sekarang, int kolom_sekarang) {
    if (buff == NULL) {
        return;
    }

    printf("\033[H\033[J");

    if (buff->head == NULL) {
        puts("(buffer kosong)");
    } else {
        for (Node *node = buff->head; node; node = node->next) {
            printf("%s\n", node->teks ? node->teks : "");
        }
    }

    printf("\n------------------------------------------------------------------------------------------------------\n");
    printf("|CTRL+S:Save | CTRL+Q:Copy | CTRL+B:Paste | CTRL+Z:Undo | CTRL+Y:Redo | CTRL+F:Find | CTRL+T:Autosave|\n");
    printf("------------------------------------------------------------------------------------------------------\n");

    const char *autoSaveStatus = buff->autoSaveOn ? "[AutoSave: ON ]" : "[AutoSave: OFF]";
    printf("Baris: %-4d | Kolom: %-4d | File: %-20s | %s | Tekan ESC untuk keluar.\n", baris_sekarang + 1, kolom_sekarang + 1, buff->namaFile, autoSaveStatus);
}