#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DESCRIPTION_LENGTH 100

typedef struct Expense{
    float amount;
    char description[DESCRIPTION_LENGTH];
    char date[11]; 
    struct Expense *next; // con tro toi chi tieu tiep theo

   
} Expense;

Expense* head = NULL; // con tro toi dau danh sach chi tieu

// Ham chuyen doi ngay sang kieu so nguyen
int dateToInt(const char* date) {
    int day, month, year;
    sscanf(date, "%d/%d/%d", &day, &month, &year);
    return year * 10000 + month * 100 + day; // chuyen doi thanh YYYYMMDD
}    

// Ham them chi tieu
void add_expense() {
    Expense* newExpense = (Expense*)malloc(sizeof(Expense)); // Cap phat bo nho cho chi tieu moi
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
    newExpense->next = head; // dat chi tieu moi vao dau danh sach
    head = newExpense;       // cap nhat head
    printf("Da them chi tieu thanh cong!");
}

// Ham xem chi tieu
void view_expenses() {
    Expense* current = head; // bat dau tu dau danh sach
    printf("Danh sach chi tieu:\n");
    while(current != NULL) {  // duyet qua tat ca cac chi tieu
        printf("# %s - %.2f VND - Ngay: %s\n", current->description, current->amount, current->date);
        current = current->next; // chuyen toi chi tieu tiep theo
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

    Expense* current = head; // bat dau tu dau danh sach
    Expense* prev = NULL;    // bien luu chi tieu truoc do


// duyet qua danh sach de tim chi tieu can xoa
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

    free(current); // giai phong bo nho
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
            Expense* newExpense = (Expense*)malloc(sizeof(Expense)); // cap phat bo nho cho chi tieu moi
            if (newExpense == NULL) {
                printf("Loi cap phat bo nho!\n");
                break;
            }
            if(fscanf(file, "%f,%[^,],%s\n", &newExpense->amount, newExpense->description, newExpense->date) == 3) {
                newExpense->next = head; // dat chi tieu moi vao dau danh sach
                head = newExpense;       // cap nhat head
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

void swap(Expense* a, Expense* b) {
    float tempAmount = a->amount;
    char tempDesciption[DESCRIPTION_LENGTH];
    char tempDate[11];

    strcpy(tempDesciption, a->description);
    strcpy(tempDate, a->date);

    a->amount = b->amount;
    strcpy(a->description, b->description);
    strcpy(a->date, b->date);

    b->amount = tempAmount;
    strcpy(b->description, tempDesciption);
    strcpy(b->date, tempDate);
}

// Bubble Sort
void bubble_sort_expenses() {
    if (head == NULL) return;

    int swapped;
    Expense* ptr1;
    Expense* lptr = NULL;
    
    do {
        swapped = 0;
        ptr1 = head;

        while (ptr1->next != lptr) {
            if (dateToInt(ptr1->date) > dateToInt(ptr1->next->date)) {
                swap(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
    
    printf("Da sap xep chi tieu thanh cong!\n");
}

void sort_expenses() {
    bubble_sort_expenses();
    view_expenses();
}

// tim kiem chi tieu bang thuat toan linear search
void search_expenses() {
    int choice;
    printf("Tim kiem theo:\n");
    printf("1. Ngay\n");
    printf("2. Tu khoa mo ta\n");
    printf("Chon: ");
    scanf("%d", &choice);

    if (choice == 1) {
        char date[11];
        printf("Nhap ngay can tim (DD/MM/YYYY): ");
        scanf("%s", date);

        Expense* current = head;
        printf("Chi tieu tim thay theo ngay %s:\n", date);
        while (current != NULL) {
            if (strcmp(current->date, date) == 0) {
                printf("# %s - %.2f VND - Ngay: %s\n", current->description, current->amount, current->date);
            }
            current = current->next;
        }
    } else if (choice == 2) {
        char keyword[DESCRIPTION_LENGTH];
        printf("Nhap tu khoa mo ta can tim: ");
        getchar(); // Loại bỏ newline từ buffer
        fgets(keyword, DESCRIPTION_LENGTH, stdin);
        keyword[strcspn(keyword, "\n")] = 0; // Xóa ký tự newline

        Expense* current = head;
        printf("Chi tieu tim thay theo tu khoa \"%s\":\n", keyword);
        while (current != NULL) {
            if (strstr(current->description, keyword) != NULL) {
                printf("# %s - %.2f VND - Ngay: %s\n", current->description, current->amount, current->date);
            }
            current = current->next;
        }
    } else {
        printf("Lua chon khong hop le!\n");
    }
}

// tao bao cao chi tieu cho tung nam
void report_expenses() {
    int year;
    printf("Nhap nam: ");
    scanf("%d", &year);

    float total = 0;
    Expense* current = head;

    // tao ten file dua tren tung nam
    char filename[30];
    snprintf(filename, sizeof(filename), "expense_report_%d.txt", year); // Ham snprintf() giup de tao ten file dua tren nam duoc nhap

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Khong the mo file de luu bao cao!\n");
        return;
    }

    fprintf(file, "Bao cao chi tieu cho nam %d:\n", year);
    printf("Bao cao chi tieu cho nam %d:\n", year);

    while (current != NULL) {
        int expYear;
        sscanf(current->date, "%*d/%*d/%d", &expYear);

        if (expYear == year) {
            fprintf(file, "# %s - %.2f VND - Ngay: %s\n", current->description, current->amount, current->date);
            printf("# %s - %.2f VND - Ngay: %s\n", current->description, current->amount, current->date);
            total += current->amount;
        }
        current = current->next;
    }

    fprintf(file, "Tong chi tieu cho nam %d: %.2f VND\n", year, total);
    printf("Tong chi tieu cho nam %d: %.2f VND\n", year, total);

    fclose(file);
    printf("Da luu bao cao chi tieu vao file '%s' thanh cong!\n", filename);
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
        printf("7. Sap xep chi tieu\n");
        printf("8. Tim kiem chi tieu\n");
        printf("9. Tao bao cao chi tieu\n");
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
            case 7: sort_expenses(); break;
            case 8: search_expenses(); break;
            case 9: report_expenses(); break;
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
