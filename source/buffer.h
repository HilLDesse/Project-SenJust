#ifndef BUFFER_H
#define BUFFER_H

#include <stdio.h>
#include <string.h>

#define MAX_LINE 100
#define MAX_COL 100

// Struktur data untuk mengelola Buffer
typedef struct 
{
    char teks[MAX_LINE][MAX_COL];
    int total_baris;
} Buffer;

// Fungsi untuk Buffer
void initBuffer(Buffer *buff); // Fungsi untuk mengosongkan memori dan menyiapkan buffer teks baru
void addBaris(Buffer *buff, char *input);  // Fungsi untuk menyalin teks input ke indeks array 2D yang masih kosong

#endif