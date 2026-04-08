#ifndef FILE_S_H
#define FILE_S_H

#include "buffer.h"

void saveFile(Buffer *buff); // Fungsi untuk menyimpan file yang diedit
void shortcutSave(Buffer *buff); // Fungsi untuk menyimpan file dengan shortcut (CTRL + S)
void saveAS(Buffer *buff); // Fungsi untuk menyimpan file dengan nama baru (CTRL + SHIFT + S)
void autoSave(Buffer *buff); // Fungsi untuk autosave dengan shortcut (CTRL + T)

#endif
