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
    int ID;
    char TaskName[NAME_LENGTH];
    Date Deadline;
    char Description[DESC_LENGTH];
    int Status; // 0 = Pending, 1 = Complete
    struct Task* next;
} Task;

void AddTask(Task **head) {
    Task* newTask = (Task*)malloc(sizeof(Task));
    if (!newTask) {
        printf("Khong the tao task moi!\n");
        return;
    }
    printf("Nhap ID: ");
    scanf("%d", &newTask->ID);
    getchar();

    printf("Nhap ten task: ");
    fgets(newTask->TaskName, NAME_LENGTH, stdin);
    strtok(newTask->TaskName, "\n");

    printf("Nhap deadline (hh đd mm yy): ");
    scanf("%d %d %d %d", &newTask->Deadline.hour, &newTask->Deadline.day,
          &newTask->Deadline.month, &newTask->Deadline.year);
    getchar();

    printf("Nhap noi dung: ");
    fgets(newTask->Description, DESC_LENGTH, stdin);
    strtok(newTask->Description, "\n");

    printf("Nhap trang thai (0: Chua xong, 1: Hoan thanh): ");
    scanf("%d", &newTask->Status);
    getchar();

    newTask->next = *head;
    *head = newTask;

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

    // Sửa tên
    printf("Tên cũ: %s\nNhập tên mới (Enter để bỏ qua): ", edittask->TaskName);
    char editname[NAME_LENGTH];
    fgets(editname, NAME_LENGTH, stdin);
    if (editname[0] != '\n') {
        strtok(editname, "\n");
        strcpy(edittask->TaskName, editname);
    }

    // Sửa deadline
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

    // Sửa mô tả
    printf("Mo ta cu: %s\nMo ta moi (Enter de bo qua): ", edittask->Description);
    char editdes[DESC_LENGTH];
    fgets(editdes, DESC_LENGTH, stdin);
    if (editdes[0] != '\n') {
        strtok(editdes, "\n");
        strcpy(edittask->Description, editdes);
    }

    // Sửa trạng thái
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

void SaveToFile(Task *head) {
    FILE* fp = fopen(FILENAME, "w");
    if (!fp) {
        printf("Không thể mở file để ghi!\n");
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
    printf("Đã lưu danh sách công việc ra file!\n");
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
        strtok(newTask->TaskName, "\n");

        fscanf(fp, "%d %d %d %d\n",
               &newTask->Deadline.hour, &newTask->Deadline.day,
               &newTask->Deadline.month, &newTask->Deadline.year);

        fgets(newTask->Description, DESC_LENGTH, fp);
        strtok(newTask->Description, "\n");

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
        printf("1. Thêm công việc\n");
        printf("2. Xoá công việc\n");
        printf("3. Sửa công việc\n");
        printf("4. Lưu danh sách & Thoát\n");
        printf("==========================\n");
        printf("Nhập lựa chọn: ");

        int choice;
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                AddTask(&head);
                break;
            case 2:
                DeleteTask(&head);
                break;
            case 3:
                EditTask(head);
                break;
            case 4:
                SaveToFile(head);
                FreeList(&head);
                return 0;
            default:
                printf("Khong hop le!\n");
                break;
        }
    }
    return 0;
}
