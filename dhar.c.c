#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>


// Define console color constants
#define DEFAULT_COLOR (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED)
#define ERROR_COLOR (FOREGROUND_RED | BACKGROUND_BLUE)
#define SUCCESS_COLOR (FOREGROUND_GREEN | BACKGROUND_BLUE)


// Define a structure to represent a borrow record
typedef struct {
    char contact[100];
    char phoneNumber[15];
    char email[100];
    float amount;
    char type; // 'B' for borrow, 'L' for lend
} BorrowRecord;

// Define a structure to represent a user account
typedef struct {
    char username[50];
    char password[50];
    char email[100]; // New field for email address
} UserAccount;

// Function to set console text color based on message type
void setConsoleColor(char messageType) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    switch (messageType) {
        case 'E': // Error (red text, white background)
            SetConsoleTextAttribute(hConsole, ERROR_COLOR);
            break;
        case 'S': // Success (green text, white background)
            SetConsoleTextAttribute(hConsole, SUCCESS_COLOR);
            break;
        default:
            SetConsoleTextAttribute(hConsole, DEFAULT_COLOR);
    }
}

// Function to reset console text color to default
void resetConsoleColor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, DEFAULT_COLOR);
}


// Function to send an SMS notification
void sendSMSNotification(const char *phoneNumber, const char *message) {


    setConsoleColor('S'); // Set color to green for success
    printf("SMS Notification sent to %s: %s\n", phoneNumber, message);
    resetConsoleColor(); // Reset color to default
}

// Function to display error message
void displayError(const char *errorMessage) {
    setConsoleColor('E'); // Set color to red for error
    fprintf(stderr, "%s\n", errorMessage);
    resetConsoleColor(); // Reset color to default
}

// Function to display success message
void displaySuccess(const char *successMessage) {
    setConsoleColor('S'); // Set color to green for success
    printf("%s\n", successMessage);
    resetConsoleColor(); // Reset color to default
}

// Function to save user accounts to file
void saveUserAccounts(UserAccount *users, int userCount) {
    FILE *file = fopen("user_accounts.txt", "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening user accounts file for writing.\n");
        return;
    }

    for (int i = 0; i < userCount; i++) {
        fprintf(file, "%s %s %s\n", users[i].username, users[i].password, users[i].email);
    }

    fclose(file);
}

// Function to load user accounts from file
int loadUserAccounts(UserAccount *users, int *userCount) {
    FILE *file = fopen("user_accounts.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "User accounts file not found. Starting with an empty user list.\n");
        return 0;
    }

    while (fscanf(file, "%s %s %s", users[*userCount].username, users[*userCount].password, users[*userCount].email) == 3) {
        (*userCount)++;
    }

    fclose(file);
    return 1;
}

// Function to save borrow records to file
void saveBorrowRecords(BorrowRecord *records, int count) {
    FILE *file = fopen("borrow_records.txt", "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening borrow records file for writing.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s %s %s %.2f %c\n", records[i].contact, records[i].phoneNumber, records[i].email, records[i].amount, records[i].type);
    }

    fclose(file);
}

// Function to load borrow records from file
int loadBorrowRecords(BorrowRecord *records, int *count) {
    FILE *file = fopen("borrow_records.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Borrow records file not found. Starting with an empty record list.\n");
        return 0;
    }

    while (fscanf(file, "%s %s %s %f %c", records[*count].contact, records[*count].phoneNumber, records[*count].email, &records[*count].amount, &records[*count].type) == 5) {
        (*count)++;
    }

    fclose(file);
    return 1;
}

