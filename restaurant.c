#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define STAFF_COUNT 5
#define SHIFTS 3
#define MAX_MENU_ITEMS 20
#define TABLE_COUNT 10
#define SALES_FILE "sales_report.txt"

// Global Variables
char menuitem[MAX_MENU_ITEMS][50] = {"Pizza", "Burger", "Pasta", "Sandwich", "French Fry", "Donuts", "Chicken Nuggets", "Hotdog", "Tacos", "Steak", "Spaghetti", "Meatball", "Chicken Fried", "Salad"};
float menuprice[MAX_MENU_ITEMS] = {250.0, 140.0, 150.0, 130.0, 120.0, 140.0, 150.0, 130.0, 120.0, 170.0, 150.0, 140.0, 160.0, 120.0};
int menuCount = 14;
int quantities[MAX_MENU_ITEMS] = {0};
const char adminPassword[] = "admin123";
int tables[TABLE_COUNT] = {0};
char username[50];

// Structures
typedef struct {
    char item_name[50];
    int quantity;
    float price;
} Order;

// Function Prototypes
int readLine(char *buf, int size);
int readInt(int *out);
int readFloat(float *out);
void displayMenu();
void placeOrder();
void generateBILL();
void adminPanel();
void manageMenu();
void manageInventory();
void collectFeedback();
void generate_bill(Order orders[], int count);
void calculate_total_bill(Order orders[], int count);
void log_sales(Order orders[], int count);
void display_daily_sales();
void inputSchedule(char staff[][20], char shifts[][20], int schedule[][SHIFTS]);
void displaySchedule(char staff[][20], char shifts[][20], int schedule[][SHIFTS]);
int login();
void allocateTables();
void makeReservations();

// ---------- Safe input helpers ----------
int readLine(char *buf, int size) {
    if (fgets(buf, size, stdin) == NULL) {
        buf[0] = '\0';
        return 0;                       // EOF
    }
    buf[strcspn(buf, "\n")] = '\0';     // remove newline
    return 1;
}

int readInt(int *out) {
    char buf[64], *end;
    if (!readLine(buf, sizeof(buf))) return 0;
    long v = strtol(buf, &end, 10);
    if (end == buf || *end != '\0') return 0;   // not a number
    *out = (int)v;
    return 1;
}

int readFloat(float *out) {
    char buf[64], *end;
    if (!readLine(buf, sizeof(buf))) return 0;
    float v = strtof(buf, &end);
    if (end == buf || *end != '\0') return 0;
    *out = v;
    return 1;
}

// ---------- Function Definitions ----------
void displayMenu() {
    printf("\nMENU:\n");
    for (int i = 0; i < menuCount; i++) {
        printf("%d. %s - $%.2f\n", i + 1, menuitem[i], menuprice[i]);
    }
}

void placeOrder() {
    int item, quantity;
    char again[10];

    do {
        displayMenu();
        printf("Enter item number (1-%d): ", menuCount);
        if (!readInt(&item) || item < 1 || item > menuCount) {
            printf("Invalid item number!\n");
        } else {
            printf("Enter quantity: ");
            if (!readInt(&quantity) || quantity <= 0) {
                printf("Invalid quantity!\n");
            } else {
                quantities[item - 1] += quantity;
                printf("Item added to order.\n");
            }
        }
        printf("Do you want to order another item? (y/n): ");
        readLine(again, sizeof(again));
    } while (again[0] == 'y' || again[0] == 'Y');
}

void generateBILL() {
    Order orders[MAX_MENU_ITEMS];
    int count = 0;

    for (int k = 0; k < menuCount; k++) {
        if (quantities[k] > 0) {
            strcpy(orders[count].item_name, menuitem[k]);
            orders[count].quantity = quantities[k];
            orders[count].price = menuprice[k];
            count++;
        }
    }

    if (count == 0) {
        printf("\nNo items ordered yet!\n");
        return;
    }

    generate_bill(orders, count);
    log_sales(orders, count);                       // save to file for daily sales
    memset(quantities, 0, sizeof(quantities));      // new order for next customer
}

