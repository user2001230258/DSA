#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DESCRIPTION_LENGTH 100

typedef struct Expense{
    float amount;
    char description[DESCRIPTION_LENGTH];
    char date[11]; 
    struct Expense *next;
} Expense;

Expense* head = NULL;
            

// Ham them chi tieu
void add_expense() {
    Expense* newExpense = (Expense*)malloc(sizeof(Expense)); // Cap phat dong
    if (newExpense == NULL) {
       printf("Loi cap phat bo nho!\n");
       return;
    }     
    printf("Nhap so tien: ");
    scanf("%f", &newExpense->amount);
    printf("Nhap mo ta: ");
    getchar(); // xoa ky tu newline
    fgets(newExpense->description, DESCRIPTION_LENGTH, stdin);
    newExpense->description[strcspn(newExpense->description, "\n")] = 0; // Xoa ky tu newline
    printf("Nhap ngay (DD/MM/YYYY): ");
    scanf("%s", newExpense->date);
    newExpense->next = head;
    head = newExpense;
    printf("Da them chi tieu thanh cong!");
}

// Ham xem chi tieu
void view_expenses() {
    Expense* current = head;
    printf("Danh sach chi tieu:\n");
    while(current != NULL) {
        printf("# %s - %.2f VND - Ngay: %s\n", current->description, current->amount, current->date);
        current = current->next;
    }
}

// Ham tinh tong chi tieu
float total_expenses() {
    float total = 0;
    Expense* current = head;
    while(current != NULL) {
        total += current->amount;
        current = current->next;
    }
    return total;
}

// Ham xoa chi tieu
void delete_expense() {
    int index;
    printf("Nhap so thu tu chi tieu can xoa: ");
    scanf("%d", &index);

    if (index <= 0) {
        printf("So thu tu khong hop le!\n");
        return;
    }

    Expense* current = head;
    Expense* prev = NULL;


    for (int i = 1; current != NULL && i < index; i++) {
        prev = current;
        current = current->next;
    }

    if(current == NULL) {
        printf("Khong tim thay chi tieu tai so thu tu %d!\n", index);
        return;
    }

    if (prev == NULL) {
        head = current->next; // xoa node dau tien
    } else {
        prev->next = current->next; // xoa node o mid hoac cuoi
    }

    free(current);
    printf("Da xoa chi tieu thanh cong!\n");
}

// Ham chinh sua chi tieu
void edit_expense() {
    int index;
    printf("Nhap so thu tu chi tieu can sua: ");
    scanf("%d", &index);

    if (index <= 0) {
        printf("So thu tu khong hop le!");
        return;
    }

    Expense* current = head;

    for (int i = 1; current != NULL && i < index; i++) {
        current = current->next;
    }

    if (current == NULL) {
        printf("Khong tim thay chi tieu tai so thu tu %d!\n", index);
        return;
    }

    printf("Nhap so tien moi: ");
    scanf("%f", &current->amount);
    printf("Nhap mo ta moi: ");
    getchar();
    fgets(current->description, DESCRIPTION_LENGTH, stdin);
    current->description[strcspn(current->description, "\n")] = 0; // Xoa ky tu newline
    printf("Nhap ngay moi (DD/MM/YYYY): ");
    scanf("%s", current->date);
    printf("Da sua chi tieu thanh cong!\n");
}

// Ham luu chi tieu vao file
void save_expenses() {
    FILE *file = fopen("expenses.txt", "w");
    if (file) {
        Expense* current = head;
        while (current != NULL) {
            fprintf(file, "%.2f,%s,%s\n", current->amount, current->description, current->date);
            current = current->next;
        }
        fclose(file);
        printf("Da luu danh sach vao file thanh cong!\n");
    } else {
        printf("Khong the mo file de luu!\n");
    }
}

// Ham tai danh sach cac chi tieu
void load_expenses() {
    FILE *file = fopen("expenses.txt", "r");
    if (file) {
        while (!feof(file)) {
            Expense* newExpense = (Expense*)malloc(sizeof(Expense));
            if (newExpense == NULL) {
                printf("Loi cap phat bo nho!\n");
                break;
            }
            if(fscanf(file, "%f,%[^,],%s\n", &newExpense->amount, newExpense->description, newExpense->date) == 3) {
                newExpense->next = head;
                head = newExpense;
        } else {
            free(newExpense);
            break;
        }
    } 
    fclose(file);
    printf("Da tai danh sach chi tieu tu file thanh cong!\n");
    } else {
        printf("Khong the mo file de tai danh sach!\n");
    }
}

void menu() {
    int choice;
    do {
        printf("\n--- QUAN LY CHI TIEU CAC NHAN ---\n");
        printf("1. Them chi tieu\n");
        printf("2. Xem danh sach chi tieu\n");
        printf("3. Tinh tong chi tieu\n");
        printf("4. Xoa chi tieu\n");
        printf("5. Sua chi tieu\n");
        printf("6. Luu chi tieu vao file\n");
        printf("0. Thoat\n");
        printf("Chon chuc nang: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: add_expense(); break;
            case 2: view_expenses(); break;
            case 3: printf("Tong chi tieu: %.2f VND\n", total_expenses()); break;
            case 4: delete_expense(); break;
            case 5: edit_expense(); break;
            case 6: save_expenses(); break;
            case 0: printf("Thoat chuong trinh.\n"); break;
            default: printf("Chuc nang khong hop le. Vui long chon lai!\n");
        }
    } while (choice != 0);
}

int main() {
    load_expenses();
    menu();
    return 0;
}