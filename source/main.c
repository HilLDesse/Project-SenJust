#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "../header/buffer.h"
#include "../header/screen.h"
#include "../header/file_s.h"

int main() 
{
    Buffer buf;          
    initBuffer(&buf);   
    
    buf.total_baris = 1; 

    int b_now = 0; // Posisi baris saat ini
    int k_now = 0; // Posisi kolom saat ini
    char input;

    printLayar(&buf, b_now, k_now);

    // Looping untuk membaca input dari keyboard
    while (1) 
    {
        input = getch(); 

        if (input == 27) // Tombol ESC untuk keluar dari mode edit
        {          
            system("cls"); 
                        
            printf("Apakah Anda ingin menyimpan teks menjadi file baru? (y/n): ");
            char simpan = getch(); 
                        
            if (simpan == 'y' || simpan == 'Y') 
            {
                saveFile(&buf); 
                            
                printf("Tekan enter untuk kembali ke menu utama...");
                getchar();
                        
                break; 
            }
            else if (simpan == 'n' || simpan == 'N') 
            {
                printf("Perubahan tidak disimpan....\n");
                printf("Tekan enter untuk melanjutkan...");
                getchar();
                break; 
            }
        }
        else if (input == 8)  // Tombol Backspace untuk menghapus karakter
        { 
            deleteHuruf(&buf, &b_now, &k_now);
            printLayar(&buf, b_now, k_now);
        }
        else if (input == 13)  // Tombol Enter untuk membuat baris baru
        { 
            newBaris(&buf, &b_now, &k_now);
            printLayar(&buf, b_now, k_now);
        }
        else if (input >= 32 && input <= 126) // Menentukan karakter yang bisa diketik (Huruf, Angka, Spasi)
        {
            insertHuruf(&buf, &b_now, &k_now, input);
            printLayar(&buf, b_now, k_now);
        }
    }
    system("cls");
    return 0;
}