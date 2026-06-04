#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>
#include <windows.h> 

#include "../header/cursor.h"
#include "../header/screen.h"
#include "../header/buffer.h"
#include "../header/file_s.h"
#include "../header/undo_redo.h"
#include "../header/clipboard.h"

#define KEY_DELETE 83 

static int is_selecting = 0;
static int sel_start_b = -1;
static int sel_start_k = -1;

void gotoXY(Buffer *buff, int x, int y) {
    // Menggunakan ANSI Escape Code agar 100% sinkron dengan printLayar.
    // Catatan: Koordinat ANSI selalu dimulai dari angka 1, sehingga (y + 1) dan (x + 1).
    printf("\033[%d;%dH", y + 1, x + 1);
    
    // Memaksa sistem terminal untuk langsung memindahkan kursor saat itu juga
    fflush(stdout); 
}

void getSelectionBounds(int *b1, int *k1, int *b2, int *k2, int b_now, int k_now, int sel_b, int sel_k) {
    if (sel_b < b_now || (sel_b == b_now && sel_k < k_now)) {
        *b1 = sel_b; *k1 = sel_k;
        *b2 = b_now; *k2 = k_now;
    } else {
        *b1 = b_now; *k1 = k_now;
        *b2 = sel_b; *k2 = sel_k;
    }
}

void cetakHighlight(Buffer *buff, int start_b, int start_k, int end_b, int end_k) {
    // Jika tidak ada seleksi atau buffer kosong, batalkan
    if (start_b == -1 || buff->head == NULL) return; 

    int b1, k1, b2, k2;
    getSelectionBounds(&b1, &k1, &b2, &k2, end_b, end_k, start_b, start_k);

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // 1. Traversal: Jalan-jalan ke node awal baris yang diblok (b1)
    Node *curr = buff->head;
    for (int i = 0; i < b1 && curr != NULL; i++) {
        curr = curr->next;
    }

    // 2. Loop mewarnai per baris (berjalan menggunakan curr = curr->next)
    for (int r = b1; r <= b2 && curr != NULL; r++) {
        int len = strlen(curr->teks);
        int c_start = (r == b1) ? k1 : 0;
        int c_end = (r == b2) ? k2 : len; 

        // Mewarnai karakter dari titik c_start sampai c_end
        for (int c = c_start; c < c_end; c++) {
            gotoXY(buff, c, r);
            SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
            printf("%c", curr->teks[c]); 
        }
        
        curr = curr->next; // Pindah ke node baris selanjutnya
    }
    
    // Kembalikan warna terminal ke standar (Teks putih, background hitam)
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}
void moveLeft(Buffer *buff) {
    if (buff->k_now > 0) {
        buff->k_now--; // Jika masih di tengah baris, geser kursor ke kiri biasa
    } 
    // Jika kursor sudah mentok di kiri (index 0) dan baris atasnya ada:
    else if (buff->current != NULL && buff->current->prev != NULL) { 
        buff->current = buff->current->prev; // Lompat ke node baris atasnya
        buff->b_now--;
        buff->k_now = strlen(buff->current->teks); // Taruh kursor di ujung kanan teks tersebut
    }
}

void moveRight(Buffer *buff) {
    if (buff->current == NULL) return; // Failsafe

    int panjang_baris = strlen(buff->current->teks);
    if (buff->k_now < panjang_baris) {
        buff->k_now++; // Jika masih di tengah baris, geser kursor ke kanan biasa
    } 
    // Jika kursor mentok di kanan dan baris bawahnya ada:
    else if (buff->current->next != NULL) { 
        buff->current = buff->current->next; // Lompat ke node baris bawahnya
        buff->b_now++;
        buff->k_now = 0; // Taruh kursor di indeks 0 (paling kiri)
    }
}