void adminPanel() {
    char inputPassword[50];
    printf("\nAdmin Panel - Enter Password: ");
    readLine(inputPassword, sizeof(inputPassword));

    if (strcmp(inputPassword, adminPassword) == 0) {
        int adminChoice;
        do {
            printf("\nAdmin Menu:\n1. Manage Menu\n2. Manage Inventory\n3. Exit Admin Panel\nEnter your choice: ");
            if (!readInt(&adminChoice)) adminChoice = 0;
            switch (adminChoice) {
                case 1: manageMenu(); break;
                case 2: manageInventory(); break;
                case 3: printf("Exiting Admin Panel.\n"); break;
                default: printf("Invalid choice!\n");
            }
        } while (adminChoice != 3);
    } else {
        printf("Access denied! Incorrect password.\n");
    }
}

void manageMenu() {
    int menuChoice;
    printf("\nManage Menu:\n1. Add New Item\n2. Remove Item\nEnter your choice: ");
    if (!readInt(&menuChoice)) menuChoice = 0;

    if (menuChoice == 1) {
        if (menuCount < MAX_MENU_ITEMS) {
            char name[50];
            float price;

            printf("Enter new item name: ");
            readLine(name, sizeof(name));
            if (name[0] == '\0' || strchr(name, ',') != NULL) {
                printf("Invalid name!\n");
                return;
            }
            printf("Enter price: ");
            if (!readFloat(&price) || price <= 0) {
                printf("Invalid price!\n");
                return;
            }
            strcpy(menuitem[menuCount], name);
            menuprice[menuCount] = price;
            quantities[menuCount] = 0;
            menuCount++;
            printf("New item added successfully!\n");
        } else {
            printf("Menu is full!\n");
        }
    } else if (menuChoice == 2) {
        int itemNumber;
        displayMenu();
        printf("Enter item number to remove: ");
        if (!readInt(&itemNumber) || itemNumber < 1 || itemNumber > menuCount) {
            printf("Invalid item number!\n");
        } else {
            for (int k = itemNumber - 1; k < menuCount - 1; k++) {
                strcpy(menuitem[k], menuitem[k + 1]);
                menuprice[k] = menuprice[k + 1];
                quantities[k] = quantities[k + 1];   // keep orders in sync
            }
            quantities[menuCount - 1] = 0;
            menuCount--;
            printf("Item removed successfully!\n");
        }
    } else {
        printf("Invalid choice!\n");
    }
}

void manageInventory() {
    printf("\nInventory management feature to be implemented.\n");
}

void collectFeedback() {
    char feedback[100];
    printf("\nEnter your feedback: ");
    readLine(feedback, sizeof(feedback));
    printf("Thank you for your feedback: \"%s\"\n", feedback);
}

void generate_bill(Order orders[], int count) {
    printf("\n======================\n BILL RECEIPT \n======================\n");
    printf("Item\tQuantity\tPrice\tTotal\n");
    calculate_total_bill(orders, count);
}

void calculate_total_bill(Order orders[], int count) {
    float total = 0;
    for (int k = 0; k < count; k++) {
        float item_total = orders[k].quantity * orders[k].price;
        printf("%s\t%d\t%.2f\t%.2f\n", orders[k].item_name, orders[k].quantity, orders[k].price, item_total);
        total += item_total;
    }
    printf("----------------------\nGrand Total: %.2f\n======================\n", total);
}

void log_sales(Order orders[], int count) {
    FILE *file = fopen(SALES_FILE, "a");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }
    for (int k = 0; k < count; k++) {
        // comma separated, so names with spaces (e.g. "French Fry") are safe
        fprintf(file, "%s,%d,%.2f\n", orders[k].item_name, orders[k].quantity, orders[k].price);
    }
    fclose(file);
}

void display_daily_sales() {
    FILE *file = fopen(SALES_FILE, "r");
    if (!file) {
        printf("No sales record found!\n");
        return;
    }
    char item[50];
    int qty;
    float price, total = 0;

    printf("Item\tQuantity\tPrice\tTotal\n");
    while (fscanf(file, " %49[^,],%d,%f", item, &qty, &price) == 3) {
        float item_total = qty * price;
        printf("%s\t%d\t%.2f\t%.2f\n", item, qty, price, item_total);
        total += item_total;
    }
    printf("----------------------\nTotal Sales: %.2f\n", total);
    fclose(file);
}