// Function to create a new user account
void createAccount(UserAccount *users, int *userCount) {
    printf("Enter a new username: ");
    scanf("%s", users[*userCount].username);

   // Additional check for a valid username (lowercase and numbers only)
    int validUsername = 1;
    for (int i = 0; users[*userCount].username[i] != '\0'; i++) {
        if (!((users[*userCount].username[i] >= 'a' && users[*userCount].username[i] <= 'z') || (users[*userCount].username[i] >= '0' && users[*userCount].username[i] <= '9'))) {
            validUsername = 0;
            break;
        }
    }

    if (!validUsername) {
        displayError("Invalid username. Usernames must contain only lowercase letters and numbers.");
        return;
    }

    printf("Enter a new password: ");
    scanf("%s", users[*userCount].password);

     // Additional check for a valid password (uppercase, lowercase, and numbers)
    int hasUppercase = 0, hasLowercase = 0, hasDigit = 0;
    for (int i = 0; users[*userCount].password[i] != '\0'; i++) {
        if (users[*userCount].password[i] >= 'A' && users[*userCount].password[i] <= 'Z') {
            hasUppercase = 1;
        } else if (users[*userCount].password[i] >= 'a' && users[*userCount].password[i] <= 'z') {
            hasLowercase = 1;
        } else if (users[*userCount].password[i] >= '0' && users[*userCount].password[i] <= '9') {
            hasDigit = 1;
        }
    }

    if (!(hasUppercase && hasLowercase && hasDigit)) {
        displayError("Invalid password. Passwords must contain at least one uppercase letter, one lowercase letter, and one number.");
        return;
    }

    printf("Enter an email address: ");
    scanf("%s", users[*userCount].email);


      // Additional check for a valid email address
    int validEmail = 1;
    for (int i = 0; users[*userCount].email[i] != '\0'; i++) {
        if (!((users[*userCount].email[i] >= 'a' && users[*userCount].email[i] <= 'z') ||
              (users[*userCount].email[i] >= 'A' && users[*userCount].email[i] <= 'Z') ||
              (users[*userCount].email[i] >= '0' && users[*userCount].email[i] <= '9') ||
              users[*userCount].email[i] == '@' || users[*userCount].email[i] == '.'))
        {
            validEmail = 0;
            break;
        }
    }

    if (!validEmail) {
        displayError("Invalid email address.");
        return;
    }

    displaySuccess("Account created successfully!");
    (*userCount)++;
}

// Function to authenticate a user
int authenticate(UserAccount *users, int userCount, char *username, char *password)
{
    for (int i = 0; i < userCount; i++)
    {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0)
        {
            return i; // Authentication successful, return user index
        }
    }
    return -1; // Authentication failed
}

// Function to find a user by email
int findUserByEmail(UserAccount *users, int userCount, char *email)
{
    for (int i = 0; i < userCount; i++)
    {
        if (strcmp(users[i].email, email) == 0)
        {
            return i; // User found
        }
    }
    return -1; // User not found
}

// Function to validate an email address
int validateEmail(const char *email)
{
    int atCount = 0, dotCount = 0;

    for (int i = 0; email[i] != '\0'; i++)
    {
        if (email[i] == '@')
        {
            atCount++;
        }
        else if (email[i] == '.')
        {
            dotCount++;
        }
    }

    // Validate conditions: exactly one '@' and at least one '.' after '@'
    if (atCount == 1 && dotCount > 0 && email[strlen(email) - 1] != '.')
    {
        return 1; // Valid email
    }

    return 0; // Invalid email
}
// Function to notify the lender
void notifyLender(BorrowRecord *record, const char *lenderMessage);


// Function to notify borrower/lender through different channels

void notifyBorrower(BorrowRecord *record) {
    char message[256];

    if (record->type == 'B') {
        // Message for borrower
        snprintf(message, sizeof(message), "You borrowed %.2f. Please repay as soon as possible.", record->amount);
    } else if (record->type == 'L') {
        // Message for lender
        snprintf(message, sizeof(message), "I borrowed %.2f from you. I will repay you soon.", record->amount);
    } else {
        // Invalid record type
        printf("Invalid record type.\n");
        return;
    }

 // Function to notify the lender
void notifyLender(BorrowRecord *record, const char *lenderMessage) {
    char message[256];

    if (record->type == 'L') {
        // Message for lender
        snprintf(message, sizeof(message), lenderMessage, record->amount);

        // Notify via different channels
        printf("Notification sent to %s (%s): %s\n", record->contact, record->phoneNumber, message);

        // Simulate sending an email
        printf("Email Notification sent to %s: %s\n", record->contact, record->email);

        // Send SMS notification
        sendSMSNotification(record->phoneNumber, message);

        // You can add more notification methods here (WhatsApp, etc.) as needed
    } else {
        // Not a lender record, do nothing or handle differently if needed
        printf("Invalid record type for lender notification.\n");
    }
}


    // Notify via different channels
    printf("Notification sent to %s (%s): %s\n", record->contact, record->phoneNumber, message);

    // Simulate sending an email
    printf("Email Notification sent to %s: %s\n", record->contact, record->email);

    // Send SMS notification
    sendSMSNotification(record->phoneNumber, message);


}

