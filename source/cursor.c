#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>
#include <windows.h> // Wajib untuk GetAsyncKeyState dan SetConsoleTextAttribute

#include "../header/cursor.h"
#include "../header/screen.h"
#include "../header/buffer.h"
#include "../header/file_s.h"

// Fungsi memindahkan kursor visual
void gotoXY(int x, int y) {
    COORD coord;
    coord.X = x; 
    coord.Y = y; 
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Fungsi BARU: Untuk mewarnai teks yang sedang di-block
void cetakHighlight(Buffer *buff, int start_b, int start_k, int end_b, int end_k) {
    if (start_b == -1) return; // Jika tidak ada yang diseleksi, batalkan

    // Pastikan kita tahu mana titik awal (kiri-atas) dan akhir (kanan-bawah)
    int b1 = start_b, k1 = start_k;
    int b2 = end_b, k2 = end_k;

    // Jika seleksi mundur (dari kanan ke kiri / bawah ke atas), tukar posisinya
    if (b1 > b2 || (b1 == b2 && k1 > k2)) {
        b1 = end_b; k1 = end_k;
        b2 = start_b; k2 = start_k;
    }

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    for (int r = b1; r <= b2; r++) {
        int len = strlen(buff->teks[r]);
        int c_start = (r == b1) ? k1 : 0;
        int c_end = (r == b2) ? k2 : len; 

        for (int c = c_start; c < c_end; c++) {
            gotoXY(c, r);
            // Ubah warna menjadi Background Putih, Text Hitam (Efek Block)
            SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
            printf("%c", buff->teks[r][c]); // Cetak ulang hurufnya di atas cetakan temanmu
        }
    }
    // Kembalikan warna terminal ke standar (Text Putih, Background Hitam)
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

// ==========================================
// 1. FUNGSI PERGERAKAN KURSOR DASAR
// ==========================================
void moveLeft(Buffer *buff) {
    if (buff->k_now > 0) {
        buff->k_now--; 
    } else if (buff->b_now > 0) { 
        buff->b_now--;
        buff->k_now = strlen(buff->teks[buff->b_now]);
    }
}

void moveRight(Buffer *buff) {
    int panjang_baris = strlen(buff->teks[buff->b_now]);
    if (buff->k_now < panjang_baris) {
        buff->k_now++; 
    } else if (buff->b_now < buff->total_baris - 1) { 
        buff->b_now++;
        buff->k_now = 0;
    }
}

void moveUp(Buffer *buff) {
    if (buff->b_now > 0) {
        buff->b_now--; 
        int panjang_atas = strlen(buff->teks[buff->b_now]);
        if (buff->k_now > panjang_atas) {
            buff->k_now = panjang_atas; 
        }
    }
}

void moveDown(Buffer *buff) {
    if (buff->b_now < buff->total_baris - 1) {
        buff->b_now++; 
        int panjang_bawah = strlen(buff->teks[buff->b_now]);
        if (buff->k_now > panjang_bawah) {
            buff->k_now = panjang_bawah;
        }
    }
}

// ==========================================
// 2. FUNGSI NAVIGASI CEPAT (CTRL + PANAH)
// ==========================================
void moveWordLeft(Buffer *buff) {
    while (buff->k_now > 0 && isspace(buff->teks[buff->b_now][buff->k_now - 1])) moveLeft(buff);
    while (buff->k_now > 0 && !isspace(buff->teks[buff->b_now][buff->k_now - 1])) moveLeft(buff);
    if (buff->k_now == 0 && buff->b_now > 0) moveLeft(buff);
}

void moveWordRight(Buffer *buff) {
    int panjang_baris = strlen(buff->teks[buff->b_now]);
    while (buff->k_now < panjang_baris && !isspace(buff->teks[buff->b_now][buff->k_now])) {
        moveRight(buff);
        panjang_baris = strlen(buff->teks[buff->b_now]); 
    }
    while (buff->k_now < panjang_baris && isspace(buff->teks[buff->b_now][buff->k_now])) {
        moveRight(buff);
        panjang_baris = strlen(buff->teks[buff->b_now]);
    }
}

// ==========================================
// 3. FUNGSI EDITOR UTAMA
// ==========================================
void editorKursor(Buffer *buff)
{
    // Variabel Lokal untuk menampung status "Block"
    int is_selecting = 0;
    int sel_start_b = -1;
    int sel_start_k = -1;

    buff->b_now = 0; 
    buff->k_now = 0; 

    if (buff->total_baris > 0) {
        buff->b_now = buff->total_baris - 1; 
        buff->k_now = strlen(buff->teks[buff->b_now]); 
    } else {
        buff->total_baris = 1; 
    }

    system("cls"); 
    printLayar(buff, buff->b_now, buff->k_now); 
    gotoXY(buff->k_now, buff->b_now);

    while (1)
    {
        buff->input = getch();
        
        // --- DETEKSI TOMBOL ARAH PANAH (Extended Keys) ---
        if (buff->input == -32 || buff->input == 224 || buff->input == 0) 
        {
            int arrow = getch(); 

            // Cek apakah tombol Shift di-keyboard sedang ditekan (0x8000 adalah kodenya)
            int isShiftPressed = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;

            // Logika Blok Teks
            if (isShiftPressed && !is_selecting) {
                // Baru mulai nge-blok
                is_selecting = 1;
                sel_start_b = buff->b_now;
                sel_start_k = buff->k_now;
            } else if (!isShiftPressed && is_selecting) {
                // Shift dilepas, batalkan blok
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

            printLayar(buff, buff->b_now, buff->k_now);
            
            // Jika sedang menyeleksi, timpa layarnya dengan warna blok!
            if (is_selecting) {
                cetakHighlight(buff, sel_start_b, sel_start_k, buff->b_now, buff->k_now);
            }

            gotoXY(buff->k_now, buff->b_now); 
        }
        // --- DETEKSI KARAKTER BIASA ---
        else if (buff->input >= 32 && buff->input <= 126) 
        { 
            is_selecting = 0; // Ketik huruf biasa = batal blok
            insertHuruf(buff, &buff->b_now, &buff->k_now, buff->input);
            printLayar(buff, buff->b_now, buff->k_now);
            buff->isSaved = 0;
            gotoXY(buff->k_now, buff->b_now); 
        }
        else if (buff->input == 8)  // Backspace
        {
            is_selecting = 0; // Hapus huruf = batal blok
            deleteHuruf(buff, &buff->b_now, &buff->k_now);
            printLayar(buff, buff->b_now, buff->k_now);
            buff->isSaved = 0;
            gotoXY(buff->k_now, buff->b_now); 
        }
        else if (buff->input == 13) // Enter
        {
            is_selecting = 0; // Baris baru = batal blok
            newBaris(buff, &buff->b_now, &buff->k_now);
            printLayar(buff, buff->b_now, buff->k_now);
            buff->isSaved = 0;
            gotoXY(buff->k_now, buff->b_now); 
        }
        else if (buff->input == 27) // Tombol ESC
        {
            break; 
        }
        
        shortcutSave(buff);
        saveAS(buff);
    }
}