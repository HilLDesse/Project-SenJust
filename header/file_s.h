#ifndef FILE_S_H
#define FILE_S_H

#include "buffer.h"

void saveFile(Buffer *buff); // Fungsi untuk menyimpan file yang diedit
void saveAS(Buffer *buff); // Fungsi untuk menyimpan file dengan nama baru (CTRL + SHIFT + S)
void autoSave(Buffer *buff); // Fungsi untuk autosave dengan shortcut (CTRL + T)

#endif
