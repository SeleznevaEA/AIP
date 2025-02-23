#include "my_struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void show_all(char *filename);
void add_test_scooter(char* filename);
bool delete_scooter(char *filename);
void menu(char *filename);
void add_scooter(char *filename);
void search_scooter_records(char *filename);
void edit_scooter(char *filename);

long get_file_size(char* filename) {
    FILE * file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return -1; // Возвращаем -1 в случае ошибки
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);
    return size;
}

void clear_stdin_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void menu(char* filename) {
    int num, success;
    do {
        printf("choose:\n1)Add a scooter\n2)Delete\n3)Search\n4)Edit\n5)Show all\n6)Exit\n");
        do {
            success = scanf("%d", &num);
            clear_stdin_buffer();
            if ((success != 1) || (num > 6) || (num < 1)) {
                printf("Choose a NUMBER 1-6\n");
            }
        } while ((success != 1) || (num > 6) || (num < 1));

        switch (num) {
            case 1:
                int input, success;
                printf("enter how you want to input (1 - test_scooter; 2 - by hand)\n");
                do {
                    success = scanf("%d", &input);
                    clear_stdin_buffer();
                    if ((success != 1) || (input > 2) || (input < 1)) {
                    printf("Choose a NUMBER 1-2\n");
                    }
                } while ((success != 1) || (input > 2) || (input < 1));
                if (input == 2)
                    add_scooter(filename);
                else
                    add_test_scooter(filename);
                break;
            case 2:
                delete_scooter(filename);
                break;
            case 3:
                search_scooter_records(filename);
                break;
            case 4:
                edit_scooter(filename);
                break;
            case 5:
                show_all(filename);
                break;
            case 6:
                printf("Exiting program.\n");
                break;
        }
    } while (num != 6);
}

void show_all(char* filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return;
    }
    char *str = "------------------------------------------------------------------------------------";
    Scooter s;
    printf("%-15s%-15s%-15s%-15s%-15s%-15s\n", "ID", "YEAR", "BRAND", "MODEL", "PRICE", "STATUS");
    printf("%s\n", str );
    while (fread(&s, sizeof(Scooter), 1, file) == 1) {
        printf("%-15s%-15s%-15s%-15s%-15s%-15s\n", s.id, s.year, s.brand, s.model, s.price, s.status);
    }

    fclose(file);
}

void add_test_scooter(char* filename) {
    FILE *file = fopen(filename, "ab+"); // Открываем для добавления
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return;
    }
    Scooter s={"test_id", "test_year", "test_brand", "test_model", "test_price", "test_status"};
    fwrite(&s, sizeof(Scooter), 1, file);
    fclose(file);
}

void add_scooter(char* filename) {
    FILE *file = fopen(filename, "ab+"); // Открываем для добавления
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return;
    }

    Scooter s;
    printf("Enter id\n");
    fgets(s.id, sizeof(s.id), stdin);
    s.id[strcspn(s.id, "\n")] = 0;

    printf("Enter year\n");
    fgets(s.year, sizeof(s.year), stdin);
    s.year[strcspn(s.year, "\n")] = 0;


    printf("Enter brand\n");
    fgets(s.brand, sizeof(s.brand), stdin);
    s.brand[strcspn(s.brand, "\n")] = 0;

    printf("Enter model\n");
    fgets(s.model, sizeof(s.model), stdin);
    s.model[strcspn(s.model, "\n")] = 0;

    printf("Enter price\n");
    fgets(s.price, sizeof(s.price), stdin);
    s.price[strcspn(s.price, "\n")] = 0;

    printf("Enter status\n");
    fgets(s.status, sizeof(s.status), stdin);
    s.status[strcspn(s.status, "\n")] = 0;


    fwrite(&s, sizeof(Scooter), 1, file);
    fclose(file);
}



