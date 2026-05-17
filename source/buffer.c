#include "../header/buffer.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

Node *buatNode(void) {

    Node *node = malloc(sizeof *node); // Alokasi memori untuk node baru

    if (node == NULL){
        printf("Gagal alokasi memori\n");
        return NULL;
    }

    node->teks = malloc(1);

    if (node->teks == NULL) { 
        printf("Gagal alokasi memori\n");
        free(node); 
        return NULL; 
    }

    node->teks[0] = '\0'; // Inisialisasi string kosong
    node->prev = NULL;
    node->next = NULL;

    return node;
}

void freeList(Node **head) {

    if (head == NULL){
        return;
    }

    Node *sekarang = *head;

    while (sekarang != NULL) { // Looping untuk membebaskan setiap node
        Node *berikut = sekarang->next;
        free(sekarang->teks);
        free(sekarang);
        sekarang = berikut;
    }

    *head = NULL;
}

void initBuffer(Buffer *buff) {

    if (buff->head != NULL) {    // Jika buffer sudah memiliki data, bebaskan dulu memori yang digunakan
        freeList(&buff->head);
    }

    Node *nodeAwal = buatNode(); // Buat node pertama yang kosong
    buff->head = nodeAwal;       // Set head, tail, dan current ke node pertama
    buff->tail = nodeAwal;
    buff->current = nodeAwal;
    buff->total_baris = 1;
    buff->k_now = 0;
    buff->b_now = 0;
    buff->autoSaveOn = 0;
    buff->input = 0;
    buff->isSaved = 0;
    buff->namaFile[0] = '\0';
}

void addBaris(Buffer *buff, char *input) {

    Node *nodeBaru = buatNode(); // Buat node baru untuk baris yang akan ditambahkan
    int panjang = strlen(input); // Hitung panjang input untuk alokasi memori yang tepat
    char *temp = (char *)realloc(nodeBaru->teks, panjang + 1); // Alokasi memori untuk teks baru
    
    if (temp == NULL) {
        printf("Gagal alokasi memori!\n");
        free(nodeBaru->teks);
        free(nodeBaru);
        return;
    }

    nodeBaru->teks = temp;
    strncpy(nodeBaru->teks, input, panjang); // Salin input ke node baru
    nodeBaru->teks[panjang] = '\0'; // Pastikan string diakhiri dengan null terminator
    nodeBaru->prev = buff->tail;
    nodeBaru->next = NULL;

    if (buff->tail != NULL) {
        buff->tail->next = nodeBaru; // Sambungkan node baru ke akhir list
    } else {
        buff->head = nodeBaru; // Jika list kosong, set head ke node baru
    }

    buff->tail = nodeBaru;
    buff->total_baris++;
}

void insertHuruf(Buffer *buff, char ch) {

    if (buff->current == NULL){
        return;
    }

    int panjangLama = strlen(buff->current->teks); // Hitung panjang teks saat ini untuk alokasi memori baru
    int panjangBaru = panjangLama + 1;
    char *temp = (char *)realloc(buff->current->teks, panjangBaru + 1); // Alokasi memori untuk teks baru setelah penambahan karakter

    if (temp == NULL) {
        printf("Memori penuh!\n");
        return;
    }

    buff->current->teks = temp;

    int i = panjangBaru;
    while (i > buff->k_now) { // Geser karakter ke kanan untuk memberi ruang bagi karakter baru
        buff->current->teks[i] = buff->current->teks[i - 1]; 
        i--;
    }

    buff->current->teks[buff->k_now] = ch; // Sisipkan karakter baru di posisi kursor
    buff->current->teks[panjangBaru] = '\0'; // Pastikan string diakhiri dengan null terminator
    buff->k_now++; // Geser kursor ke kanan setelah penambahan karakter
}

