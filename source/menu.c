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
                
                // Mengambil alih logika "Buat File" agar tidak memakai file_ec.c
                printf("\nMasukkan nama file yang ingin dibuat (contoh: tugas.txt): "); 
                fgets(buff->namaFile, 100, stdin);
                buff->namaFile[strcspn(buff->namaFile, "\n")] = 0; 

                file = fopen(buff->namaFile, "w"); 
                if (file != NULL) {
                    fclose(file); // Langsung tutup karena hanya untuk memancing file terbuat di sistem
                    printf("File '%s' siap diedit!\n", buff->namaFile);
                    printf("Tekan enter untuk mulai mengedit...\n");
                    getch();
                    
                    // PANGGIL EDITOR KURSOR KITA, bukan editFile!
                    editorKursor(buff); 
                } else {
                    printf("Gagal membuat file!\n");
                    getch();
                }
                break;
                
            case 2:
                initBuffer(buff);
                buff->isSaved = 0; 
                buff->namaFile[0] = '\0';
                
                openFile(buff);
                
                if (buff->namaFile[0] != '\0') {
                    printf("\nTekan tombol apa saja untuk mulai mengedit file ini...\n");
                    getch();
                    
                    // PANGGIL EDITOR KURSOR KITA, bukan editFile!
                    editorKursor(buff);
                } else {
                    printf("\nTekan tombol apa saja untuk kembali ke menu...\n");
                    getch();
                }
                break;
                
            case 3:
                printf("\nKeluar dari aplikasi...\n");
                break;
                
            default:
                printf("\nPilihan tidak valid! Tekan tombol apa saja untuk mencoba lagi...\n");
                getch();
        }
    } while (pilihan != 3);
}