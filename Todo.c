#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "Todolist.txt"

#define NAME_LENGTH 32
#define DESC_LENGTH 68

#define COL_NAME    24   
#define COL_DEAD    18   
#define COL_DESC    40   
#define COL_STATUS  10   

#define WRAP_DESC   COL_DESC

typedef struct Date {
    int hour;
    int day;
    int month;
    int year;
} Date;

typedef struct Task {
    char TaskName[NAME_LENGTH];
    char Description[DESC_LENGTH];
    Date Deadline;
    int ID;
    int Status; 
    struct Task* next;
} Task;

void AddTask(Task **head) {
    Task* newTask = (Task*)malloc(sizeof(Task));
    if (!newTask) {
        printf("Khong the tao task moi!\n");
        return;
    }

    printf("Nhap ten task: ");
    fgets(newTask->TaskName, NAME_LENGTH, stdin);
    newTask->TaskName[strcspn(newTask->TaskName, "\n")] = '\0';

    printf("Nhap deadline (hh dd mm yy): ");
    scanf("%d %d %d %d", &newTask->Deadline.hour, &newTask->Deadline.day,
          &newTask->Deadline.month, &newTask->Deadline.year);
    getchar();

    printf("Nhap noi dung: ");
    fgets(newTask->Description, DESC_LENGTH, stdin);
    newTask->Description[strcspn(newTask->Description, "\n")] = '\0';

    printf("Nhap trang thai (0: Chua xong, 1: Hoan thanh): ");
    scanf("%d", &newTask->Status);
    getchar();

    newTask->next = NULL; 

    if (*head == NULL) 
    {
        *head = newTask;
        newTask->ID = 1;
    } 
    else 
    {
        Task *temp = *head;
        while (temp->next != NULL) 
        {
            temp = temp->next;
        }
        temp->next = newTask;
        newTask->ID = temp->ID + 1;
    }

    printf("Them task thanh cong!\n");
}

void DeleteTask(Task **head) {
    if (!(*head)) {
        printf("Khong co task nao!\n");
        return;
    }
    int id;
    printf("Nhap ID: ");
    scanf("%d", &id);
    getchar();

    Task *deltask = *head, *prev = NULL;
    while (deltask && deltask->ID != id) {
        prev = deltask;
        deltask = deltask->next;
    }
    if (!deltask) {
        printf("Khong tim thay task ID = %d\n", id);
        return;
    }
    if (!prev) {
        *head = deltask->next;
    } else {
        prev->next = deltask->next;
    }
    free(deltask);
    printf("Da xoa task ID = %d\n", id);
}

void EditTask(Task *head) {
    if (!head) {
        printf("Khong co task nao!\n");
        return;
    }
    int id;
    printf("Nhap ID task: ");
    scanf("%d", &id);
    getchar();

    Task* edittask = head;
    while (edittask && edittask->ID != id) {
        edittask = edittask->next;
    }
    if (!edittask) {
        printf("Khong tim thay ID = %d\n", id);
        return;
    }

    printf("Tên cu: %s\nNhập tên mới (Enter de bo qua): ", edittask->TaskName);
    char editname[NAME_LENGTH];
    fgets(editname, NAME_LENGTH, stdin);
    if (editname[0] != '\n') {
        editname[strcspn(editname, "\n")] = '\0';
        strcpy(edittask->TaskName, editname);
    }

    printf("Deadline cu: %02d/%02d/%04d %02d:00\n",
           edittask->Deadline.day, edittask->Deadline.month,
           edittask->Deadline.year, edittask->Deadline.hour);
    printf("Nhap Deadline moi (hh dd mm yy, nhap 0 0 0 0 de bo qua): ");
    int hh, dd, mm, yyyy;
    scanf("%d %d %d %d", &hh, &dd, &mm, &yyyy);
    getchar();
    if (!(hh == 0 && dd == 0 && mm == 0 && yyyy == 0)) {
        edittask->Deadline.hour = hh;
        edittask->Deadline.day = dd;
        edittask->Deadline.month = mm;
        edittask->Deadline.year = yyyy;
    }

    printf("Mo ta cu: %s\nMo ta moi (Enter de bo qua): ", edittask->Description);
    char editdes[DESC_LENGTH];
    fgets(editdes, DESC_LENGTH, stdin);
    if (editdes[0] != '\n') {
        editdes[strcspn(editdes, "\n")] = '\0';
        strcpy(edittask->Description, editdes);
    }

    printf("Trang thai cu: %s\n", edittask->Status ? "Hoan thanh" : "Chua xong");
    printf("Trang thai moi (0: Chua xong, 1: Hoan thanh, -1: Bo qua): ");
    int editstatus;
    scanf("%d", &editstatus);
    getchar();
    if (editstatus == 0 || editstatus == 1) {
        edittask->Status = editstatus;
    }

    printf("Da cap nhat task!\n");
}

void PrintHead() {
    printf("+----+------------------------+------------------+----------------------------------------+----------+\n");   
    printf("|%-4s|%-24s|%-18s|%-40s|%-10s|\n",
           "ID", "Task Name", "Deadline", "Description", "Status");
    printf("+----+------------------------+------------------+----------------------------------------+----------+\n");
}

