#ifndef CURSOR_H
#define CURSOR_H

#include "buffer.h"

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define CTRL_LEFT 115
#define CTRL_RIGHT 116

void moveUp(Buffer *buff);
void moveDown(Buffer *buff);
void moveLeft(Buffer *buff);
void moveRight(Buffer *buff);
void moveWordLeft(Buffer *buff);
void moveWordRight(Buffer *buff);

// Tambahkan fungsi ini: Pengganti editFile milik file_ec.c
void editorKursor(Buffer *buff);

#endif