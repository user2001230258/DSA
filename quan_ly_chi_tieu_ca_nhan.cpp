#include <iostream>
#include <string>

using namespace std;

// Dinh nghia cau truc cho 1 khoan moi
struct ChiTieu {
    int day;
    int month;
    int year;
    string loaiChiTieu;
    string moTa;
    float soTien;
    ChiTieu *next; // con tro den khoan chi tieu tiep theo
};

// Ham nhap thong tin cho 1 khoan chi
ChiTieu* chiTieuDauVao() {
    ChiTieu *e = new ChiTieu;
    cout << "Nhap ngay/thang/nam (dd/mm/yyyy): ";
    scanf("%d/%d/%d", &e->day, &e->month, &e->year);
    cin.ignore(); // Loại bỏ ký tự newline còn sót lại trong buffer
    cout << "Nhap loai chi tieu: ";
    getline(cin, e->loaiChiTieu);
    cout << "Nhap mo ta: ";
    getline(cin, e->moTa);
    cout << "Nhap so tien: ";
    cin >> e->soTien;
    e->next = NULL; // khoi tao con tri next la NULL
    return e;
}

// Ham hien thi thong tin khoan chi
void inChiTieu(ChiTieu* e) {
    printf("%02d/%02d/%04d | %-20s | %-30s | %.2f\n", 
           e->day, e->month, e->year, e->loaiChiTieu.c_str(), 
           e->moTa.c_str(), e->soTien);
}

// Ham chen 1 khoan chi moi vao dau danh sach
ChiTieu* themChiTieu(ChiTieu* head, ChiTieu* chiTieuMoi) {
    if (head == NULL) return chiTieuMoi;
    else {
        ChiTieu* current = head;
        while(current->next != NULL) {
            current = current->next;
        }
        // Them khoan chi moi vao sau phan tu cuoi
        current->next = chiTieuMoi;
        return head;
    }
}

// Ham hien thi DS cac khoan chi
void danhSachChiTieu(ChiTieu* head) {
    ChiTieu *current = head;
    while (current != NULL) {
        inChiTieu(current);
        current = current->next;
    }
}

// Ham giai phong bo nho chi linked list
void giaiPhongDanhSach(ChiTieu* head) {
    ChiTieu *current = head;
    while(current != NULL) {
        ChiTieu* next = current->next;
        delete current;
        current = next;
    }
}

int main() {
    ChiTieu* head = NULL; // khoi tao ds rong
    int choice;

    do {
        cout << "\n======= QUAN LY CHI TIEU CA NHAN =======\n";
        cout << "1. Them moi khoan chi\n";
        cout << "2. Hien thi danh sach chi tieu\n";
        cout << "3. Thoat chuong trinh\n";
        cout << "Chon chuc nang: ";
        cin >> choice;
        cin.ignore(); // Loại bỏ ký tự newline còn sót lại trong buffer

        switch (choice) {
            case 1:
                head = themChiTieu(head, chiTieuDauVao());
                cout << "Them khoan chi thanh cong!\n";
                break;

            case 2: 
                if (head == NULL) {
                    cout << "Chua co khoan chi nao!\n";
                } else {
                    cout << "\n===== DANH SACH CHI TIEU =====\n";
                    danhSachChiTieu(head);
                }
                break;

            case 3:
                giaiPhongDanhSach(head); // Giai phong truoc khi thoat
                cout << "Thoat chuong trinh!\n";
                break;

            default:
                cout << "Lua chon khong hop le!\n";
        }
    }while(choice!=3);

    return 0;
}