void inputSchedule(char staff[][20], char shifts[][20], int schedule[][SHIFTS]) {
    for (int i = 0; i < STAFF_COUNT; i++) {
        printf("Assign shifts for %s:\n", staff[i]);
        for (int j = 0; j < SHIFTS; j++) {
            int v;
            do {
                printf("%s shift (1 for assigned, 0 for not assigned): ", shifts[j]);
                if (!readInt(&v)) v = -1;
            } while (v != 0 && v != 1);
            schedule[i][j] = v;
        }
    }
}

void displaySchedule(char staff[][20], char shifts[][20], int schedule[][SHIFTS]) {
    printf("\nStaff Schedule:\n");
    for (int i = 0; i < STAFF_COUNT; i++) {
        printf("%s: ", staff[i]);
        int hasShift = 0;
        for (int j = 0; j < SHIFTS; j++) {
            if (schedule[i][j]) {
                printf("%s ", shifts[j]);
                hasShift = 1;
            }
        }
        if (!hasShift) {
            printf("No shifts assigned");
        }
        printf("\n");
    }
}

int login() {
    char inputUsername[50], inputPassword[50];
    printf("\n===== Login =====\n");

    do {
        printf("Create your username: ");
        if (!readLine(inputUsername, sizeof(inputUsername))) return 0;
    } while (inputUsername[0] == '\0');

    do {
        printf("Create your password: ");
        if (!readLine(inputPassword, sizeof(inputPassword))) return 0;
    } while (inputPassword[0] == '\0');

    strcpy(username, inputUsername);
    printf("\nLogin Successful! Welcome, %s.\n", username);
    return 1;
}

void allocateTables() {
    for (int i = 0; i < TABLE_COUNT; i++) {
        if (tables[i] == 0) {
            tables[i] = 1;
            printf("Success! Table %d has been allocated.\n", i + 1);
            return;
        }
    }
    printf("Sorry! All tables are currently occupied.\n");
}

void makeReservations() {
    char name[50];
    int time;

    printf("\n===== Make a Reservation =====\n");
    printf("Enter your name: ");
    readLine(name, sizeof(name));
    printf("Enter your preferred time (HHMM, 24-hour format): ");

    if (!readInt(&time) || time < 0 || time > 2359 || time % 100 > 59) {
        printf("Invalid time entered.\nUse 24-hour format.\n");
        return;
    }
    printf("Thank you, %s! Your reservation at %04d is confirmed.\n", name, time);
}

int main() {
    if (!login()) return 0;

    int choice;
    char staff[STAFF_COUNT][20] = {"Asad", "Sabbir", "Istiak", "Pronay", "Abir"};
    char shifts[SHIFTS][20] = {"Morning", "Afternoon", "Night"};
    int schedule[STAFF_COUNT][SHIFTS] = {0};

    while (1) {
        printf("\n==== Restaurant Management System ===="
               "\n1. Display Menu and Place Order"
               "\n2. Generate Bill"
               "\n3. Admin Panel"
               "\n4. Feedback"
               "\n5. Allocate Table"
               "\n6. Make Reservation"
               "\n7. Staff Scheduling"
               "\n8. View Daily Sales"
               "\n9. Exit"
               "\nEnter your choice: ");

        if (!readInt(&choice)) {
            if (feof(stdin)) return 0;      // stop on end of input
            printf("Invalid choice!\n");
            continue;
        }

        switch (choice) {
            case 1: placeOrder(); break;
            case 2: generateBILL(); break;
            case 3: adminPanel(); break;
            case 4: collectFeedback(); break;
            case 5: allocateTables(); break;
            case 6: makeReservations(); break;
            case 7: inputSchedule(staff, shifts, schedule); displaySchedule(staff, shifts, schedule); break;
            case 8: display_daily_sales(); break;
            case 9: printf("Thank you, %s! Have a great day!\n", username); return 0;
            default: printf("Invalid choice!\n");
        }
    }
}