void moveUp(Buffer *buff) {
    // Cek apakah kursor sedang tidak di baris paling atas (Node prev ada isinya)
    if (buff->current != NULL && buff->current->prev != NULL) {
        
        // Pindahkan "mata kursor" ke Node baris atasnya
        buff->current = buff->current->prev; 
        buff->b_now--; // Update angka status baris
        
        // Sesuaikan posisi kolom (jangan sampai kursor bablas di ruang kosong)
        int panjang_atas = strlen(buff->current->teks); // Minta ukuran teks di Node atas
        if (buff->k_now > panjang_atas) {
            buff->k_now = panjang_atas; 
        }
    }
}

void moveDown(Buffer *buff) {
    // Cek apakah node baris bawah (next) itu ada isinya
    if (buff->current != NULL && buff->current->next != NULL) {
        buff->current = buff->current->next; // Geser mata kursor ke baris bawah
        buff->b_now++; 
        
        // Sesuaikan kursor kolom agar tidak melebihi panjang teks baris baru
        int panjang_bawah = strlen(buff->current->teks);
        if (buff->k_now > panjang_bawah) {
            buff->k_now = panjang_bawah;
        }
    }
}

void moveWordLeft(Buffer *buff) {
    if (buff->current == NULL) return; // Failsafe

    // Mundur selama menemukan spasi
    while (buff->k_now > 0 && isspace(buff->current->teks[buff->k_now - 1])) {
        moveLeft(buff);
    }
    // Mundur selama menemukan huruf (bukan spasi)
    while (buff->k_now > 0 && !isspace(buff->current->teks[buff->k_now - 1])) {
        moveLeft(buff);
    }
    // Jika sudah di ujung kiri baris, lompat ke baris atasnya
    if (buff->k_now == 0 && buff->current->prev != NULL) {
        moveLeft(buff);
    }
}

void moveWordRight(Buffer *buff) {
    if (buff->current == NULL) return; // Failsafe

    int panjang_baris = strlen(buff->current->teks);
    
    // Maju selama menemukan huruf (bukan spasi)
    while (buff->k_now < panjang_baris && !isspace(buff->current->teks[buff->k_now])) {
        moveRight(buff);
        panjang_baris = strlen(buff->current->teks); 
    }
    // Maju selama menemukan spasi
    while (buff->k_now < panjang_baris && isspace(buff->current->teks[buff->k_now])) {
        moveRight(buff);
        panjang_baris = strlen(buff->current->teks);
    }
}

void deleteWordLeft(Buffer *buff) {
    if (buff->current == NULL) return; // Failsafe

    // Jika kursor berada di ujung kiri baris, lakukan backspace biasa (gabung ke baris atas)
    if (buff->k_now == 0) {
        deleteHuruf(buff);
        return;
    }

    // 1. Hapus spasi kosong yang ada tepat di belakang kursor (jika ada)
    while (buff->k_now > 0 && isspace(buff->current->teks[buff->k_now - 1])) {
        deleteHuruf(buff);
    }

    // 2. Hapus huruf-huruf pembentuk kata sampai menabrak spasi lagi atau mentok ke kiri
    while (buff->k_now > 0 && !isspace(buff->current->teks[buff->k_now - 1])) {
        deleteHuruf(buff);
    }
}

int hapusHighlightJikaAda(Buffer *buff) {
    if (is_selecting) {
        recordState(buff); // Simpan ke memory Undo agar bisa di CTRL+Z
        deleteSelection(buff, sel_start_b, sel_start_k);
        is_selecting = 0; 
        sel_start_b = -1; 
        sel_start_k = -1;
        return 1; 
    }
    return 0; 
}