// Function to add a new borrow record
void addBorrowRecord(BorrowRecord *records, int *count, char userType)
{
    printf("Enter contact name: ");
    scanf("%s", records[*count].contact);
    printf("Enter phone number: ");
    scanf("%s", records[*count].phoneNumber);
    printf("Enter email: ");
    scanf("%s", records[*count].email);
    printf("Enter amount: ");
    scanf("%f", &records[*count].amount);

    // Borrow or lend type
    records[*count].type = userType;

    // Notify the borrower/lender when a new record is added
    notifyBorrower(&records[*count]);

    (*count)++;
}

// Function to display all borrow records
void displayBorrowRecords(BorrowRecord *records, int count)
{
    printf("\nBorrow Records:\n");
    for (int i = 0; i < count; i++)
    {
        printf("%s - %s - %s - %.2f - %c\n", records[i].contact, records[i].phoneNumber, records[i].email, records[i].amount, records[i].type);
    }
}

// Function to simulate sending an email for password reset
void sendResetEmail(UserAccount *user)
{
    printf("Password reset email sent to %s. Check your email for further instructions.\n", user->email);
}

// Function to add a new lend record
void addLendRecord(BorrowRecord *records, int *count, char userType)
{
    printf("Enter contact name: ");
    scanf("%s", records[*count].contact);
    printf("Enter phone number: ");
    scanf("%s", records[*count].phoneNumber);
    printf("Enter email: ");
    scanf("%s", records[*count].email);
    printf("Enter amount: ");
    scanf("%f", &records[*count].amount);

    // Borrow or lend type
    records[*count].type = userType;

    // Notify the borrower/lender when a new record is added
    notifyBorrower(&records[*count]);

    (*count)++;
}

