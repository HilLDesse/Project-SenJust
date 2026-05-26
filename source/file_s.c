#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include "../header/screen.h"
#include "../header/file_ec.h"
#include "../header/file_s.h"
#include "../header/buffer.h"
#include "../header/cursor.h"


int saveFile(Buffer *buff) 
{ 
    if (buff == NULL || strlen(buff->namaFile) == 0) { // Periksa apakah buffer valid dan nama file tidak kosong
        printf("Nama file tidak valid. Simpan dibatalkan.\n");
        return 0;
    }

    FILE *file = fopen(buff->namaFile, "w"); // Buka file untuk ditulis, jika file tidak ada maka akan dibuat baru
    if (file == NULL) { // Periksa apakah file berhasil dibuka
        perror("Gagal membuka file");
        return 0;
    }
 
    Node *node = buff->head; 
    while (node != NULL) { // Looping untuk menulis setiap baris ke file
        if (node->teks != NULL) {
            if (fprintf(file, "%s\n", node->teks) < 0) { // Periksa apakah penulisan berhasil
                fclose(file);
                return 0;
            }
        } else {
            fprintf(file, "\n"); // Jika teks NULL, tulis baris kosong
        }
        node = node->next;
    }
    fclose(file);
    buff->isSaved = 1; // Tandai bahwa file sudah disimpan
    return 1;
}

void saveAS(Buffer *buff)
{
    if (buff == NULL || buff->input != 1) { // Periksa apakah buffer valid dan input adalah CTRL + A
        return;
    }

    char namaoldFile[100];
    char namanewFile[100];
    strcpy(namaoldFile, buff->namaFile); // Simpan nama file lama untuk referensi jika penyimpanan gagal

    while (1) {
        printf("Masukkan nama file baru: ");
        if (fgets(namanewFile, sizeof(namanewFile), stdin) == NULL) { // Periksa apakah input berhasil dibaca
            printf("\n[PERINGATAN] Input nama file gagal.\n");
            return;
        }

        namanewFile[strcspn(namanewFile, "\n")] = '\0'; // Hapus newline yang mungkin terbaca dari input

        if (strlen(namanewFile) == 0) { // Periksa apakah nama file baru kosong
            printf("\n[PERINGATAN] Nama file tidak boleh kosong!\n\n");
            continue;
        }

        if (strcmp(namanewFile, namaoldFile) == 0) { // Periksa apakah nama file baru sama dengan nama file lama
            printf("\n[PERINGATAN] Nama file baru tidak boleh sama!\n\n");
            continue;
        }

        FILE *cekFile = fopen(namanewFile, "r"); // Cek apakah file dengan nama baru sudah ada
        if (cekFile != NULL) {
            fclose(cekFile);
            printf("\n[PERINGATAN] File '%s' sudah ada\n", namanewFile);
            continue;
        }

        break;
    }

    strcpy(buff->namaFile, namanewFile); // Update nama file di buffer dengan nama file baru
    if (!saveFile(buff)) { // Coba simpan file dengan nama baru, jika gagal kembalikan nama file lama
        printf("\nGagal menyimpan file '%s'\n", buff->namaFile);
        strcpy(buff->namaFile, namaoldFile);
        return;
    }

    printf("\nFile berhasil disimpan dan dibuka: %s\n", buff->namaFile);
    printf("Tekan tombol apapun untuk melanjutkan...");
    getch();
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