#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../header/buffer.h"
#include "../header/clipboard.h"
#include "../header/cursor.h"

// Clipboard dinamis (ukurannya bisa tak terbatas) menggantikan array statis
static char *clipboardText = NULL;

void copyText(Buffer *buff, int sel_b, int sel_k) {
    int b1, k1, b2, k2;
    getSelectionBounds(&b1, &k1, &b2, &k2, buff->b_now, buff->k_now, sel_b, sel_k);

    // 1. Traverse (jalan-jalan) ke node awal baris yang diblok
    Node *curr = buff->head;
    for (int i = 0; i < b1 && curr != NULL; i++) {
        curr = curr->next;
    }

    // 2. Hitung total karakter untuk memesan memori malloc
    int total_len = 0;
    Node *temp = curr;
    for (int r = b1; r <= b2 && temp != NULL; r++) {
        int len = strlen(temp->teks);
        int c_start = (r == b1) ? k1 : 0;
        int c_end = (r == b2) ? k2 : len;
        total_len += (c_end - c_start) + 1; // +1 untuk Enter (Newline)
        temp = temp->next;
    }

    // 3. Bebaskan clipboard lama dan alokasikan memori baru
    if (clipboardText != NULL) free(clipboardText);
    clipboardText = (char *)malloc(total_len + 1); // +1 untuk Null Terminator
    if (clipboardText == NULL) return; // Failsafe jika RAM penuh
    clipboardText[0] = '\0';

    // 4. Salin teks karakter demi karakter
    int idx = 0;
    for (int r = b1; r <= b2 && curr != NULL; r++) {
        int len = strlen(curr->teks);
        int c_start = (r == b1) ? k1 : 0;
        int c_end = (r == b2) ? k2 : len;

        for (int c = c_start; c < c_end; c++) {
            clipboardText[idx++] = curr->teks[c];
        }
        if (r < b2) clipboardText[idx++] = '\n';
        curr = curr->next;
    }
    clipboardText[idx] = '\0'; // Tutup string
}

void deleteSelection(Buffer *buff, int sel_b, int sel_k) {
    int b1, k1, b2, k2;
    getSelectionBounds(&b1, &k1, &b2, &k2, buff->b_now, buff->k_now, sel_b, sel_k);
    
    // 1. Cari node akhir (baris ke-b2) untuk meletakkan kursor
    Node *curr = buff->head;
    for (int i = 0; i < b2 && curr != NULL; i++) {
        curr = curr->next;
    }
    
    // 2. Sinkronkan kursor saat ini ke posisi akhir blok
    buff->current = curr;
    buff->b_now = b2;
    buff->k_now = k2;
    
    // 3. Gunakan fungsi delete buatan temanmu hingga kursor mundur ke titik b1, k1
    while (buff->b_now > b1 || (buff->b_now == b1 && buff->k_now > k1)) {
        deleteHuruf(buff); 
    }
}

void cutText(Buffer *buff, int sel_b, int sel_k) {
    copyText(buff, sel_b, sel_k); 
    deleteSelection(buff, sel_b, sel_k); 
}

void pasteText(Buffer *buff) {
    if (clipboardText == NULL) return; // Batalkan jika belum ada yang di-copy
    
    // Looping untuk menyisipkan teks dari clipboard dinamis ke layar
    for (int i = 0; clipboardText[i] != '\0'; i++) {
        if (clipboardText[i] == '\n') {
            newBaris(buff); // Panggil fungsi Linked List temanmu
        } else {
            insertHuruf(buff, clipboardText[i]); // Panggil fungsi Linked List temanmu
        }
    }
}