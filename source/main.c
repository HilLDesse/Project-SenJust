#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "../header/buffer.h"
#include "../header/screen.h"
#include "../header/file_ec.h"
#include "../header/file_s.h"
#include "../header/menu.h"

int main() 
{
    Buffer buff;          
    initBuffer(&buff);   
    
    buff.total_baris = 0; 

    buff.isSaved = 0; // Status file belum disimpan
    buff.namaFile[0] = '\0'; 

    tampilkanMenu(&buff);

    return 0;
}
