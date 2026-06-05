#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>

#include "../header/text_features.h"
#include "../header/File_OC.h"

void findText(Buffer *ed) {

    char cari [100];
    printf("Cari kata yang ingin dicari (Case Sensitive) : ");    

    fgets(cari, sizeof(cari), stdin); 
    cari[strcspn(cari, "\n")] = 0; 

    if (strlen(cari) == 0) { 
        printf("Kata yang dicari tidak boleh kosong!.\n");
        printf("Tekan enter untuk kembali ke menu...");
        getch();
        return; // Kembali ke edit File
    }

    int ketemu = 0;
    int baris_index = 0;
    Node *curr = ed->head; // Mulai dari baris pertama
    while (curr != NULL) {
        char *posisi = strstr(curr->teks, cari);
        if (posisi != NULL) {
            ed->current = curr; // Pindahkan current ke baris yang ditemukan
            ed->b_now = baris_index; // Pindahkan kursor ke baris yang ditemukan

            ed->k_now = (posisi - curr->teks) + strlen(cari); // Pindahkan kursor ke posisi kata yang ditemukan
            printf("Kata ditemukan di baris %d\n", baris_index + 1);
            printf("Isi teks : %s\n", curr->teks);
            ketemu = 1;
            break;
        }
        curr = curr->next; // Lanjut ke baris berikutnya
        baris_index++;
    }
    
    if (!ketemu) { 
        printf("Kata tidak ditemukan!.\n");
    }

    printf("\nTekan tombol apapun untuk menuju kata yang ditemukan..");
    getch();
}

void NumberList(Buffer *ed, int angka_sebelumnya) {
    newBaris(ed);

    char nomorBaru[20];
    sprintf(nomorBaru, "%d. ", angka_sebelumnya + 1);
    
    // Hitung kebutuhan memori untuk nomor baru dan teks lama
    int panjang_nomor = strlen(nomorBaru);
    int panjang_teks_lama = strlen(ed->current->teks);

    // Alokasi memori dinamis untuk teks baru (nomor + teks lama)
    char *teks_baru = (char *)malloc(panjang_nomor + panjang_teks_lama + 1); // +1 untuk null terminator

    if (teks_baru == NULL) {
        printf("Memori penuh saat membuat list!\n");
        return;
    }

    // Gabungkan nomor baru dengan teks lama
    strcpy(teks_baru, nomorBaru);
    strcat(teks_baru, ed->current->teks);

    // Bebaskan memori teks lama dan ganti dengan teks baru
    free(ed->current->teks);

    ed->current->teks = teks_baru;
    ed->k_now = panjang_nomor; // Pindahkan kursor ke posisi setelah nomor 

    Node *traverse = ed->current->next;
    int nomor_berikutnya = angka_sebelumnya + 2; 

    while (traverse != NULL) {
        int deteksi_nomor = 0;

        if (sscanf(traverse->teks, "%d. ", &deteksi_nomor) == 1 && strstr(traverse->teks, ". ") != NULL) {
            // Misalnya jika angka aslinya "3. ", kita akan melewati 3 karakter ini untuk mendapatkan nomor berikutnya
            char old_nomor[20];
            sprintf(old_nomor, "%d. ", deteksi_nomor);

            // Ambil sisa teks aslinya (tepat setelah angka dan spasi)
            char *teks_asli = traverse->teks + strlen(old_nomor);

            char new_nomor[20];
            sprintf(new_nomor, "%d. ", nomor_berikutnya);

            // Alokasi memori untuk teks yang diperbarui (nomor baru + teks asli)
            char *update_teks = (char *)malloc(strlen(new_nomor) + strlen(teks_asli) + 1); // +1 untuk null terminator
            if (update_teks != NULL) {
                strcpy(update_teks, new_nomor);
                strcat(update_teks, teks_asli);

                // Replace teks lama dengan teks yang diperbarui
                free(traverse->teks);
                traverse->teks = update_teks;
            }
            nomor_berikutnya++;
            traverse = traverse->next;
        } else {
            break; 
        }
    }
}

void BulletedList(Buffer *ed, const char *bullet_style) {
    newBaris(ed);

    int panjang_bullet = strlen(bullet_style);
    int panjang_teks_lama = strlen(ed->current->teks);

    char *teks_baru = (char *)malloc(panjang_bullet + panjang_teks_lama + 1); // +1 untuk null terminator

    if (teks_baru == NULL) {
        printf("Memori penuh saat membuat bullet list!\n");
        return;
    }

    strcpy(teks_baru, bullet_style);
    strcat(teks_baru, ed->current->teks);

    free(ed->current->teks);
    ed->current->teks = teks_baru;

    ed->k_now = panjang_bullet; // Pindahkan kursor ke posisi setelah bullet
}