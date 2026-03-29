#include <stdio.h>
#include <stdlib.h>
#include "../header/screen.h"

void printLayar(Buffer *b, int baris_sekarang, int kolom_sekarang)
{
    system("cls"); // Menghapus seluruh teks di terminal

    for (int i = 0; i < b->total_baris; i++) // Membaca memori dari baris pertama hingga baris terakhir.
    {
        printf("%s\n", b->teks[i]); // Menampilkan isi tiap baris ke layar
    }
    
    printf("\n=======================================\n");
    printf("Tekan ESC untuk keluar aplikasi.\n");
    printf("\033[%d;%dH", baris_sekarang + 1, kolom_sekarang + 1); // Menempatkan cursor di posisi yang benar
}