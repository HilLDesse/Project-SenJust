#include "../header/buffer.h"
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

void insertHuruf(Buffer *buff, int *baris, int *kolom, char ch)
{
    int panjang = strlen(buff->teks[*baris]); // Menghitung jumlah karakter di baris saat ini

    if (panjang >= MAX_COL - 1) 
    {
        return; // Agar karakter tidak melebihi batas kolom
    }

    for (int i = panjang; i >= *kolom; i--) 
    {
        buff->teks[*baris][i + 1] = buff->teks[*baris][i]; // Menggeser karakter di kanan kursor ke arah kanan
    }
    buff->teks[*baris][*kolom] = ch;
    (*kolom)++;
}

void deleteHuruf(Buffer *buff, int *baris, int *kolom)
{
    if (*kolom > 0) // Memastikan ada karakter di kiri yang bisa dihapus
    {
        int panjang = strlen(buff->teks[*baris]);

        for (int i = *kolom; i <= panjang; i++)
        {
            buff->teks[*baris][i - 1] = buff->teks[*baris][i]; // Menggeser karakter dari kanan ke kiri
        }
        (*kolom)--;
    } 
    else if (*baris > 0)
    {
        int long_atas = strlen(buff->teks[*baris - 1]); // Mencari panjang baris atas 

        strcat(buff->teks[*baris - 1], buff->teks[*baris]); // Menggabungkan teks baris saat ini ke ujung baris atas

        for (int i = *baris; i < buff->total_baris; i++)
        {
            strcpy(buff->teks[i], buff->teks[i + 1]); // Menggeser baris di bawahnya naik ke atas
        }
        buff->total_baris--;
        (*baris)--;
        *kolom = long_atas;
    }
}

void newBaris(Buffer *buff, int *baris, int *kolom) 
{
    if (buff->total_baris >= MAX_LINE) 
    {
        return; // Agar tidak melebihi batas baris
    }

    for (int i = buff->total_baris; i > *baris; i--) // Membuat ruang kosong di bawah baris kursor saat ini
    {
        strcpy(buff->teks[i], buff->teks[i - 1]);
    }

    strcpy(buff->teks[*baris + 1], &buff->teks[*baris][*kolom]); // Memindahkan sisa karakter dari kursor ke baris baru
    
    buff->teks[*baris][*kolom] = '\0';
    buff->total_baris++;
    (*baris)++;
    *kolom = 0;
}