int main()
{
    int capacity = 10; // Initial capacity for borrow records
    BorrowRecord *borrowRecords = (BorrowRecord *)malloc(capacity * sizeof(BorrowRecord));
    int recordCount = 0;

    int userCapacity = 10; // Initial capacity for user accounts
    UserAccount *userAccounts = (UserAccount *)malloc(userCapacity * sizeof(UserAccount));
    int userCount = 0;

    if (borrowRecords == NULL || userAccounts == NULL)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    int choice;
    char username[50];
    char password[50];
    char email[100];

    // Load user accounts and borrow records from files
    if (!loadUserAccounts(userAccounts, &userCount))
    {
        printf("Failed to load user accounts.\n");
    }

    if (!loadBorrowRecords(borrowRecords, &recordCount))
    {
        printf("Failed to load borrow records.\n");
    }

    do
    {
        printf("\nDHAR - Borrow Management System\n");
        printf("1. Create Account\n");
        printf("2. Login\n");
        printf("3. Forget Password\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            if (userCount == userCapacity)
            {
                userCapacity *= 2; // Double the capacity if needed
                userAccounts = realloc(userAccounts, userCapacity * sizeof(UserAccount));
                if (userAccounts == NULL)
                {
                    fprintf(stderr, "Memory reallocation failed.\n");
                    return 1;
                }
            }
            createAccount(userAccounts, &userCount);
            saveUserAccounts(userAccounts, userCount);
            break;
        case 2:
            printf("Enter username: ");
            scanf("%s", username);
            printf("Enter password: ");
            scanf("%s", password);

            int loggedInUserIndex = authenticate(userAccounts, userCount, username, password);

            if (loggedInUserIndex != -1)
            {
                printf("Login successful.\n");

                // After login, show additional menu for borrow records
                do
                {
                    printf("\n1. Add Borrow Record\n");
                    printf("2. Display Borrow Records\n");
                    printf("3. Notify Borrower\n");
                    printf("4. Display Lend Records\n"); // Added option to display lend records
                    printf("5. Notify Lender\n");        // Added option to notify lender
                    printf("6. Add Lend Record\n");       // Added option to add lend record
                    printf("7. Logout\n");
                    printf("Enter your choice: ");
                    scanf("%d", &choice);

                    switch (choice)
                    {
                    case 1:
                        if (recordCount == capacity)
                        {
                            capacity *= 2; // Double the capacity if needed
                            borrowRecords = realloc(borrowRecords, capacity * sizeof(BorrowRecord));
                            if (borrowRecords == NULL)
                            {
                                fprintf(stderr, "Memory reallocation failed.\n");
                                return 1;
                            }
                        }
                        printf("Select record type (B for Borrow, L for Lend): ");
                        scanf(" %c", &choice);
                        if (choice == 'B' || choice == 'L')
                        {
                            addBorrowRecord(borrowRecords, &recordCount, choice);
                        }
                        else
                        {
                            printf("Invalid record type. Please enter B or L.\n");
                        }
                        saveBorrowRecords(borrowRecords, recordCount);
                        break;
                    case 2:
                        displayBorrowRecords(borrowRecords, recordCount);
                        break;
                    case 3:
                        if (recordCount > 0)
                        {
                            notifyBorrower(&borrowRecords[0]);
                        }
                        else
                        {
                            printf("No records to notify.\n");
                        }
                        break;
                    case 4:
                                               // Display lend records
                        printf("\nLend Records:\n");
                        for (int i = 0; i < recordCount; i++)
                        {
                            if (borrowRecords[i].type == 'L')
                            {
                                printf("%s - %s - %s - %.2f - %c\n", borrowRecords[i].contact, borrowRecords[i].phoneNumber, borrowRecords[i].email, borrowRecords[i].amount, borrowRecords[i].type);
                            }
                        }
                        break;
                    case 5:
                        // Notify lender
                        if (recordCount > 0)
                        {
                            printf("Notify the lender of the first record:\n");
                            notifyBorrower(&borrowRecords[0]);
                        }
                        else
                        {
                            printf("No records to notify.\n");
                        }
                        break;
                    case 6:
                        // Add Lend Record
                        if (recordCount == capacity)
                        {
                            capacity *= 2; // Double the capacity if needed
                            borrowRecords = realloc(borrowRecords, capacity * sizeof(BorrowRecord));
                            if (borrowRecords == NULL)
                            {
                                fprintf(stderr, "Memory reallocation failed.\n");
                                return 1;
                            }
                        }
                        addLendRecord(borrowRecords, &recordCount, 'L');
                        saveBorrowRecords(borrowRecords, recordCount);
                        break;
                    case 7:
                        printf("Logging out.\n");
                        break;
                    default:
                        printf("Invalid choice. Please try again.\n");
                    }
                } while (choice != 7);
            }
            else
            {
                printf("Login failed. Invalid username or password.\n");
            }
            break;
        case 3:
            printf("Enter email address: ");
            scanf("%s", email);

            int userIndex = findUserByEmail(userAccounts, userCount, email);
            if (userIndex != -1)
            {
                sendResetEmail(&userAccounts[userIndex]);
            }
            else
            {
                printf("User with this email address not found.\n");
            }
            break;
        case 4:
            printf("Exiting DHAR. Goodbye!\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 4);
     // Example usage of setConsoleColor
    setConsoleColor('S'); // Set color to success (green text, white background)
    printf("This is a success message.\n");
    resetConsoleColor(); // Reset color to default


    // Free allocated memory before exiting
    free(borrowRecords);
    free(userAccounts);

    return 0;
}
