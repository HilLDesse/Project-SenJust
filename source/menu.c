#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "../header/menu.h"
#include "../header/buffer.h"
#include "../header/file_ec.h"
#include "../header/File_OC.h"
#include "../header/cursor.h"

void tampilkanMenu(Buffer *buff) 
{
    int pilihan;
    FILE *file;
    
    do {
        system("cls");
        printf("=======================================\n");
        printf("        APLIKASI TEXT EDITOR           \n");
        printf("=======================================\n");
        printf("1. Buat File Baru\n");
        printf("2. Buka File\n");
        printf("3. Keluar\n");
        printf("=======================================\n");
        printf("Pilih menu (1-3): ");
        
        scanf("%d", &pilihan);
        while(getchar() != '\n'); 

        switch(pilihan) {
            case 1:
                initBuffer(buff);
                buff->isSaved = 0;
                buff->namaFile[0] = '\0';
                createFile(buff);
                
            case 2:
                initBuffer(buff);
                buff->isSaved = 0; 
                buff->namaFile[0] = '\0';
                
                openFile(buff);
                
            case 3:
                printf("\nKeluar dari aplikasi...\n");
                break;
                
            default:
                printf("\nPilihan tidak valid! Tekan tombol apa saja untuk mencoba lagi...\n");
                getch();
        }
    } while (pilihan != 3);
}