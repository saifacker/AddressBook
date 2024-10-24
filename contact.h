#ifndef CONTACT_H
#define CONTACT_H

#define MAX_CONTACTS 100
#define MAX_NAME_LEN 50
#define MAX_PHONE_LEN 15
#define MAX_EMAIL_LEN 50

typedef struct {
    char name[MAX_NAME_LEN];
    char phone[MAX_PHONE_LEN];
    char email[MAX_EMAIL_LEN];
} Contact;


typedef struct {
    Contact contacts[MAX_CONTACTS];
    int contactCount;
} AddressBook;

void initialize(AddressBook *addressBook);
void createContact(AddressBook *addressBook);
void searchContact(const AddressBook *addressBook);  
void editContact(AddressBook *addressBook);
void deleteContact(AddressBook *addressBook);
void listContacts(const AddressBook *addressBook);    
void saveContactsToFile(const AddressBook *addressBook);
void escapeCSVField(const char *field, char *escapedField);


#endif
