#include "../header/buffer.h"
#include <conio.h> 

void initBuffer(Buffer *buff) 
{
    buff->total_baris = 0; // Set total baris ke 0
    buff->autoSaveOn = 0; // Set autoSaveOn ke 0 atau mati

    for(int i = 0; i < MAX_LINE; i++) 
    {
        memset(buff->teks[i], 0, MAX_COL); // Mengisi setiap kolom dengan karakter null (0) agar bersih dari sisa data lama
    }
}

void addBaris(Buffer *buff, char *input)
{
    if (buff->total_baris >= MAX_LINE) {
        return; // Hentikan penambahan baris
    }
    strncpy(buff->teks[buff->total_baris], input, MAX_COL - 1); // Salin input ke baris saat ini dengan batas maksimal kolom
    buff->teks[buff->total_baris][MAX_COL - 1] = '\0';
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
        int long_bawah = strlen(buff->teks[*baris]); // Mencari panjang baris bawah
        int kapasitas_sisa = (MAX_COL - 1) - long_atas; // Menghitung kapasitas sisa di baris atas 

        if (long_bawah <= kapasitas_sisa) 
        {
            strcat(buff->teks[*baris - 1], buff->teks[*baris]); 
            for (int i = *baris; i < buff->total_baris - 1; i++) 
            {
                strcpy(buff->teks[i], buff->teks[i + 1]); 
            }
            
            memset(buff->teks[buff->total_baris - 1], 0, MAX_COL);

            buff->total_baris--;
            (*baris)--;
            *kolom = long_atas;
        } 
        else if (kapasitas_sisa > 0)
        {
            strncat(buff->teks[*baris - 1], buff->teks[*baris], kapasitas_sisa);
            int sisa_panjang = long_bawah - kapasitas_sisa;
            for (int i = 0; i <= sisa_panjang; i++) 
            {
                buff->teks[*baris][i] = buff->teks[*baris][i + kapasitas_sisa]; 
            }
            (*baris)--;
            *kolom = long_atas;
        }
        else 
        {
            (*baris)--;
            *kolom = long_atas;
        }
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