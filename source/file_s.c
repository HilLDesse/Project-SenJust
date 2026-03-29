#include <stdio.h>
#include <string.h>
#include "../header/file_s.h"
#include "../header/buffer.h"

void saveFile(Buffer *buff) {
    
    printf("\nMasukkan nama file yang ingin dibuat (contoh: tugas.txt): "); 
    fgets(buff->namaFile, 100, stdin); // Mengambil input nama file dari user
    
    buff->namaFile[strcspn(buff->namaFile, "\n")] = 0; // Menghapus /n dan menggantingan dengan 0 
    
    FILE *file = fopen(buff->namaFile, "w"); // Membuat file yang baru atau menimpa file yang sudah ada dan membuka file tersebut untuk ditulis

    if (file != NULL) 
    {
        for (int i = 0; i < buff->total_baris; i++) {
            fprintf(file, "%s\n", buff->teks[i]); // Menulis setiap baris teks ke dalam file 
        }
        
        fclose(file); 
        
        buff->isSaved = 1; // Tanda bahwa file sudah disimpan
        printf("File '%s' berhasi sukses dibuat dan disimpan!\n", buff->namaFile);
    } 
    else 
    {
        printf("Gagal membuat file \n");
    }
}
