#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>

#include "../header/teks_features.h"
#include "../header/File_OC.h"

void findText(Buffer *ed) {

    char cari [100];
    printf("Cari kata yang ingin dicari: ");    
    fgets(cari, 100, stdin);
    cari[strcspn(cari, "\n")] = 0; // Membuang Newline

    if (strlen(cari) == 0) { // Memastikan Input tidak boleh kosong
        printf("Kata yang dicari tidak boleh kosong!.\n");
        printf("Tekan enter untuk kembali ke menu...");
        getch();
        return; // Kembali ke edit File
    }

    int ketemu = 0;
    for (int i = 0; i < ed->total_baris; i++) {
        if (strstr(ed->teks[i], cari) != NULL) {
            printf("Ditemukan di baris %d\n", i + 1);
            printf("Isi: %s\n",ed->teks[i]);
            ketemu = 1;
        }
    }
    if (!ketemu) { 
        printf("Kata tidak ditemukan!.\n");
    }

    printf("\nTekan tombol apapun untuk kembali ke menu...");
    getch();
}

void NumberList(Buffer *ed, int angka_sebelumnya) {
    newBaris(ed, &ed->b_now, &ed->k_now);

    char nomorBaru[20];
    sprintf(nomorBaru, "%d. ", angka_sebelumnya + 1);
    
    char teks_sementara[MAX_COL];
    strcpy(teks_sementara, nomorBaru);
    strcat(teks_sementara, ed->teks[ed->b_now]);

    strcpy(ed->teks[ed->b_now], teks_sementara);

    ed->k_now = strlen(nomorBaru);
}