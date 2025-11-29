
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "expenses.txt"
#define EXPORT_FILE "export.csv"
#define MAX_LINE 256
#define MAX_RECORDS 1000
#define BUDGET_LIMIT 1000.0

typedef struct {
    char date[11];     
    char category[50];
    float amount;
} Expense;

Expense records[MAX_RECORDS];
int record_count = 0;

void loadExpenses();
void saveExpenses();
void addExpense();
void viewByDate();
void viewByCategory();
void checkMonthlyBudget();
void monthlySummary();
void editEntry();
void deleteEntry();
void exportToCSV();

int main() {
    int choice;
    while (1) {
        printf("\n--- Expense Tracker ---\n");
        printf("1. Add Expense\n");
        printf("2. View by Date\n");
        printf("3. View by Category\n");
        printf("4. Check Monthly Budget\n");
        printf("5. Monthly Summary\n");
        printf("6. Edit Entry\n");
        printf("7. Delete Entry\n");
        printf("8. Export to CSV\n");
        printf("9. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: addExpense(); break;
            case 2: viewByDate(); break;
            case 3: viewByCategory(); break;
            case 4: checkMonthlyBudget(); break;
            case 5: monthlySummary(); break;
            case 6: editEntry(); break;
            case 7: deleteEntry(); break;
            case 8: exportToCSV(); break;
            case 9: exit(0);
            default: printf("Invalid choice!\n");
        }
    }
    return 0;
}

void loadExpenses() {
    FILE *file = fopen(FILE_NAME, "r");
    if (!file) return;

    record_count = 0;
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), file) && record_count < MAX_RECORDS) {
        sscanf(line, "%10[^,],%49[^,],%f", records[record_count].date, records[record_count].category, &records[record_count].amount);
        record_count++;
    }
    fclose(file);
}

void saveExpenses() {
    FILE *file = fopen(FILE_NAME, "w");
    if (!file) return;

    for (int i = 0; i < record_count; i++) {
        fprintf(file, "%s,%s,%.2f\n", records[i].date, records[i].category, records[i].amount);
    }
    fclose(file);
}

void addExpense() {
    loadExpenses();
    if (record_count >= MAX_RECORDS) {
        printf("Storage full.\n");
        return;
    }

    printf("Enter date (YYYY-MM-DD): ");
    fgets(records[record_count].date, 11, stdin);
    getchar();
    printf("Enter category: ");
    fgets(records[record_count].category, 50, stdin);
    records[record_count].category[strcspn(records[record_count].category, "\n")] = 0;
    printf("Enter amount: ");
    scanf("%f", &records[record_count].amount);
    getchar();

    record_count++;
    saveExpenses();
    printf("Expense added.\n");
}

void viewByDate() {
    char date[11];
    loadExpenses();
    printf("Enter date (YYYY-MM-DD): ");
    fgets(date, 11, stdin);
    getchar();

    float total = 0;
    for (int i = 0; i < record_count; i++) {
        if (strcmp(records[i].date, date) == 0) {
            printf("  %s: $%.2f\n", records[i].category, records[i].amount);
            total += records[i].amount;
        }
    }
    printf("Total: $%.2f\n", total);
}

void viewByCategory() {
    char category[50];
    loadExpenses();
    printf("Enter category: ");
    fgets(category, 50, stdin);
    category[strcspn(category, "\n")] = 0;

    float total = 0;
    for (int i = 0; i < record_count; i++) {
        if (strcmp(records[i].category, category) == 0) {
            printf("  %s: $%.2f\n", records[i].date, records[i].amount);
            total += records[i].amount;
        }
    }
    printf("Total: $%.2f\n", total);
}

void checkMonthlyBudget() {
    char month[8];
    loadExpenses();
    printf("Enter month (YYYY-MM): ");
    fgets(month, 8, stdin);
    getchar();

    float total = 0;
    for (int i = 0; i < record_count; i++) {
        if (strncmp(records[i].date, month, 7) == 0) {
            total += records[i].amount;
        }
    }
    printf("Total for %s: $%.2f\n", month, total);
    if (total > BUDGET_LIMIT)
        printf("⚠️  Budget exceeded!\n");
    else
        printf("✅ Within budget.\n");
}

void monthlySummary() {
    char month[8];
    loadExpenses();
    printf("Enter month (YYYY-MM): ");
    fgets(month, 8, stdin);
    getchar();

    float totals[MAX_RECORDS] = {0};
    char cats[MAX_RECORDS][50];
    int catCount = 0;

    for (int i = 0; i < record_count; i++) {
        if (strncmp(records[i].date, month, 7) == 0) {
            int found = 0;
            for (int j = 0; j < catCount; j++) {
                if (strcmp(cats[j], records[i].category) == 0) {
                    totals[j] += records[i].amount;
                    found = 1;
                    break;
                }
            }
            if (!found) {
                strcpy(cats[catCount], records[i].category);
                totals[catCount] = records[i].amount;
                catCount++;
            }
        }
    }
    printf("\nCategory-wise summary for %s:\n", month);
    for (int i = 0; i < catCount; i++) {
        printf("  %s: $%.2f\n", cats[i], totals[i]);
    }
}

void editEntry() {
    char date[11], category[50];
    float amount;
    loadExpenses();
    printf("Enter date of entry to edit: ");
    fgets(date, 11, stdin); getchar();
    printf("Enter category: ");
    fgets(category, 50, stdin);
    category[strcspn(category, "\n")] = 0;
    printf("Enter amount: ");
    scanf("%f", &amount); getchar();

    for (int i = 0; i < record_count; i++) {
        if (strcmp(records[i].date, date) == 0 &&
            strcmp(records[i].category, category) == 0 &&
            records[i].amount == amount) {
            printf("Editing entry: %s, %s, %.2f\n", date, category, amount);
            printf("New date: ");
            fgets(records[i].date, 11, stdin); getchar();
            printf("New category: ");
            fgets(records[i].category, 50, stdin);
            records[i].category[strcspn(records[i].category, "\n")] = 0;
            printf("New amount: ");
            scanf("%f", &records[i].amount); getchar();
            saveExpenses();
            printf("Entry updated.\n");
            return;
        }
    }
    printf("Entry not found.\n");
}

void deleteEntry() {
    char date[11], category[50];
    float amount;
    loadExpenses();
    printf("Enter date of entry to delete: ");
    fgets(date, 11, stdin); getchar();
    printf("Enter category: ");
    fgets(category, 50, stdin);
    category[strcspn(category, "\n")] = 0;
    printf("Enter amount: ");
    scanf("%f", &amount); getchar();

    for (int i = 0; i < record_count; i++) {
        if (strcmp(records[i].date, date) == 0 &&
            strcmp(records[i].category, category) == 0 &&
            records[i].amount == amount) {
            for (int j = i; j < record_count - 1; j++)
                records[j] = records[j + 1];
            record_count--;
            saveExpenses();
            printf("Entry deleted.\n");
            return;
        }
    }
    printf("Entry not found.\n");
}

void exportToCSV() {
    loadExpenses();
    FILE *file = fopen(EXPORT_FILE, "w");
    if (!file) {
        printf("Error exporting to CSV.\n");
        return;
    }
    fprintf(file, "Date,Category,Amount\n");
    for (int i = 0; i < record_count; i++) {
        fprintf(file, "%s,%s,%.2f\n", records[i].date, records[i].category, records[i].amount);
    }
    fclose(file);
    printf("Exported to %s.\n", EXPORT_FILE);

}