bool delete_scooter(char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file for reading");
        return false;
    }

    // Получаем размер файла и количество записей
    long fileSize = get_file_size(filename);
    long recordSize = sizeof(Scooter);
    long numRecords = fileSize / recordSize;

    if (numRecords == 0) {
        printf("The file is empty. Nothing to delete.\n");
        fclose(file);
        return true; // Считаем, что удаление прошло успешно (ничего не удалили, т.к. файл пустой)
    }

    printf("Enter ID to delete (1-%ld):\n", numRecords);
    int idToDelete;
    int success;

    do {
        success = scanf("%d", &idToDelete);
        clear_stdin_buffer();
        if ((success != 1) || (idToDelete < 1) || (idToDelete > numRecords)) {
            printf("Incorrect. Enter ID 1 - %ld:\n", numRecords);
        }
    } while ((success != 1) || (idToDelete < 1) || (idToDelete > numRecords));


    // Выделяем память для хранения всех записей
    Scooter *scooters = (Scooter *)malloc(numRecords * sizeof(Scooter));
    if (scooters == NULL) {
        perror("Error allocating memory");
        fclose(file);
        return false;
    }

    // Читаем все записи из файла
    size_t itemsRead = fread(scooters, recordSize, numRecords, file);
    fclose(file); // Закрываем файл после чтения

    if (itemsRead != numRecords) {
        fprintf(stderr, "Error reading data from the file. Read %zu out of %ld.\n", itemsRead, numRecords);
        free(scooters);
        return false;
    }


    // Открываем временный файл для записи
    FILE *tempFile = fopen("temp_file.bin", "wb");
    if (tempFile == NULL) {
        perror("Error opening temporary file");
        free(scooters);
        return false;
    }

    // Записываем все записи, кроме той, которую нужно удалить
    for (int i = 0; i < numRecords; i++) {
        if (i + 1 != idToDelete) {
            fwrite(&scooters[i], recordSize, 1, tempFile);
        }
    }

    fclose(tempFile);
    free(scooters);

    // Удаляем исходный файл
    if (remove(filename) != 0) {
        perror("Error deleting original file");
        return false;
    }

    // Переименовываем временный файл в имя исходного файла
    if (rename("temp_file.bin", filename) != 0) {
        perror("Error renaming temporary file");
        return false;
    }

    printf("Scooter with ID %d deleted successfully.\n", idToDelete);
    return true;
}
bool compare(char* field_name, char* search_value, Scooter scooter){
    bool match = false;

        if (strcmp(field_name, "id") == 0 && strcmp(scooter.id, search_value) == 0) {
            match = true;
        } else if (strcmp(field_name, "year") == 0) {
            match = (strcmp(scooter.year, search_value) == 0);
        } else if (strcmp(field_name, "brand") == 0) {
            match = (strcmp(scooter.brand, search_value) == 0);
        } else if (strcmp(field_name, "model") == 0) {
            match = (strcmp(scooter.model, search_value) == 0);
        } else if (strcmp(field_name, "price") == 0) {
            match = (strcmp(scooter.price, search_value) == 0);
        } else if (strcmp(field_name, "status") == 0) {
            match = (strcmp(scooter.status, search_value) == 0);
        }
    return match;
}
void search_scooter_records(char* filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return;
    }

    char field_name[20];
    char search_value[20];

    printf("Enter field name: ");
    fgets(field_name, sizeof(field_name), stdin);
    field_name[strcspn(field_name, "\n")] = 0;

    printf("Enter search value: ");
    fgets(search_value, sizeof(search_value), stdin);
    search_value[strcspn(search_value, "\n")] = 0;

    bool found = false;
    Scooter scooter;
    fseek(file, 0, SEEK_SET);

    printf("Search result:\n");
    bool (*pcompare)(char*,char*, Scooter) = compare;
    while(fread(&scooter, sizeof(Scooter), 1, file) != 0) {
        bool match = pcompare(field_name, search_value, scooter);

        if (match) {
            printf("%-15s%-15s%-15s%-15s%-15s%-15s\n", scooter.id, scooter.year,
                scooter.brand, scooter.model, scooter.price, scooter.status);
            found = true;
        }
    }

    if (!found) {
        printf("Записи не найдены.\n");
    }
}


void edit_scooter(char *filename) {
    FILE *file = fopen(filename, "rb+");  // Open in read-write binary mode
    if (file == NULL) {
        perror("Error opening file for reading and writing");
        return;
    }

    long fileSize = get_file_size(filename);
    long recordSize = sizeof(Scooter);
    long numRecords = fileSize / recordSize;

    if (numRecords == 0) {
        printf("The file is empty. Nothing to edit.\n");
        fclose(file);
        return;
    }

    printf("Enter № to edit (1-%ld):\n", numRecords);
    int idToEdit;
    int success;

    do {
        success = scanf("%d", &idToEdit);
        clear_stdin_buffer();
        if ((success != 1) || (idToEdit < 1) || (idToEdit > numRecords)) {
            printf("Incorrect. Enter № 1 - %ld:\n", numRecords);
        }
    } while ((success != 1) || (idToEdit < 1) || (idToEdit > numRecords));

    Scooter s;
    fseek(file, (idToEdit - 1) * recordSize, SEEK_SET);  // Go to the record to edit

    if (fread(&s, recordSize, 1, file) != 1) {
        perror("Error reading record for editing");
        fclose(file);
        return;
    }

    printf("Current record:\n");
    printf("%-15s%-15s%-15s%-15s%-15s%-15s\n", "ID", "YEAR", "BRAND", "MODEL", "PRICE", "STATUS");
    printf("%-15s%-15s%-15s%-15s%-15s%-15s\n", s.id, s.year, s.brand, s.model, s.price, s.status);

    printf("Enter new data:\n");

    printf("Enter id\n");
    fgets(s.id, sizeof(s.id), stdin);
    s.id[strcspn(s.id, "\n")] = 0;

    printf("Enter year\n");
    fgets(s.year, sizeof(s.year), stdin);
    s.year[strcspn(s.year, "\n")] = 0;

    printf("Enter brand\n");
    fgets(s.brand, sizeof(s.brand), stdin);
    s.brand[strcspn(s.brand, "\n")] = 0;

    printf("Enter model\n");
    fgets(s.model, sizeof(s.model), stdin);
    s.model[strcspn(s.model, "\n")] = 0;

    printf("Enter price\n");
    fgets(s.price, sizeof(s.price), stdin);
    s.price[strcspn(s.price, "\n")] = 0;

    printf("Enter status\n");
    fgets(s.status, sizeof(s.status), stdin);
    s.status[strcspn(s.status, "\n")] = 0;
    fseek(file, (idToEdit - 1) * recordSize, SEEK_SET);
    if (fwrite(&s, recordSize, 1, file) != 1) {
        perror("Error writing the edited record");
        fclose(file);
        return;
    }

    fclose(file);
    printf("Record with № %d edited successfully.\n", idToEdit);
}
