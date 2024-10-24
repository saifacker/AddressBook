#include <stdio.h>
#include "contact.h"
#include<ctype.h>
#define BOLD    "\033[1m"
#define UNDERLINE "\033[4m"
#define RESET   "\033[0m"

int main() {
    AddressBook addressBook;
    initialize(&addressBook);

    int choice;
    char continueChoice;

    do {
        printf("\n" BOLD "::::ADDRESS BOOK::::\n" RESET);
        printf("1. Create Contact\n");
        printf("2. Edit Contact\n");
        printf("3. Delete Contact\n");
        printf("4. List Contacts\n");
        printf("5. Search Contact\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 4:
                listContacts(&addressBook);
                break;
            case 1:
                createContact(&addressBook);
                break;
            case 2:
                editContact(&addressBook);
                break;
            case 3:
                deleteContact(&addressBook);
                break;
            case 5:
                searchContact(&addressBook);
                break;
            case 6:
            printf("Exiting the program... Goodbye! :)");

                return 0;  
            default:
                printf("Invalid choice. Please enter a number between 1 and 6.\n");
        }

        printf("Do you want to continue? (Y/N): ");
        scanf(" %c", &continueChoice);
      continueChoice = tolower(continueChoice);

    } while (continueChoice == 'y');

    printf("Exiting the program...\n");
    return 0;
}