#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

#include "../header/screen.h"
#include "../header/file_s.h"
#include "../header/file_ec.h"

void editFile(Buffer *buff)
{
    buff->b_now = 0; // Posisi baris saat ini
    buff->k_now = 0; // Posisi kolom saat ini

    if (buff->total_baris > 0) {
        buff->b_now = buff->total_baris - 1; // Lompat ke baris terbawah
        buff->k_now = strlen(buff->teks[buff->b_now]); // Lompat ke ujung kanan teks
    }
    else
    {
        buff->total_baris = 1; // Jika file baru/kosong siapkan 1 baris
    }

    system("cls"); 
    printLayar(buff, buff->b_now, buff->k_now); 

    while (1)
    {
        buff->input = getch();
        
        if (buff->input >= 32 && buff->input <= 126) // Menentukan karakter yang bisa diketik (Huruf, Angka, Spasi)
        { 
            insertHuruf(buff, &buff->b_now, &buff->k_now, buff->input);
            printLayar(buff, buff->b_now, buff->k_now);
            buff->isSaved = 0;
        }
        else if (buff->input == 8)  // Input Backspace untuk menghapus karakter
        {
            deleteHuruf(buff, &buff->b_now, &buff->k_now);
            printLayar(buff, buff->b_now, buff->k_now);
            buff->isSaved = 0;
        }
        else if (buff->input == 13) // Input Enter untuk buat baris baru
        {
            newBaris(buff, &buff->b_now, &buff->k_now);
            printLayar(buff, buff->b_now, buff->k_now);
            buff->isSaved = 0;
        }
        else if (buff->input == 27 && buff->isSaved == 0) // Input Esc untuk keluar jika file belum disimpan
        {
            system("cls");
            printf("File belum disimpan, simpan sekarang? (y/n): ");
            char simpan = getch();
            if (simpan == 'y' || simpan == 'Y') 
            {
                saveFile(buff);
                printf("\nLanjut edit file atau kembali menu utama? (y/n): ");
                char lanjut = getch();
                if (lanjut == 'y' || lanjut == 'Y')
                {
                    printLayar(buff, buff->b_now, buff->k_now);
                    editFile(buff);
                }
                else if (lanjut == 'n' || lanjut == 'N')
                {
                    printf("Kembali ke menu utama...");
                    break; 
                }
            }
            else if (simpan == 'n' || simpan == 'N') 
            {
                printf("Perubahan tidak disimpan\n");
                printf("Lanjutkan kembali ke menu utama...");
                getchar();
                break; 
            }
            break; 
        }
        else if (buff->input == 27 && buff->isSaved == 1) // Input Esc untuk keluar jika file sudah disimpan
        {
            system("cls");
            printf("Lanjut edit file atau kembali menu utama? (y/n): ");
            char lanjut = getch();
            if (lanjut == 'y' || lanjut == 'Y')
            {
                printLayar(buff, buff->b_now, buff->k_now);
                editFile(buff);
            }
            else if (lanjut == 'n' || lanjut == 'N')
            {
                printf("\nLanjutkan kembali ke menu utama...");
                getchar();
                break; 
            }
        }
        shortcutSave(buff);
    }
}

void createFile(Buffer *buff)
{
    printf("\nMasukkan nama file yang ingin dibuat (contoh: tugas.txt): "); 
    fgets(buff->namaFile, 100, stdin); // Mengambil input nama file dari user
    buff->namaFile[strcspn(buff->namaFile, "\n")] = 0; // Menghapus /n dan menggantingan dengan 0 

    FILE *file = fopen(buff->namaFile, "w"); // Membuka file dengan mode write
    
    if (file != NULL) {
        for (int i = 0; i < buff->total_baris; i++) {
            fprintf(file, "%s\n", buff->teks[i]);
        }
    
        printf("File '%s' sukses dibuat!\n", buff->namaFile);
        buff->isSaved = 1; 
    
    
        printf("Tekan enter untuk menanjutkan edit file '%s' \n", buff->namaFile);
        getch();
        editFile(buff);
    } 
    else
    {
        printf("Gagal membuat file!\n");
    }
}