void deleteHuruf(Buffer *buff) {

    if (buff->current == NULL){ // Jika tidak ada baris saat ini, tidak ada yang bisa dihapus
        return;
    }

    if (buff->k_now > 0) { // Jika kursor tidak di awal baris, hapus karakter di posisi kursor
        int panjang = strlen(buff->current->teks);
        int i = buff->k_now;

        while (i <= panjang) { // Geser karakter ke kiri untuk menutup ruang setelah penghapusan karakter
            buff->current->teks[i - 1] = buff->current->teks[i];
            i++;
        }

        buff->k_now--; // Geser kursor ke kiri setelah penghapusan karakter
        int ukBaru = panjang; // Hitung panjang baru setelah penghapusan karakter
        char *temp = (char *)realloc(buff->current->teks, ukBaru); // Alokasi memori baru untuk teks setelah penghapusan karakter
        
        if (temp != NULL){ // Jika alokasi memori berhasil, update pointer teks ke memori baru
            buff->current->teks = temp;
        }

    } else if (buff->current->prev != NULL) { // Jika kursor di awal baris dan ada baris di atasnya, gabungkan dengan baris di atasnya
        Node *barisAtas = buff->current->prev;
        Node *barisBawah = buff->current;

        int panjangAtas = strlen(barisAtas->teks);
        int panjangBawah = strlen(barisBawah->teks);
        int totalPanjang = panjangAtas + panjangBawah; // Hitung total panjang untuk alokasi memori baru setelah penggabungan kedua baris
        char *temp = (char *)realloc(barisAtas->teks, totalPanjang + 1); // Alokasi memori baru untuk teks setelah penggabungan kedua baris
        
        if (temp == NULL){
            return;
        }

        barisAtas->teks = temp; // 
        memcpy(barisAtas->teks + panjangAtas, barisBawah->teks, panjangBawah + 1); // Gabungkan teks baris bawah ke baris atas
        barisAtas->next = barisBawah->next; // Sambungkan baris atas ke baris setelah baris bawah
        
        if (barisBawah->next != NULL) { // Jika ada baris setelah baris bawah, sambungkan baris tersebut ke baris atas
            barisBawah->next->prev = barisAtas;
        } else { // Jika baris bawah adalah baris terakhir, update tail ke baris atas
            buff->tail = barisAtas;
        }

        free(barisBawah->teks); // Bebaskan memori teks dari baris bawah yang sudah digabungkan
        free(barisBawah); // Bebaskan memori node dari baris bawah yang sudah digabungkan
        
        buff->total_baris--;
        buff->current = barisAtas;
        buff->b_now--;
        buff->k_now = panjangAtas;
    }
}

void newBaris(Buffer *buff) {

    if (buff->current == NULL){ 
        return;
    }

    Node *nodeBaru = buatNode();

    if (nodeBaru == NULL) {
        return;
    }

    int sisa = strlen(buff->current->teks) - buff->k_now; // Hitung jumlah karakter yang tersisa setelah posisi kursor untuk dipindahkan ke baris baru
    char *temp = (char *)realloc(nodeBaru->teks, sisa + 1); // Alokasi memori untuk teks baru di baris baru

    if (temp == NULL) { // Jika alokasi memori gagal, bebaskan memori yang sudah dialokasikan untuk node baru dan keluar dari fungsi
        free(nodeBaru->teks);
        free(nodeBaru);
        return;
    }

    nodeBaru->teks = temp;
    memcpy(nodeBaru->teks, buff->current->teks + buff->k_now, sisa + 1); // Pindahkan karakter yang tersisa ke baris baru
    temp = (char *)realloc(buff->current->teks, buff->k_now + 1); // Alokasi memori baru untuk teks di baris saat ini
    
    if (temp == NULL) { 
        free(nodeBaru->teks);
        free(nodeBaru);
        return;
    }

    buff->current->teks = temp;
    buff->current->teks[buff->k_now] = '\0';
    nodeBaru->next = buff->current->next;
    nodeBaru->prev = buff->current;
    buff->current->next = nodeBaru;

    if (nodeBaru->next != NULL) { // Jika ada baris setelah baris baru, sambungkan baris tersebut ke baris baru
        nodeBaru->next->prev = nodeBaru;
    } else { // Jika baris baru adalah baris terakhir, update tail ke baris baru
        buff->tail = nodeBaru;
    }

    buff->total_baris++;
    buff->current = nodeBaru;
    buff->b_now++;
    buff->k_now = 0;
}