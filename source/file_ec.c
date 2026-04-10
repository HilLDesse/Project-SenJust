#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

#include "../header/screen.h"
#include "../header/file_s.h"
#include "../header/file_ec.h"
#include "../header/cursor.h"
#include "../header/File_OC.h"
#include "../header/teks_features.h"
#include "../header/undo_redo.h"

void editFile(Buffer *buff)
{
    buff->is_selecting = 0;
    buff->next = 1; // program terus berlanjut untuk close file
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
    gotoXY(buff, buff->k_now, buff->b_now);

    while (1)
    {
        buff->input = getch();
        
        if (buff->input >= 32 && buff->input <= 126) // Menentukan karakter yang bisa diketik (Huruf, Angka, Spasi)
        { 
            recordState(buff);
            if (buff->is_selecting) {
                deleteSelection(buff, buff->sel_start_b, buff->sel_start_k);
                buff->is_selecting = 0; buff->sel_start_b = -1; buff->sel_start_k = -1;
            }
            insertHuruf(buff, &buff->b_now, &buff->k_now, buff->input);
            printLayar(buff, buff->b_now, buff->k_now);
            buff->isSaved = 0;
            gotoXY(buff, buff->k_now, buff->b_now);
        }
        else if (buff->input == 8)  // Input Backspace untuk menghapus karakter
        {
            recordState(buff);
            if (buff->is_selecting) {
                deleteSelection(buff, buff->sel_start_b, buff->sel_start_k);
                buff->is_selecting = 0; buff->sel_start_b = -1; buff->sel_start_k = -1;
            }
            deleteHuruf(buff, &buff->b_now, &buff->k_now);
            printLayar(buff, buff->b_now, buff->k_now);
            buff->isSaved = 0;
            gotoXY(buff, buff->k_now, buff->b_now);
        }
        else if (buff->input == 13) // Input Enter untuk buat baris baru
        {
            int deteksi_angka = 0;
            recordState(buff);
            if (buff->is_selecting) {
                deleteSelection(buff, buff->sel_start_b, buff->sel_start_k);
                buff->is_selecting = 0; buff->sel_start_b = -1; buff->sel_start_k = -1;
            }

            if (sscanf(buff->teks[buff->b_now], "%d. ", &deteksi_angka) == 1) {
                NumberList(buff, deteksi_angka);
            } else {
                newBaris(buff, &buff->b_now, &buff->k_now);
            }

            printLayar(buff, buff->b_now, buff->k_now);
            buff->isSaved = 0;
            gotoXY(buff, buff->k_now, buff->b_now);
        } 
        else if(buff->input == 6) // Tombol F (Ctrl+F)
        {
            system("cls");
            findText(buff);

            system("cls");
            printLayar(buff, buff->b_now, buff->k_now);
            gotoXY(buff, buff->b_now, buff->k_now);
        }
        else if (buff->input == 27) // Tombol ESC
        {
            closeFile(buff);
            if(buff->next == 0){
                break;
            }
        }
        shortcutSave(buff);
        saveAS(buff);
        autoSave(buff);
        editorKursor(buff);
    }
}

void createFile(Buffer *buff)
{
    nama_file:
    printf("\nMasukkan nama file yang ingin dibuat (contoh: tugas.txt): "); 
    fgets(buff->namaFile, 100, stdin); 
    buff->namaFile[strcspn(buff->namaFile, "\n")] = 0; 

    FILE *cekFile = fopen(buff->namaFile, "r"); 
    if (cekFile != NULL)
    {
        fclose(cekFile);
        printf("\n[PERINGATAN] File sudah ada!\n");
        goto nama_file;
    }

    FILE *file = fopen(buff->namaFile, "w");
    if (file != NULL) 
    {
        for (int i = 0; i < buff->total_baris; i++) {
            fprintf(file, "%s\n", buff->teks[i]);
        }
    
        fclose(file); 
    
        printf("File '%s' sukses dibuat!\n", buff->namaFile);
        buff->isSaved = 1; 
    
        printf("Tekan enter untuk melanjutkan edit file '%s' \n", buff->namaFile);
        getch();
        editFile(buff);
    } 
    else
    {
        printf("Gagal membuat file!\n");
        getch();
    }
}