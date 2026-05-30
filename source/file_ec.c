#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

#include "../header/screen.h"
#include "../header/file_ec.h"
#include "../header/file_s.h"
#include "../header/buffer.h"
#include "../header/cursor.h"
#include "../header/text_features.h"

void editFile(Buffer *buff) {

    if (buff->head == NULL) { // Jika buffer kosong, buat node pertama untuk memulai pengeditan
        Node *nodeAwal = buatNode();

        if (nodeAwal == NULL) {
            printf("Gagal inisialisasi buffer!\n");
            return;
        }

        buff->head = buff->tail = buff->current = nodeAwal;
        buff->total_baris = 1;
        buff->b_now = 0;
        buff->k_now = 0;
        
    } else { // Jika buffer sudah memiliki data, set posisi kursor ke akhir teks di baris terakhir
        buff->current = buff->tail;
        buff->b_now   = buff->total_baris - 1;
        buff->k_now   = strlen(buff->current->teks);
    }
    printLayar(buff, buff->b_now, buff->k_now);
    gotoXY(buff, buff->k_now, buff->b_now);

    while (1)
    {
        buff->input = getch();

        if (buff->input >= 32 && buff->input <= 126) // Karakter yang bisa dicetak
        {
            insertHuruf(buff, (char)buff->input);
            printLayar(buff, buff->b_now, buff->k_now);
            gotoXY(buff, buff->k_now, buff->b_now);
            buff->isSaved = 0;
        }
        else if (buff->input == 8)   // Backspace
        {
            deleteHuruf(buff);
            printLayar(buff, buff->b_now, buff->k_now);
            gotoXY(buff, buff->k_now, buff->b_now);
            buff->isSaved = 0;
        }
        else if (buff->input == 13)  // Enter
        {
            int angka_list = 0;
            if (sscanf(buff->current->teks, "%d. ", &angka_list) == 1 && strstr(buff->current->teks, ". ") != NULL) {
                NumberList(buff, angka_list);
            } else {
                newBaris(buff);
            }
            printLayar(buff, buff->b_now, buff->k_now);
            buff->isSaved = 0;
        }
        else if (buff->input == 6)  // Ctrl+F untuk fitur FindText
        {
            system("cls");
            findText(buff);
            
            printLayar(buff, buff->b_now, buff->k_now);
            gotoXY(buff, buff->k_now, buff->b_now);

            continue;
        }
        else if (buff->input == 19) // Save File (Ctrl+S)
        {
            saveFile(buff);
            buff->isSaved = 1;
        }
        else if (buff->input == 1) {
            saveAS(buff);
            buff->isSaved = 1;
        } 
        else if (buff->input == 20) // Tombol Ctrl+T
        {
            autoSave(buff);
        }
        else
        {
            editorKursor(buff);
        }
    }
}

void createFile(Buffer *buff)
{
    while (1) {
        printf("\nMasukkan nama file yang ingin dibuat (contoh: tugas.txt): ");
        fgets(buff->namaFile, 100, stdin);
        buff->namaFile[strcspn(buff->namaFile, "\n")] = 0;

        if (strlen(buff->namaFile) == 0) {
            printf("[PERINGATAN] Nama file tidak boleh kosong!\n");
            continue;
        }

        FILE *cekFile = fopen(buff->namaFile, "r");
        if (cekFile != NULL) {
            fclose(cekFile);
            printf("\n[PERINGATAN] File '%s' sudah ada! Gunakan nama lain.\n",
                   buff->namaFile);
            continue;
        }

        break;
    }

    FILE *file = fopen(buff->namaFile, "w");
    if (file != NULL)
    {
        Node *node = buff->head;
        while (node != NULL) {
            if (node->teks == NULL) { // Hindari dereference NULL jika node tidak lengkap
                node = node->next;
                continue;
            }
            fprintf(file, "%s\n", node->teks);
            node = node->next;
        }
        fclose(file);

        printf("File '%s' sukses dibuat!\n", buff->namaFile);
        buff->isSaved = 1;

        printf("Tekan tombol apa saja untuk mulai mengedit...\n");
        getch();
        editFile(buff);
    }
    else
    {
        perror("fopen");
        printf("Gagal membuat file!\n");
        getch();
    }
}