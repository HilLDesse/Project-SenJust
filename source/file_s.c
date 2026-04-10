#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include "../header/screen.h"
#include "../header/file_ec.h"
#include "../header/file_s.h"
#include "../header/buffer.h"
#include "../header/cursor.h"


void saveFile(Buffer *buff) 
{ 
    FILE *file = fopen(buff->namaFile, "w"); // Membuka file dengan mode tulis (write)

    if (file != NULL) 
    {
        for (int i = 0; i < buff->total_baris; i++) {
            fprintf(file, "%s\n", buff->teks[i]); // Menulis setiap baris teks ke dalam file 
        }
        
        fclose(file);
        buff->isSaved = 1; // Menandai bahwa file sudah disimpan
    } 
    else
    {
        printf("Gagal menyimpan file!\n");
    }
}

void shortcutSave(Buffer *buff) 
{
    if (buff->input == 19) 
    {
        saveFile(buff);
        buff->isSaved = 1; // Menandai bahwa file sudah disimpan
    }
}

void saveAS(Buffer *buff) 
{
    if (buff->input == 1)
    {
        system("cls");
        strcpy(buff->namaoldFile, buff->namaFile);
        input_nama_file: 
        printf("Masukkan nama file baru: ");
        fgets(buff->namanewFile, 100, stdin); // Mengambil input nama file dari user
        buff->namanewFile[strcspn(buff->namanewFile, "\n")] = 0; // Menghapus /n dan menggantingan dengan 0 
        if (strcmp(buff->namanewFile, buff->namaoldFile) == 0) 
        {
            printf("\n[PERINGATAN] Nama file baru tidak boleh sama!\n\n");
            goto input_nama_file; // Lompat kembali ke atas
        } 
        else if (strlen(buff->namanewFile) == 0) 
        {
            printf("\n[PERINGATAN] Nama file tidak boleh kosong!\n\n");
            goto input_nama_file; // Lompat kembali ke atas
        }

        FILE *cekFile = fopen(buff->namanewFile, "r");
        if (cekFile != NULL)
        {
            fclose(cekFile);
            
            printf("\n[PERINGATAN] File '%s' sudah ada\n", buff->namanewFile);
            goto input_nama_file; // Lompat kembali ke atas

        }

        strcpy(buff->namaFile, buff->namanewFile); // Menyalin file lama ke file baru
        saveFile(buff); // Menyimpan file dengan nama baru
        printf("\nFile berhasil disimpan\n");
        printf("Tekan 1 untuk edit file baru dan tekan 2 untuk edit file lama: ");

        input_pilihan: 
        printf("Pilihan: ");
        char pilihan = getch();
        if (pilihan == '1') 
        {
            printf("\n\nMembuka file '%s'...\n", buff->namaFile);
        } 
        else if (pilihan == '2') 
        {
            strcpy(buff->namaFile, buff->namaoldFile);
            printf("\n\nKembali ke file '%s'...\n", buff->namaFile);
        }
        else 
        {
            printf("\nPilihan tidak valid, tekan 1 atau 2\n");
            goto input_pilihan; // Lompat kembali ke pilihan
        }
        
        printf("Tekan tombol apapun untuk melanjutkan...");
        getch();
    }
}

void autoSave(Buffer *buff) 
{
    if (buff->input == 20) // Tombol Ctrl+T
    {
        buff->autoSaveOn = !buff->autoSaveOn; 
        
        system("cls"); 
        if (buff->autoSaveOn == 1) 
        {
            printf("Auto Save: ON\n");
            
            if (strlen(buff->namaFile) > 0) 
            {
                saveFile(buff);
            }
        } 
        else 
        {
            printf("Auto Save: OFF\n");
        }
        
        printf("\nTekan tombol apa saja untuk kembali mengedit...");
        getch(); 
        
        system("cls");
        printLayar(buff, buff->b_now, buff->k_now);
        
        gotoXY(buff, buff->k_now, buff->b_now); 
    }

    if (buff->autoSaveOn == 1 && buff->isSaved == 0 && strlen(buff->namaFile) > 0) 
    {
        saveFile(buff);
    }
}