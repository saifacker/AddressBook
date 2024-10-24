#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "contact.h"
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLD    "\033[1m"
#define UNDERLINE "\033[4m"
char *strcasestr(const char *haystack, const char *needle);

void printEqualLine() {
    printf("+------------+--------------------------------+-----------------+--------------------------------+ \n");
}


void listContacts(const AddressBook *addressBook) {
    printEqualLine();
    printf("| " CYAN BOLD "%-10s " RESET "| " CYAN BOLD "%-30s " RESET "| " CYAN BOLD "%-15s " RESET "| " CYAN BOLD "%-30s " RESET "|"RESET"\n", 
           "S.No", "Name", "Phone No", "Email ID");
    printEqualLine();

    for (int i = 0; i < addressBook->contactCount; i++) {
        printf("| " WHITE "%-10d " RESET "| " YELLOW "%-30s " RESET "| " MAGENTA "%-15s " RESET "| " RED "%-30s "RESET "|" RESET "\n", 
               i + 1, 
               addressBook->contacts[i].name, 
               addressBook->contacts[i].phone, 
               addressBook->contacts[i].email);
    }

    printEqualLine();
}

void initialize(AddressBook *addressBook) {
    FILE *file = fopen("contacts.csv", "r");  
    if (file == NULL) {
        addressBook->contactCount = 0; 
        return;
    }

    char line[200];
    addressBook->contactCount = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';

        char *token = strtok(line, ",");
        if (token != NULL) {
            strncpy(addressBook->contacts[addressBook->contactCount].name, token, MAX_NAME_LEN - 1);
            addressBook->contacts[addressBook->contactCount].name[MAX_NAME_LEN - 1] = '\0';  
        }

        token = strtok(NULL, ",");
        if (token != NULL) {
            strncpy(addressBook->contacts[addressBook->contactCount].phone, token, MAX_PHONE_LEN - 1);
            addressBook->contacts[addressBook->contactCount].phone[MAX_PHONE_LEN - 1] = '\0';  
        }

        token = strtok(NULL, ",");
        if (token != NULL) {
            strncpy(addressBook->contacts[addressBook->contactCount].email, token, MAX_EMAIL_LEN - 1);
            addressBook->contacts[addressBook->contactCount].email[MAX_EMAIL_LEN - 1] = '\0';  
        }

        addressBook->contactCount++;

        if (addressBook->contactCount >= MAX_CONTACTS) {
            break;
        }
    }

    fclose(file);
}

void createContact(AddressBook *addressBook) {
    char name[60], phone[20], email[50];
    int validEmail = 0;
    int validPhone = 0;

    printf("Enter the name: ");
    scanf(" %[^\n]", name);

    for (int i = 0; i < addressBook->contactCount; i++) {
        // if (strcasecmp(name, addressBook->contacts[i].name) == 0) {
        //     printf("Name already exists. Please enter a different name.\n");
        //     return;
        // }
    }

    while (!validPhone) {
        printf("Enter the phone number: ");
        scanf(" %[^\n]", phone);

        if (strlen(phone) == 10 && strspn(phone, "0123456789") == 10) {
            validPhone = 1;
        } else {
            printf("Invalid phone number. Please enter a 10-digit number.\n");
        }
    }

while (!validEmail) {
        printf("Enter the email: ");
        scanf(" %[^\n]", email);

        char *atSign = strchr(email, '@');
        if (atSign != NULL) {
            char *domain = atSign + 1; // Skip '@'
            char *dot = strchr(domain, '.');
            if (dot != NULL && strcmp(dot, ".com") == 0 && dot - domain > 0) {
                validEmail = 1; 
            } else {
                printf("Invalid email format. Please enter an email in the format name@example.com.\n");
            }
        } else {
            printf("Invalid email format. Please enter an email in the format name@example.com.\n");
        }
    }
    strcpy(addressBook->contacts[addressBook->contactCount].name, name);
    strcpy(addressBook->contacts[addressBook->contactCount].phone, phone);
    strcpy(addressBook->contacts[addressBook->contactCount].email, email);
    addressBook->contactCount++;

    saveContactsToFile(addressBook);
    printf("Contact added successfully.\n");
}