// Lalu ganti fungsi editorKursor menjadi ini:
void editorKursor(Buffer *buff)
{
    // Deklarasi variabel lokal untuk menyimpan input tombol
    int arrow;
    int isShiftPressed;

    if (buff->input == -32 || buff->input == 224 || buff->input == 0) 
    {
        arrow = getch(); 
        isShiftPressed = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;

        if (isShiftPressed && !is_selecting) 
        {
            is_selecting = 1;
            sel_start_b = buff->b_now;
            sel_start_k = buff->k_now;
        }
        else if (arrow == KEY_DELETE) 
        {
            recordState(buff);
            if (is_selecting) 
            {
                deleteSelection(buff, sel_start_b, sel_start_k);
                is_selecting = 0; 
                sel_start_b = -1; 
                sel_start_k = -1;
            } else 
            {
                int old_b = buff->b_now, old_k = buff->k_now;
                moveRight(buff);
                if (buff->b_now != old_b || buff->k_now != old_k) 
                {
                    deleteHuruf(buff); 
                }
            }
            buff->isSaved = 0;
            checkAutoSave(buff);
        } 
        else 
        {
            if (!isShiftPressed && is_selecting)
            {
                is_selecting = 0; 
                sel_start_b = -1; 
                sel_start_k = -1;
            }
            switch (arrow) {
                case KEY_UP: moveUp(buff); break;
                case KEY_DOWN: moveDown(buff); break;
                case KEY_LEFT: moveLeft(buff); break;
                case KEY_RIGHT: moveRight(buff); break;
                case CTRL_LEFT: moveWordLeft(buff); break;
                case CTRL_RIGHT: moveWordRight(buff); break;
            }
        }
        printLayar(buff, buff->b_now, buff->k_now);
        if (is_selecting) cetakHighlight(buff, sel_start_b, sel_start_k, buff->b_now, buff->k_now);
        gotoXY(buff, buff->k_now, buff->b_now);
    }
    else if (buff->input == 17) // CTRL + Q (Copy)
    {
        if (is_selecting) 
        {
            copyText(buff, sel_start_b, sel_start_k);
            is_selecting = 0; 
            sel_start_b = -1; 
            sel_start_k = -1;
            printLayar(buff, buff->b_now, buff->k_now);
            gotoXY(buff, buff->k_now, buff->b_now);
        }
    }
    else if (buff->input == 24) // CTRL + X (Cut)
    {
        if (is_selecting) 
        {
            recordState(buff);
            cutText(buff, sel_start_b, sel_start_k);
            is_selecting = 0; 
            sel_start_b = -1; 
            sel_start_k = -1;
            buff->isSaved = 0;
            checkAutoSave(buff);
            printLayar(buff, buff->b_now, buff->k_now);
            gotoXY(buff, buff->k_now, buff->b_now);
        }
    }
    else if (buff->input == 2) // CTRL + B (Paste)
    {
        recordState(buff);
        if (is_selecting) 
        {
            deleteSelection(buff, sel_start_b, sel_start_k);
            is_selecting = 0; 
            sel_start_b = -1; 
            sel_start_k = -1;
        }
        pasteText(buff);
        buff->isSaved = 0;
        checkAutoSave(buff);
        printLayar(buff, buff->b_now, buff->k_now);
        gotoXY(buff, buff->k_now, buff->b_now);
    }
    else if (buff->input == 26) // CTRL + Z (Undo)
    {
        undo(buff);
        printLayar(buff, buff->b_now, buff->k_now);
        gotoXY(buff, buff->k_now, buff->b_now);
    }
    else if (buff->input == 25) // CTRL + Y (Redo)
    {
        redo(buff);
        printLayar(buff, buff->b_now, buff->k_now);
        gotoXY(buff, buff->k_now, buff->b_now);
    }
    else if (buff->input == 127) // CTRL + BACKSPACE
    {
        recordState(buff); // Simpan ke Undo history
        
        if (is_selecting) 
        {
            // Jika sedang highlight, hapus blokannya
            deleteSelection(buff, sel_start_b, sel_start_k);
            is_selecting = 0; 
            sel_start_b = -1; 
            sel_start_k = -1;
        }
        else 
        {
            // Jika tidak ada highlight, hapus satu kata ke belakang
            deleteWordLeft(buff);
        }
        
        buff->isSaved = 0;
        checkAutoSave(buff);
        
        printLayar(buff, buff->b_now, buff->k_now);
        gotoXY(buff, buff->k_now, buff->b_now);
    }
}