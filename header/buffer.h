#ifndef BUFFER_H
#define BUFFER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef struct Node {
    char *teks;          // Pointer ke string yang menyimpan isi baris
    struct Node *prev;   // Pointer ke baris sebelumnya
    struct Node *next;   // Pointer ke baris berikutnya
} Node;

typedef struct {
    Node *head;          // Pointer ke baris pertama
    Node *tail;          // Pointer ke baris terakhir
    Node *current;       // Pointer ke baris saat ini

    int total_baris;     // Jumlah total baris dalam buffer
    int k_now;           // Posisi kolom saat ini
    int b_now;           // Posisi baris saat ini
    int next;            // Status untuk melanjutkan program setelah edit

    int input;           // Untuk menyimpan input karakter dari keyboard
    int isSaved;         // Status penyimpanan
    int autoSaveOn;      // Status autosave
    time_t lastSaveTime; // Waktu terakhir penyimpanan

    char namaFile[100];  // File yang sedang dibuka
    int row_offset;      // Untuk menyimpan offset baris saat ini (untuk fitur scroll)
} Buffer;

Node *buatNode();                         // Membuat node baru dengan string kosong
void freeList(Node **head);                // Membebaskan memori yang digunakan oleh seluruh node
void initBuffer(Buffer *buff);            // Menginisialisasi buffer dengan node pertama
void insertHuruf(Buffer *buff, char ch);  // Menyisipkan karakter di posisi kursor saat ini
void deleteHuruf(Buffer *buff);           // Menghapus karakter di posisi kursor saat ini
void newBaris(Buffer *buff);              // Menambahkan baris baru di bawah baris saat ini

#endif