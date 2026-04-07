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

#define KEY_DELETE 83 

static char clipboard[10000] = "";

void gotoXY(int x, int y) {
    COORD coord;
    coord.X = x; 
    coord.Y = y; 
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
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
    if (start_b == -1) return;

    int b1, k1, b2, k2;
    getSelectionBounds(&b1, &k1, &b2, &k2, end_b, end_k, start_b, start_k);

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    for (int r = b1; r <= b2; r++) {
        int len = strlen(buff->teks[r]);
        int c_start = (r == b1) ? k1 : 0;
        int c_end = (r == b2) ? k2 : len; 

        for (int c = c_start; c < c_end; c++) {
            gotoXY(c, r);
            // Ubah warna menjadi Background Putih, Text Hitam (Efek Block)
            SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
            printf("%c", buff->teks[r][c]); 
        }
    }
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void copyText(Buffer *buff, int sel_b, int sel_k) {
    int b1, k1, b2, k2;
    getSelectionBounds(&b1, &k1, &b2, &k2, buff->b_now, buff->k_now, sel_b, sel_k);
    
    int clipIndex = 0;
    clipboard[0] = '\0'; 
    
    for (int r = b1; r <= b2; r++) {
        int len = strlen(buff->teks[r]);
        int c_start = (r == b1) ? k1 : 0;
        int c_end = (r == b2) ? k2 : len;
        
        for (int c = c_start; c < c_end; c++) {
            clipboard[clipIndex++] = buff->teks[r][c];
        }
        if (r < b2) clipboard[clipIndex++] = '\n';
    }
    clipboard[clipIndex] = '\0'; 
}

void deleteSelection(Buffer *buff, int sel_b, int sel_k) {
    int b1, k1, b2, k2;
    getSelectionBounds(&b1, &k1, &b2, &k2, buff->b_now, buff->k_now, sel_b, sel_k);
    
    buff->b_now = b2;
    buff->k_now = k2;
    
    while (buff->b_now > b1 || (buff->b_now == b1 && buff->k_now > k1)) {
        deleteHuruf(buff, &buff->b_now, &buff->k_now);
    }
}

void cutText(Buffer *buff, int sel_b, int sel_k) {
    copyText(buff, sel_b, sel_k); 
    deleteSelection(buff, sel_b, sel_k); 
}

void pasteText(Buffer *buff) {
    for (int i = 0; clipboard[i] != '\0'; i++) {
        if (clipboard[i] == '\n') {
            newBaris(buff, &buff->b_now, &buff->k_now);
        } else {
            insertHuruf(buff, &buff->b_now, &buff->k_now, clipboard[i]);
        }
    }
}

void moveLeft(Buffer *buff) {
    if (buff->k_now > 0) buff->k_now--; 
    else if (buff->b_now > 0) { 
        buff->b_now--;
        buff->k_now = strlen(buff->teks[buff->b_now]);
    }
}

void moveRight(Buffer *buff) {
    int panjang_baris = strlen(buff->teks[buff->b_now]);
    if (buff->k_now < panjang_baris) buff->k_now++; 
    else if (buff->b_now < buff->total_baris - 1) { 
        buff->b_now++;
        buff->k_now = 0;
    }
}

void moveUp(Buffer *buff) {
    if (buff->b_now > 0) {
        buff->b_now--; 
        int panjang_atas = strlen(buff->teks[buff->b_now]);
        if (buff->k_now > panjang_atas) buff->k_now = panjang_atas; 
    }
}

void moveDown(Buffer *buff) {
    if (buff->b_now < buff->total_baris - 1) {
        buff->b_now++; 
        int panjang_bawah = strlen(buff->teks[buff->b_now]);
        if (buff->k_now > panjang_bawah) buff->k_now = panjang_bawah;
    }
}

// 2. FUNGSI NAVIGASI CEPAT (CTRL + PANAH)
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

// 3. FUNGSI EDITOR UTAMA
void editorKursor(Buffer *buff)
{
    buff->input = getch();
    if (buff->input == -32 || buff->input == 224 || buff->input == 0) 
    {
        buff->arrow = getch(); 
        buff->isShiftPressed = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;

        if (buff->isShiftPressed && !buff->is_selecting) 
        {
            buff->is_selecting = 1;
            buff->sel_start_b = buff->b_now;
            buff->sel_start_k = buff->k_now;
        }
        if (buff->arrow == KEY_DELETE) 
        {
            if (buff->is_selecting) 
            {
                deleteSelection(buff, buff->sel_start_b, buff->sel_start_k);
                buff->is_selecting = 0; buff->sel_start_b = -1; buff->sel_start_k = -1;
            } else 
            {
                int old_b = buff->b_now, old_k = buff->k_now;
                moveRight(buff);
                if (buff->b_now != old_b || buff->k_now != old_k) 
                {
                    deleteHuruf(buff, &buff->b_now, &buff->k_now);
                }
            }
            buff->isSaved = 0;
        } 
        else 
        {
            if (!buff->isShiftPressed && buff->is_selecting)
            {
                buff->is_selecting = 0; 
                buff->sel_start_b = -1; buff->sel_start_k = -1;
            }
            switch (buff->arrow) {
                case KEY_UP: moveUp(buff); break;
                case KEY_DOWN: moveDown(buff); break;
                case KEY_LEFT: moveLeft(buff); break;
                case KEY_RIGHT: moveRight(buff); break;
                case CTRL_LEFT: moveWordLeft(buff); break;
                case CTRL_RIGHT: moveWordRight(buff); break;
            }
        }
        printLayar(buff, buff->b_now, buff->k_now);
        if (buff->is_selecting) cetakHighlight(buff, buff->sel_start_b, buff->sel_start_k, buff->b_now, buff->k_now);
            gotoXY(buff->k_now, buff->b_now); 
    }
    else if (buff->input == 3)
    {
        if (buff->is_selecting) 
        {
            copyText(buff, buff->sel_start_b, buff->sel_start_k);
            buff->is_selecting = 0; buff->sel_start_b = -1; buff->sel_start_k = -1;
            printLayar(buff, buff->b_now, buff->k_now);
            gotoXY(buff->k_now, buff->b_now);
        }
    }
    else if (buff->input == 24)
    {
        if (buff->is_selecting) 
        {
            cutText(buff, buff->sel_start_b, buff->sel_start_k);
            buff->is_selecting = 0; buff->sel_start_b = -1; buff->sel_start_k = -1;
            buff->isSaved = 0;
            printLayar(buff, buff->b_now, buff->k_now);
            gotoXY(buff->k_now, buff->b_now);
        }
    }
    else if (buff->input == 22)
    {
        if (buff->is_selecting) 
        {
            deleteSelection(buff, buff->sel_start_b, buff->sel_start_k);
            buff->is_selecting = 0; buff->sel_start_b = -1; buff->sel_start_k = -1;
        }
        pasteText(buff);
        buff->isSaved = 0;
        printLayar(buff, buff->b_now, buff->k_now);
        gotoXY(buff->k_now, buff->b_now);
    }
}