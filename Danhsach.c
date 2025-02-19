#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Nguoidung {
    char tennguoidung[20];
    struct Nguoidung *next;
} Nguoidung;

Nguoidung* Tao_Node(char* ten) {
    Nguoidung *Nguoi_Moi = (Nguoidung*)malloc(sizeof(Nguoidung));
    if (Nguoi_Moi == NULL) {
        printf("Loi he thong!\n");
        exit(1);
    }
    strncpy(Nguoi_Moi->tennguoidung, ten, 20);
    Nguoi_Moi->tennguoidung[19] = '\0';
    Nguoi_Moi->next = NULL;
    return Nguoi_Moi;
}

void Them_Node_Cuoi(Nguoidung **Dau, char* ten) {
    Nguoidung *Nguoi_Moi = Tao_Node(ten);
    if (*Dau == NULL) {
        *Dau = Nguoi_Moi;
        return;
    }
    Nguoidung *di_chuyen = *Dau;
    while (di_chuyen->next != NULL) {
        di_chuyen = di_chuyen->next;
    }
    di_chuyen->next = Nguoi_Moi;
}

void Them_Node_Bat_Ki(Nguoidung **Dau, char* ten, int vitri)
{
    Nguoidung *Nguoi_Moi = Tao_Node(ten);
    Nguoidung *di_chuyen = *Dau;
    int count = 1;

    if(vitri == 1)
    {
        Nguoi_Moi->next = *Dau;
        *Dau = Nguoi_Moi;
    }
    else
    {
        while(di_chuyen != NULL && vitri != 0 && count < vitri - 1)
        {
            di_chuyen = di_chuyen->next;
            count++;
        }
        if (di_chuyen == NULL) 
        {
            if (*Dau == NULL) {
                *Dau = Nguoi_Moi;
            } else {
                Nguoidung *kiem_tra = *Dau;
                while (kiem_tra->next != NULL) {
                    kiem_tra = kiem_tra->next;
                }
                kiem_tra->next = Nguoi_Moi;
            }
        }
        else
        {
            Nguoi_Moi->next = di_chuyen->next;
            di_chuyen->next = Nguoi_Moi;
        }
    }
}

void Ghi_Vao_File(Nguoidung *Dau, const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (f == NULL) {
        printf("Khong mo duoc file %s!\n", filename);
        return;
    }
    while (Dau != NULL) {
        fprintf(f, "%s\n", Dau->tennguoidung);
        Dau = Dau->next;
    }
    fclose(f);
    printf("Da ghi danh sach ten vao file %s!\n", filename);
}

void Giai_Phong_Danh_Sach(Nguoidung **Dau) {
    Nguoidung *temp;
    while (*Dau != NULL) {
        temp = *Dau;
        *Dau = (*Dau)->next;
        free(temp);
    }
}

void Doc_Tu_File(Nguoidung **Dau, const char *filename) {
    Giai_Phong_Danh_Sach(Dau);
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("Khong the mo file %s!\n", filename);
        return;
    }
    char ten[20];
    while (fgets(ten, sizeof(ten), f) != NULL) {
        ten[strcspn(ten, "\n")] = '\0';
        Them_Node_Cuoi(Dau, ten);
    }
    fclose(f);
    printf("Doc file %s thanh cong!\n", filename);
}

void Tim_Kiem(Nguoidung *Dau, char* ten) {
    int found = 0;
    while (Dau != NULL) {
        if (strstr(Dau->tennguoidung, ten) != NULL) {
            printf("- %s\n", Dau->tennguoidung);
            found = 1;
        }
        Dau = Dau->next;
    }
    if (!found) {
        printf("Khong tim thay ten nao chua %s.\n", ten);
    }
}

int main() {
    Nguoidung *Dau = NULL;
    unsigned int lua_chon, vitri;
    char ten[20];
    char filename[] = "tennguoidung.txt";

    while (1) {
        printf("\n----- MENU -----\n");
        printf("1. Nhap ten nguoi dung tu console\n");
        printf("2. Ghi danh sach ten nguoi dung vao file\n");
        printf("3. Doc danh sach ten nguoi dung tu file\n");
        printf("4. Tim kiem ten nguoi dung\n");
        printf("5. Xoa danh sach va thoat chuong trinh\n");
        printf("Chon mot tuy chon (1-5): ");
        scanf("%d", &lua_chon);
        getchar();

        switch (lua_chon) {
            case 1:
                printf("Nhap ten nguoi dung: ");
                fgets(ten, sizeof(ten), stdin);
                ten[strcspn(ten, "\n")] = '\0';
                printf("Nhap vi tri can them: ");
                scanf("%d", &vitri);
                getchar();
                Them_Node_Bat_Ki(&Dau, ten, vitri);
                break;
            case 2:
                Ghi_Vao_File(Dau, filename);
                break;
            case 3:
                Doc_Tu_File(&Dau, filename);
                break;
            case 4:
                printf("Nhap ten can tim: ");
                fgets(ten, sizeof(ten), stdin);
                ten[strcspn(ten, "\n")] = '\0';
                Tim_Kiem(Dau, ten);
                break;
            case 5:
                printf("Thoat chuong trinh!\n");
                Giai_Phong_Danh_Sach(&Dau);
                return 0;
            default:
                printf("Lua chon khong hop le!\n");
        }
    }
}