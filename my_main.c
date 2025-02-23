#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "my_struct.h"
#include "my_funcs.h"
#define N 1


int main(){
    char * filename = "scooters.bin";
    #if N==1
        FILE *file = fopen(filename, "wb+");
        if (file == NULL) {
            perror("Ошибка открытия файла");
            return -1;
        }
        Scooter arr[5]={{"1","2019", "Whoosh","SUper1", "1005","Ready"},
        {"2","2020", "Whoosh","SUper2", "1006","Ready"},
        {"3","2021", "Whoosh","SUper3", "1007","Not Ready"},
        {"4","2022", "Whoosh","SUper4", "1008","Not Ready"},
        {"5","2023", "Whoosh","SUper5", "1009","Ready"}};
        for (int i=0; i<5;i++){
            fwrite(&arr[i], sizeof(Scooter), 1, file);
        }
        fclose(file);
    #endif
    menu(filename);
    return 0;
}
//     int id;
//     int year;
//     char brand[MAX_BRAND_SIZE], model[MAX_MODEL_SIZE];
//     float price;
//     char status[MAX_STATUS_SIZE];