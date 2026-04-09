#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "../header/menu.h"
#include "../header/buffer.h"
#include "../header/file_ec.h"
#include "../header/File_OC.h"
#include "../header/cursor.h"

void tampilkanShortcuts() {
    system("cls");
    printf("+=============================================================================+\n");
    printf("|                  * * * SHORTCUT SEN'JUST TEXT EDITOR * * * |\n");
    printf("+=============================================================================+\n");
    printf("|                                                                             |\n");
    printf("|    CTRL + Q       : Copy teks yang di highlight                             |\n");
    printf("|    CTRL + X       : Cut teks yang di highlight                              |\n");
    printf("|    CTRL + B       : Paste teks yang sudah di copy                           |\n");
    printf("|    CTRL + S       : Simpan File (Save)                                      |\n");
    printf("|    CTRL + A       : Simpan dengan nama baru (Save As)                       |\n");
    printf("|    CTRL + T       : Nyala / Matikan fitur AutoSave                          |\n");
    printf("|    CRTL + Z       : Undo (Membatalkan aksi terakhir)                        |\n");
    printf("|    CTRL + Y       : Redo (Mengembalikan aksi yang sudah di-undo)            |\n");
    printf("|    ESC            : Keluar dari Editor / Tutup File                         |\n");
    printf("|    CTRL + F       : Cari Teks (Find)                                        |\n");
    printf("|    Tombol Panah   : Memindahkan Kursor (Atas/Bawah/Kiri/Kanan)              |\n");
    printf("|    CTRL + Panah   : Memindahkan Kursor per kata (Word jump)                 |\n");
    printf("|    SHIFT + Panah  : Memblok / Sorot Teks (Highlight)                        |\n");
    printf("|                                                                             |\n");
    printf("+=============================================================================+\n\n");
    printf("  >> Tekan tombol apa saja untuk kembali... ");
    getch();
}

void tampilkanCredit() {
    system("cls");
    printf("+=============================================================================+\n");
    printf("|                   * * * CREDIT SEN'JUST TEXT EDITOR * * * |\n");
    printf("+=============================================================================+\n");
    printf("|                                                                             |\n");
    printf("|  POLITEKNIK NEGERI BANDUNG                                                  |\n");
    printf("|  TEKNIK INFORMATIKA - D3 TEKNIK INFORMATIKA                                 |\n");
    printf("|  TIM PENGEMBANG :                                                           |\n");
    printf("|    > Ahmad Aisy Fathryan Siswandar  - 251511033                             |\n");
    printf("|    > Hilmi Kautsar                  - 251511041                             |\n");
    printf("|    > Rafi Muhammad Rafi Ardiyansyah - 251511051                             |\n");
    printf("|                                                                             |\n");
    printf("|  Terima kasih telah menggunakan aplikasi kami!                              |\n");
    printf("|                                                                             |\n");
    printf("+=============================================================================+\n\n");
    printf("  >> Tekan tombol apa saja untuk kembali... ");
    getch();
}

void tampilkanMenu(Buffer *buff) 
{
    int pilihan;
    FILE *file;
    
    do {
        system("cls");
        printf("=======================================\n");
        printf("         SEN'JUST TEXT EDITOR          \n");
        printf("=======================================\n");
        printf("1. Buat File Baru\n");
        printf("2. Buka File\n");
        printf("3. Shortcuts\n");
        printf("4. Credit\n");
        printf("5. Keluar\n");
        printf("=======================================\n");
        printf("Pilih menu (1-5): ");
        
        scanf("%d", &pilihan);
        while(getchar() != '\n'); 

        switch(pilihan) {
            case 1:
                initBuffer(buff);
                buff->isSaved = 0;
                buff->namaFile[0] = '\0';
                createFile(buff);
                break;
                
            case 2:
                initBuffer(buff);
                buff->isSaved = 0; 
                buff->namaFile[0] = '\0';
                openFile(buff);
                break;

            case 3:
                tampilkanShortcuts();
                break;

            case 4:
                tampilkanCredit();
                break;
                
            case 5:
                printf("\nKeluar dari aplikasi...\n");
                break;
                
            default:
                printf("\nPilihan tidak valid! Tekan tombol apa saja untuk mencoba lagi...\n");
                getch();
        }
    } while (pilihan != 5);
}