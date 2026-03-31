#include <stdio.h>
#include <string.h>
#include "../header/file_s.h"
#include "../header/buffer.h"



void saveFile(Buffer *buff) 
{ 
    FILE *file = fopen(buff->namaFile, "w"); // Membuka file dengan mode tulis (write)

    if (file != NULL) 
    {
        for (int i = 0; i < buff->total_baris; i++) {
            fprintf(file, "%s\n", buff->teks[i]); // Menulis setiap baris teks ke dalam file 
        }
        
        buff->isSaved = 1; // Menandai bahwa file sudah disimpan
    } 
    else
    {
        printf("Gagal menyimpan file!\n");
    }
}

void shortcutSave(Buffer *buff) 
{
    if (buff->input == 19) 
    {
        saveFile(buff);
        buff->isSaved = 1; // Menandai bahwa file sudah disimpan
    }
}