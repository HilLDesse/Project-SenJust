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
        printf("Masukkan nama file baru: ");
        fgets(buff->namanewFile, 100, stdin); // Mengambil input nama file dari user
        buff->namanewFile[strcspn(buff->namanewFile, "\n")] = 0; // Menghapus /n dan menggantingan dengan 0 
        strcpy(buff->namaFile, buff->namanewFile); // Menyalin file lama ke file baru
        saveFile(buff); // Menyimpan file dengan nama baru
        strcpy(buff->namaFile, buff->namaoldFile);
        printf("\nKembali ke file sebelumnya");
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