void editContact(AddressBook *addressBook) {
    char search[100];
    printf("Enter the name, phone number, or email to edit: ");
    scanf(" %[^\n]", search);

    int index = -1;
    for (int i = 0; i < addressBook->contactCount; i++) {
        if (strcmp(addressBook->contacts[i].name, search) == 0 ||
            strcmp(addressBook->contacts[i].phone, search) == 0 ||
            strcmp(addressBook->contacts[i].email, search) == 0) {
            index = i;
            break;
        }
    }

    if (index != -1) {
        int choice;
        printf("Contact found. What would you like to edit?\n");
        printf("1. Name\n2. Phone Number\n3. Email\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter new name: ");
                scanf(" %[^\n]", addressBook->contacts[index].name);
                break;
            case 2:
                printf("Enter new phone number: ");
                scanf(" %[^\n]", addressBook->contacts[index].phone);
                break;
            case 3: {
                int validEmail = 0;
                while (!validEmail) {
                    printf("Enter new email: ");
                    scanf(" %[^\n]", addressBook->contacts[index].email);

                    char *atSign = strchr(addressBook->contacts[index].email, '@');
                    if (atSign != NULL) {
                        char *domain = atSign + 1;
                        if (strlen(domain) >= 4 &&
                            strcmp(domain + strlen(domain) - 4, ".com") == 0) {
                            validEmail = 1; 
                        } else {
                            printf("Invalid email format. Please enter an email in the format name@domain.com.\n");
                        }
                    } else {
                        printf("Invalid email format. Please enter an email in the format name@domain.com.\n");
                    }
                }
                break;
            }
            default:
                printf("Invalid choice.\n");
                return;
        }

        saveContactsToFile(addressBook);
        printf("Contact updated successfully.\n");
    } else {
        printf("Contact not found.\n");
    }
}


void deleteContact(AddressBook *addressBook) {
    if (addressBook->contactCount == 0) {
        printf("Address book is empty. No contact to delete.\n");
        return;
    }

    char search[100];
    printf("Enter the name to delete: ");
    scanf(" %[^\n]", search);

    int matchingIndexes[100]; 
    int matchCount = 0;

    for (int i = 0; i < addressBook->contactCount; i++) {
        if (strcasecmp(addressBook->contacts[i].name, search) == 0) {
            matchingIndexes[matchCount] = i;
            matchCount++;
        }
    }

    if (matchCount == 0) {
        printf("No contacts found with the name '%s'.\n", search);
        return;
    }

    if (matchCount == 1) {
        printf("Found 1 contact: \n");
    } else {
        printf("Found %d contacts:\n", matchCount);
    }

    for (int i = 0; i < matchCount; i++) {
        int index = matchingIndexes[i];
        printf("%d. Name: %s, Phone: %s, Email: %s\n", 
               i + 1, 
               addressBook->contacts[index].name, 
               addressBook->contacts[index].phone, 
               addressBook->contacts[index].email);
    }

    int choice;
    if (matchCount > 1) {
        printf("Enter the number of the contact to delete: ");
        scanf("%d", &choice);
        if (choice < 1 || choice > matchCount) {
            printf("Invalid choice. Operation aborted.\n");
            return;
        }
    } else {
        choice = 1; 
    }

    int indexToDelete = matchingIndexes[choice - 1];

    for (int i = indexToDelete; i < addressBook->contactCount - 1; i++) {
        addressBook->contacts[i] = addressBook->contacts[i + 1];
    }
    addressBook->contactCount--;

    saveContactsToFile(addressBook);
    printf("Contact deleted successfully.\n");
}


void searchContact(const AddressBook *addressBook) {
    char search[100];
    printf("Enter the name to search: ");
    scanf(" %[^\n]", search);

    printEqualLine();
    printf("| " CYAN BOLD "%-10s " RESET  "| " CYAN BOLD "%-30s " RESET "| " CYAN BOLD "%-15s " RESET "| " CYAN BOLD "%-30s " RESET "|"RESET"\n", 
           "S.No", "Name", "Phone No", "Email ID");
    printEqualLine();


    int found = 0;
    for (int i = 0; i < addressBook->contactCount; i++) {
        if (strcasestr(addressBook->contacts[i].name, search) != NULL) {
            found = 1;
            printf("| " WHITE "%-10d " RESET "| " GREEN "%-30s " RESET "| " MAGENTA "%-15s " RESET "| " RED "%-30s "RESET"|" RESET "\n", 
                   i + 1,  addressBook->contacts[i].name,
                   addressBook->contacts[i].phone, addressBook->contacts[i].email);
        }
    }

    if (!found) {
        printf("| " RED "%-90s " RESET "|\n", "No contact found.");
    }

    printEqualLine();
}
void saveContactsToFile(const AddressBook *addressBook) {
    FILE *file = fopen("contacts.csv", "w");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    for (int i = 0; i < addressBook->contactCount; i++) {
        fprintf(file, "%s,%s,%s\n",
                addressBook->contacts[i].name,
                addressBook->contacts[i].phone,
                addressBook->contacts[i].email);
    }

    fclose(file);
}
char *strcasestr(const char *haystack, const char *needle) {
    if (!*needle)
        return (char *)haystack;

    for (const char *p = haystack; *p; p++) {
        const char *h = p;
        const char *n = needle;
        while (*h && *n && tolower((unsigned char)*h) == tolower((unsigned char)*n)) {
            h++;
            n++;
        }
        if (!*n)
            return (char *)p;
    }
    return NULL;
}
