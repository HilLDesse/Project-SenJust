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

void gotoXY(int x, int y) {
    COORD coord;
    coord.X = x; // Kolom
    coord.Y = y; // Baris
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

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

void moveWordLeft(Buffer *buff) {
    while (buff->k_now > 0 && isspace(buff->teks[buff->b_now][buff->k_now - 1])) {
        moveLeft(buff);
    }
    while (buff->k_now > 0 && !isspace(buff->teks[buff->b_now][buff->k_now - 1])) {
        moveLeft(buff);
    }
    if (buff->k_now == 0 && buff->b_now > 0) {
        moveLeft(buff);
    }
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

void editorKursor(Buffer *buff)
{
    buff->b_now = 0; 
    buff->k_now = 0; 

    if (buff->total_baris > 0) {
        buff->b_now = buff->total_baris - 1; 
        buff->k_now = strlen(buff->teks[buff->b_now]); 
    }
    else {
        buff->total_baris = 1; 
    }

    system("cls"); 
    printLayar(buff, buff->b_now, buff->k_now); 
    
    // Pindahkan kursor visual ke posisi awal
    gotoXY(buff->k_now, buff->b_now);

    while (1)
    {
        buff->input = getch();
        
        // --- DETEKSI TOMBOL ARAH PANAH (Extended Keys) ---
        if (buff->input == -32 || buff->input == 224 || buff->input == 0) 
        {
            int arrow = getch(); 
            switch (arrow) {
                case KEY_UP: moveUp(buff); break;
                case KEY_DOWN: moveDown(buff); break;
                case KEY_LEFT: moveLeft(buff); break;
                case KEY_RIGHT: moveRight(buff); break;
                case CTRL_LEFT: moveWordLeft(buff); break;
                case CTRL_RIGHT: moveWordRight(buff); break;
            }
            printLayar(buff, buff->b_now, buff->k_now);
            gotoXY(buff->k_now, buff->b_now); // Kunci: Pindahkan kursor lagi!
        }
        // --- DETEKSI KARAKTER BIASA ---
        else if (buff->input >= 32 && buff->input <= 126) 
        { 
            insertHuruf(buff, &buff->b_now, &buff->k_now, buff->input);
            printLayar(buff, buff->b_now, buff->k_now);
            buff->isSaved = 0;
            gotoXY(buff->k_now, buff->b_now); // Kunci: Pindahkan kursor lagi!
        }
        else if (buff->input == 8)  // Backspace
        {
            deleteHuruf(buff, &buff->b_now, &buff->k_now);
            printLayar(buff, buff->b_now, buff->k_now);
            buff->isSaved = 0;
            gotoXY(buff->k_now, buff->b_now); // Kunci: Pindahkan kursor lagi!
        }
        else if (buff->input == 13) // Enter
        {
            newBaris(buff, &buff->b_now, &buff->k_now);
            printLayar(buff, buff->b_now, buff->k_now);
            buff->isSaved = 0;
            gotoXY(buff->k_now, buff->b_now); // Kunci: Pindahkan kursor lagi!
        }
        else if (buff->input == 27) // Tombol ESC untuk keluar
        {
            break; 
        }
        
        shortcutSave(buff);
        saveAS(buff);
    }
}

