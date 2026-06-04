#include <stdio.h>
#include "../header/screen.h"
#include "../header/buffer.h"
#include "../header/cursor.h"

static void printWrappedLine(const char *text, int width) {
    if (text == NULL) {
        text = "";
    }

    if (width <= 0) {
        width = 80;
    }

    int len = (int)strlen(text);
    if (len == 0) {
        putchar('\n');
        return;
    }

    for (int pos = 0; pos < len; pos += width) {
        int chunk = len - pos;
        if (chunk > width) {
            chunk = width;
        }
        printf("%.*s\n", chunk, text + pos);
    }
}

void printLayar(Buffer *buff, int baris_sekarang, int kolom_sekarang) {
    if (buff == NULL) {
        return;
    }

    printf("\033[H\033[J");

    int cols = getTerminalWidth();

    if (buff->head == NULL) {
        puts("(buffer kosong)");
    } else {
        for (Node *node = buff->head; node; node = node->next) {
            printWrappedLine(node->teks, cols);
        }
    }

    printf("\n------------------------------------------------------------------------------------------------------\n");
    printf("|CTRL+S:Save | CTRL+Q:Copy | CTRL+B:Paste | CTRL+Z:Undo | CTRL+Y:Redo | CTRL+F:Find | CTRL+T:Autosave|\n");
    printf("------------------------------------------------------------------------------------------------------\n");

    const char *namaFile = buff->namaFile[0] != '\0' ? buff->namaFile : "[Untitled]";
    printf("Baris: %-4d | Kolom: %-4d | File: %s | Tekan ESC untuk keluar.\n", baris_sekarang + 1, kolom_sekarang + 1, namaFile);
}