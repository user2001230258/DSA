#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define DESCRIPTION_LENGTH 100

typedef struct Expense {
    float amount;
    char description[DESCRIPTION_LENGTH];
    char date[11]; 
    struct Expense *next;
} Expense;

Expense* head = NULL;

int dateToInt(const char* date) {
    int day, month, year;
    sscanf(date, "%d/%d/%d", &day, &month, &year);
    return year * 10000 + month * 100 + day;
}    

bool is_valid_date(int day, int month, int year) {
    if (month < 1 || month > 12) return false;
    if (day < 1) return false;
    
    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
        if (day > 29) return false;
    } else {
        if (day > days_in_month[month - 1]) return false;
    }
    
    return true;
}

void add_expense() {
    Expense* newExpense = (Expense*)malloc(sizeof(Expense));
    if (newExpense == NULL) {
       printf("Loi cap phat bo nho!\n");
       return;
    }     
    printf("Nhap so tien: ");
    scanf("%f", &newExpense->amount);
    printf("Nhap mo ta: ");
    getchar(); 
    fgets(newExpense->description, DESCRIPTION_LENGTH, stdin);
    newExpense->description[strcspn(newExpense->description, "\n")] = 0;

    int day, month, year;
    do {
        printf("Nhap ngay (DD/MM/YYYY): ");
        scanf("%d/%d/%d", &day, &month, &year);
        if (!is_valid_date(day, month, year)) {
            printf("Ngay khong hop le. Vui long nhap lai.\n");
        }
    } while (!is_valid_date(day, month, year));
    snprintf(newExpense->date, sizeof(newExpense->date), "%02d/%02d/%04d", day, month, year);

    newExpense->next = head;
    head = newExpense;
    printf("Da them chi tieu thanh cong!\n");
}

void view_expenses() {
    Expense* current = head;
    printf("Danh sach chi tieu:\n");
    while(current != NULL) {
        printf("# %s - %.2f VND - Ngay: %s\n", current->description, current->amount, current->date);
        current = current->next;
    }
}

float total_expenses() {
    float total = 0;
    Expense* current = head;
    while(current != NULL) {
        total += current->amount;
        current = current->next;
    }
    return total;
}

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

    if (current == NULL) {
        printf("Khong tim thay chi tieu tai so thu tu %d!\n", index);
        return;
    }

    if (prev == NULL) {
        head = current->next;
    } else {
        prev->next = current->next;
    }

    free(current);
    printf("Da xoa chi tieu thanh cong!\n");
}

void edit_expense() {
    int index;
    printf("Nhap so thu tu chi tieu can sua: ");
    scanf("%d", &index);

    if (index <= 0) {
        printf("So thu tu khong hop le!\n");
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
    current->description[strcspn(current->description, "\n")] = 0;

    int day, month, year;
    do {
        printf("Nhap ngay moi (DD/MM/YYYY): ");
        scanf("%d/%d/%d", &day, &month, &year);
        if (!is_valid_date(day, month, year)) {
            printf("Ngay khong hop le. Vui long nhap lai.\n");
        }
    } while (!is_valid_date(day, month, year));
    snprintf(current->date, sizeof(current->date), "%02d/%02d/%04d", day, month, year);

    printf("Da sua chi tieu thanh cong!\n");
}

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

// Ham in chi tieu theo thang
void print_expenses_by_month(int month, int year) {
    Expense* current = head;
    int expense_found = 0;
    printf("Cac chi tieu trong thang %02d/%04d:\n", month, year);
    
    while (current != NULL) {
        int expense_day, expense_month, expense_year;
        sscanf(current->date, "%d/%d/%d", &expense_day, &expense_month, &expense_year);
        
        if (expense_month == month && expense_year == year) {
            printf("# %s - %.2f VND - Ngay: %s\n", current->description, current->amount, current->date);
            expense_found = 1;
        }
        
        current = current->next;
    }
    
    if (!expense_found) {
        printf("Khong co chi tieu nao trong thang %02d/%04d.\n", month, year);
    }
}

// Ham in chi tieu lon nhat va nho nhat theo thang
void print_min_max_expenses_by_month(int month, int year) {
    Expense* current = head;
    Expense* minExpense = NULL;
    Expense* maxExpense = NULL;

    while (current != NULL) {
        int expense_day, expense_month, expense_year;
        sscanf(current->date, "%d/%d/%d", &expense_day, &expense_month, &expense_year);

        if (expense_month == month && expense_year == year) {
            if (minExpense == NULL || current->amount < minExpense->amount) {
                minExpense = current;
            }
            if (maxExpense == NULL || current->amount > maxExpense->amount) {
                maxExpense = current;
            }
        }
        
        current = current->next;
    }

    printf("Chi tieu trong thang %02d/%04d:\n", month, year);
    if (minExpense) {
        printf("- Chi tieu nho nhat: %s - %.2f VND - Ngay: %s\n", minExpense->description, minExpense->amount, minExpense->date);
    } else {
        printf("- Khong co chi tieu nho nhat trong thang.\n");
    }
    
    if (maxExpense) {
        printf("- Chi tieu lon nhat: %s - %.2f VND - Ngay: %s\n", maxExpense->description, maxExpense->amount, maxExpense->date);
    } else {
        printf("- Khong co chi tieu lon nhat trong thang.\n");
    }
}

void menu() {
    int choice;
    do {
        printf("\n--- QUAN LY CHI TIEU ---\n");
        printf("1. Them chi tieu\n");
        printf("2. Xem danh sach chi tieu\n");
        printf("3. Tinh tong chi tieu\n");
        printf("4. Xoa chi tieu\n");
        printf("5. Sua chi tieu\n");
        printf("6. Luu chi tieu vao file\n");
        printf("7. Xem chi tieu theo thang\n");
        printf("8. Xem chi tieu lon nhat va nho nhat theo thang\n");
        printf("9. Thoat\n");
        printf("Chon chuc nang: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: add_expense(); break;
            case 2: view_expenses(); break;
            case 3: printf("Tong chi tieu: %.2f VND\n", total_expenses()); break;
            case 4: delete_expense(); break;
            case 5: edit_expense(); break;
            case 6: save_expenses(); break;
            case 7: {
                int month, year;
                printf("Nhap thang va nam (MM/YYYY): ");
                scanf("%d/%d", &month, &year);
                print_expenses_by_month(month, year);
                break;
            }
            case 8: {
                int month, year;
                printf("Nhap thang va nam (MM/YYYY): ");
                scanf("%d/%d", &month, &year);
                print_min_max_expenses_by_month(month, year);
                break;
            }
            case 9: printf("Thoat chuong trinh.\n"); break;
            default: printf("Chuc nang khong hop le. Vui long chon lai!\n");
        }
    } while (choice != 9);
}

int main() {
    load_expenses();
    menu();
    return 0;
}
