#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#include "../header/buffer.h"
#include "../header/screen.h"
#include "../header/file_ec.h"
#include "../header/file_s.h"
#include "../header/menu.h"

int main() {
    Buffer buff;

    buff.head    = NULL;
    buff.tail    = NULL;
    buff.current = NULL;

    initBuffer(&buff);

    buff.isSaved      = 0;
    buff.autoSaveOn   = 0;
    buff.lastSaveTime = time(NULL);
    buff.namaFile[0]  = '\0';

    tampilkanMenu(&buff);

    freeList(&buff.head);

    return 0;
}
