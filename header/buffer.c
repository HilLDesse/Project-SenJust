#include "../source/buffer.h"
#include <conio.h> 

void initBuffer(Buffer *buff) 
{
    buff->total_baris = 0; // Set total baris ke 0

    for(int i = 0; i < MAX_LINE; i++) 
    {
        memset(buff->teks[i], 0, MAX_COL); // Mengisi setiap kolom dengan karakter null (0) agar bersih dari sisa data lama
    }
}

void addBaris(Buffer *buff, char *input)
{
    strcpy(buff->teks[buff->total_baris], input); // Menyalin string dari input ke baris paling akhir yang tersedia di array
    buff->total_baris++; 
}