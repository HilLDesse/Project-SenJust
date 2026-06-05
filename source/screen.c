#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "../header/screen.h"
#include "../header/buffer.h"

#define MAX_ROWS 20

void printLayar(Buffer *buff, int baris_sekarang, int kolom_sekarang) {
    if (buff == NULL) return;

    // 1. Dapatkan lebar layar terminal OS secara real-time
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int term_width = 80; // Lebar default
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        term_width = csbi.srWindow.Right - csbi.srWindow.Left - 1;
    }
    if (term_width < 40) term_width = 80; // Failsafe

    // 2. Hitung Horizontal Scroll (Geser kanan-kiri)
    int page_width = term_width - 10;
    buff->col_offset = (kolom_sekarang / page_width) * page_width;

    // 3. Hitung Vertical Scroll (Geser atas-bawah)
    if (baris_sekarang < buff->row_offset) {
        buff->row_offset = baris_sekarang; 
    }
    else if (baris_sekarang >= buff->row_offset + MAX_ROWS) {
        buff->row_offset = baris_sekarang - MAX_ROWS + 1; 
    }

    printf("\033[H\033[J"); // Bersihkan layar

    if (buff->head == NULL) {
        puts("(buffer kosong)");
        for(int i = 1; i < MAX_ROWS; i++) printf("\n");
    } else {
        Node *node = buff->head;
        // Skip baris yang ada di atas layar (karena scroll ke bawah)
        for (int i = 0; i < buff->row_offset && node != NULL; i++) {
            node = node->next;
        }

        // 4. Cetak teks dengan pemotongan visual ke kanan
        for (int i = 0; i < MAX_ROWS; i++) {
            if (node != NULL) {
                int len = node->teks ? strlen(node->teks) : 0;
                
                // Hanya cetak bagian teks yang masuk dalam pandangan (kamera) saat ini
                if (len > buff->col_offset) {
                    int copy_len = len - buff->col_offset;
                    if (copy_len > term_width) copy_len = term_width; 
                    
                    // Mencetak sebagian string agar tidak melipat (wrap)
                    printf("%.*s\033[K\n", copy_len, node->teks + buff->col_offset);
                } else {
                    printf("\033[K\n"); 
                }
                node = node->next;
            } else {
                printf("\033[K\n"); 
            }
        }
    }

    printf("\n------------------------------------------------------------------------------------------------------\n");
    printf("| CTRL+S:Save | CTRL+A:SaveAs | CTRL+Q:Copy | CTRL+X:Cut | CTRL+B:Paste | CTRL+Z:Undo | CTRL+Y:Redo  |\n");
    printf("| CTRL+F:Find | CTRL+T:AutoSv | CTRL+Bksp:DelWord | Shift+Arr:Block | Ctrl+Arr:Jump | ESC:Keluar     |\n");
    printf("------------------------------------------------------------------------------------------------------\n");

    printf("Baris: %-4d | Kolom: %-4d | File: %s | Tekan ESC untuk keluar.\n", baris_sekarang + 1, kolom_sekarang + 1, buff->namaFile);
}