#ifndef CURSOR_H
#define CURSOR_H

#include "buffer.h"

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define CTRL_LEFT 115
#define CTRL_RIGHT 116

void gotoXY(Buffer *buff, int x, int y);
void getSelectionBounds(int *b1, int *k1, int *b2, int *k2, int b_now, int k_now, int sel_b, int sel_k);
void cetakHighlight(Buffer *buff, int start_b, int start_k, int end_b, int end_k);
void deleteSelection(Buffer *buff, int sel_b, int sel_k);
void copyText(Buffer *buff, int sel_b, int sel_k);
void cutText(Buffer *buff, int sel_b, int sel_k);
void pasteText(Buffer *buff);
void moveUp(Buffer *buff);
void moveDown(Buffer *buff);
void moveLeft(Buffer *buff);
void moveRight(Buffer *buff);
void moveWordLeft(Buffer *buff);
void moveWordRight(Buffer *buff);

// Tambahkan fungsi ini: Pengganti editFile milik file_ec.c
void editorKursor(Buffer *buff);

#endif