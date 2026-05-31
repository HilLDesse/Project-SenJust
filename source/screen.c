#include <stdio.h>
#include "../header/screen.h"
#include "../header/buffer.h"

void printLayar(Buffer *buff, int baris_sekarang, int kolom_sekarang) {
    printf("\033[H\033[J");

    for (Node *node = buff->head; node; node = node->next) {
        printf("%s\n", node->teks);
    }

    printf("\n------------------------------------------------------------------------------------------------------\n");
    printf("|CTRL+S:Save | CTRL+Q:Copy | CTRL+B:Paste | CTRL+Z:Undo | CTRL+Y:Redo | CTRL+F:Find | CTRL+T:Autosave|\n");
    printf("------------------------------------------------------------------------------------------------------\n");

    printf("Baris: %-4d | Kolom: %-4d | File: %s | Tekan ESC untuk keluar.\n", baris_sekarang + 1, kolom_sekarang + 1, buff->namaFile);
}