void PrintTaskRow(const Task* t) {
    char deadlineStr[50];
    snprintf(deadlineStr, sizeof(deadlineStr),
             "%02d/%02d/%04d %02d:00",
             t->Deadline.day, t->Deadline.month,
             t->Deadline.year, t->Deadline.hour);

    const char* statusStr = t->Status ? "Hoanthanh" : "Chua xong";

    int lenDes = strlen(t->Description);
    if (lenDes == 0) {
        printf("|%-4d|%-24.24s|%-18.18s|%-40.40s|%-10.10s|\n",
               t->ID, t->TaskName, deadlineStr, "", statusStr);
        printf("+----+------------------------+------------------+----------------------------------------+----------+\n");   
        return;
    }

    int wrap = WRAP_DESC;
    int lines = (lenDes + wrap - 1) / wrap;

    for (int i = 0; i < lines; i++) {
        char Des[WRAP_DESC + 1];
        int start = i * wrap;
        if (start < lenDes) {
            strncpy(Des, t->Description + start, wrap);
            Des[wrap] = '\0';
        } else {
            Des[0] = '\0';
        }

        if (i == 0) {
            printf("|%-4d|%-24.24s|%-18.18s|%-40.40s|%-10.10s|\n",
                   t->ID, t->TaskName, deadlineStr, Des, statusStr);
        } else {
            printf("|%-4s|%-24.24s|%-18.18s|%-40.40s|%-10.10s|\n",
                   "", "", "", Des, "");
        }
    }
    printf("+----+------------------------+------------------+----------------------------------------+----------+\n");
}

void Display(Task *head) {
    PrintHead();
    if (!head) {
        printf("|%-4s|%-24.24s|%-18.18s|%-40.40s|%-10.10s|\n", "", "", "", "Khong co du lieu", "");
        printf("+----+------------------------+------------------+----------------------------------------+----------+\n");
        return;
    }
    Task* task = head;
    while (task) {
        PrintTaskRow(task);
        task = task->next;
    }
}

void Search(Task *head) {
    if (!head) {
        printf("Khong co task!\n");
        return;
    }
    int searchID;
    printf("Nhap ID: ");
    scanf("%d", &searchID);
    getchar();

    int found = 0;
    Task *current = head;
    while (current) {
        if (current->ID == searchID) {
            printf("+----+------------------------+------------------+----------------------------------------+----------+\n");
            PrintTaskRow(current);
            found = 1;
            break;
        }
        current = current->next;
    }
    if (!found) {
        printf("Khong tim thay task co ID: %d\n", searchID);
    }
}

void SaveToFile(Task *head) {
    FILE* fp = fopen(FILENAME, "w");
    if (!fp) {
        printf("Khong the mo file de ghi!\n");
        return;
    }
    Task* task = head;
    while (task) {
        fprintf(fp, "%d\n%s\n%d %d %d %d\n%s\n%d\n",
                task->ID,
                task->TaskName,
                task->Deadline.hour, task->Deadline.day,
                task->Deadline.month, task->Deadline.year,
                task->Description,
                task->Status);
        task = task->next;
    }
    fclose(fp);
    printf("Da luu danh sach cong viec ra file!\n");
}

void ReadFromFile(Task **head) {
    FILE* fp = fopen(FILENAME, "r");
    if (!fp) return;

    while (!feof(fp)) {
        Task* newTask = (Task*)malloc(sizeof(Task));
        if (!newTask) break;

        if (fscanf(fp, "%d\n", &newTask->ID) != 1) {
            free(newTask);
            break;
        }
        fgets(newTask->TaskName, NAME_LENGTH, fp);
        newTask->TaskName[strcspn(newTask->TaskName, "\n")] = '\0';

        fscanf(fp, "%d %d %d %d\n",
               &newTask->Deadline.hour, &newTask->Deadline.day,
               &newTask->Deadline.month, &newTask->Deadline.year);

        fgets(newTask->Description, DESC_LENGTH, fp);
        newTask->Description[strcspn(newTask->Description, "\n")] = '\0';

        fscanf(fp, "%d\n", &newTask->Status);

        newTask->next = *head;
        *head = newTask;
    }
    fclose(fp);
}

void FreeList(Task **head) {
    Task* temp;
    while (*head) {
        temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}

int main() {
    Task* head = NULL;
    ReadFromFile(&head);

    while (1) {
        Display(head);

        printf("\n========== MENU ==========\n");
        printf("1. Them task\n");
        printf("2. Xoa task\n");
        printf("3. Sua task\n");
        printf("4. Luu danh sach\n");
        printf("5. Doc danh sach tu file\n");
        printf("6. Tim kiem task theo ID\n");
        printf("7. Thoat\n");
        printf("==========================\n");
        printf("Nhap lua chon: ");

        int choice;
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                AddTask(&head);
                printf("Da them task!\n");
                break;
            case 2:
                DeleteTask(&head);
                printf("Da xoa task!\n");
                break;
            case 3:
                EditTask(head);
                break;
            case 4:
                SaveToFile(head);
                printf("Da luu danh sach!\n");
                break;
            case 5:
                FreeList(&head);
                ReadFromFile(&head);
                printf("Da doc danh sach tu file!\n");
                break;
            case 6: 
                Search(head);
                break;
            case 7:
                printf("Danh sach sau khi thoat chuong trinh se khong duoc luu!\nBan co muon luu danh sach khong? (1: Co, 0: Khong):");
                int save;
                scanf("%d", &save);
                getchar();
                switch(save)
                {
                    case 1:
                        SaveToFile(head);
                        FreeList(&head);
                        printf("Da luu danh sach va thoat chuong trinh!\n");
                        return 0;
                    case 0:
                        FreeList(&head);
                        printf("Da thoat chuong trinh!\n");
                        return 0;
                    default:
                        printf("Khong hop le!\n");
                        break;
                }
                break;
            default:
                printf("Khong hop le!\n");
                break;
        }
    }
    return 0;
}
