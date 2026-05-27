#include <string.h>
#include <stdlib.h>
#include "../header/undo_redo.h"

static Buffer undoStack[MAX_HISTORY];
static Buffer redoStack[MAX_HISTORY];
static int undoTop = -1;
static int redoTop = -1;

void initHistory() {
    // Bersihkan sisa memori di tumpukan jika ada (mencegah memory leak)
    while (undoTop >= 0) {
        freeList(&undoStack[undoTop].head);
        undoTop--;
    }
    while (redoTop >= 0) {
        freeList(&redoStack[redoTop].head);
        redoTop--;
    }
    
    undoTop = -1;
    redoTop = -1;
}

// --- FUNGSI HELPER: DEEP COPY ---
// Menyalin seluruh isi Linked List secara fisik dari 'src' ke 'dest'
static void copyBufferState(Buffer *dest, Buffer *src) {
    // 1. Salin variabel statis
    dest->total_baris = src->total_baris;
    dest->k_now = src->k_now;
    dest->b_now = src->b_now;
    dest->input = src->input;
    dest->isSaved = src->isSaved;
    dest->autoSaveOn = src->autoSaveOn;
    strcpy(dest->namaFile, src->namaFile);

    // 2. Kosongkan pointer Linked List di dest
    dest->head = NULL;
    dest->tail = NULL;
    dest->current = NULL;

    // 3. Traversal: Kloning setiap node satu per satu
    Node *srcNode = src->head;
    Node *prevDestNode = NULL;

    while (srcNode != NULL) {
        Node *newNode = buatNode(); // Fungsi dari buffer.c
        
        // Ganti isi teks bawaan dengan memori baru seukuran teks sumber
        free(newNode->teks);
        newNode->teks = (char *)malloc(strlen(srcNode->teks) + 1);
        strcpy(newNode->teks, srcNode->teks);

        // Sambungkan Node kloningan ke rantai dest
        if (dest->head == NULL) {
            dest->head = newNode;
        } else {
            prevDestNode->next = newNode;
            newNode->prev = prevDestNode;
        }

        // Simpan posisi mata kursor agar persis sama
        if (src->current == srcNode) {
            dest->current = newNode;
        }

        prevDestNode = newNode;
        srcNode = srcNode->next;
    }
    dest->tail = prevDestNode;
}

void recordState(Buffer *buff) {
    // Setiap kali user mengetik baru, masa depan (redo) dihancurkan dan memorinya dibersihkan
    while (redoTop >= 0) {
        freeList(&redoStack[redoTop].head);
        redoTop--;
    }

    if (undoTop < MAX_HISTORY - 1) {
        undoTop++;
        copyBufferState(&undoStack[undoTop], buff); // Simpan pakai Deep Copy
    } 
    else {
        // Stack penuh! Geser antrean.
        // WAJIB bebaskan memori node paling tua agar RAM tidak bocor (Memory Leak)
        freeList(&undoStack[0].head);
        
        for(int i = 0; i < MAX_HISTORY - 1; i++) {
            // Geser kotaknya saja (pointer berpindah, aman karena list ke-0 sudah di-free)
            undoStack[i] = undoStack[i + 1]; 
        }
        
        // Simpan state terbaru di posisi puncak
        copyBufferState(&undoStack[MAX_HISTORY - 1], buff);
    }
}

void undo(Buffer *buff) {
    if (undoTop >= 0) {
        // 1. Simpan keadaan layar saat ini ke Redo Stack
        redoTop++;
        copyBufferState(&redoStack[redoTop], buff);
        
        // 2. Hancurkan memori layar aktif saat ini
        freeList(&buff->head);
        
        // 3. Panggil memori dari Undo Stack dan kloning ke layar aktif
        copyBufferState(buff, &undoStack[undoTop]);
        
        // 4. Hancurkan memori di undo stack dan turunkan batas top
        freeList(&undoStack[undoTop].head);
        undoTop--;
    }
}

void redo(Buffer *buff) {
    if (redoTop >= 0) {
        // 1. Simpan keadaan layar saat ini ke Undo Stack
        undoTop++;
        copyBufferState(&undoStack[undoTop], buff);

        // 2. Hancurkan memori layar aktif saat ini
        freeList(&buff->head);

        // 3. Panggil memori dari Redo Stack dan kloning ke layar aktif
        copyBufferState(buff, &redoStack[redoTop]);

        // 4. Hancurkan memori di redo stack dan turunkan batas top
        freeList(&redoStack[redoTop].head);
        redoTop--;
    }
}