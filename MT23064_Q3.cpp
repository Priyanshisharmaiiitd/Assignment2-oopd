#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>

using namespace std;

const int MAX_BOOKS = 100;
const int MAX_MAGAZINES = 81;
const int MAX_JOURNALS = 99;
const int MAX_STUDENTS = 100;
const int MAX_FACULTY = 100;

class LibraryItem {
public:
    string item_id;
    string title;
    string original_title;
    LibraryItem() {}
};

class Book : public LibraryItem {
public:
    string isbn;
    string authors;
    string count;
};

class Magazine : public LibraryItem {
public:
    string publication;
    string rank;
};

class Journal : public LibraryItem {
public:
    string publisher;
    bool isSubscribed;
};

class User {
public:
    string user_name;
    string user_id;
    string user_type;
    bool is_borrowed;
    time_t request_date;
    time_t due_date;
    string borrowed_item_id;
};

int readBooks(Book books[]) {
    ifstream booksFile("books.csv");
    if (!booksFile.is_open()) {
        cerr << "Error: Unable to open books.csv" << endl;
        return 1;
    }

    string line;
    getline(booksFile, line);

    int booksCount = 0;
    char delimiter = ',';

    while (getline(booksFile, line)) {
        stringstream ss(line);

        getline(ss, books[booksCount].item_id, delimiter);
        getline(ss, books[booksCount].count, delimiter);
        getline(ss, books[booksCount].isbn, delimiter);
        getline(ss, books[booksCount].authors, delimiter);
        getline(ss, books[booksCount].title, delimiter);
        getline(ss, books[booksCount].original_title, delimiter);

        booksCount++;

        if (booksCount >= MAX_BOOKS) {
            cerr << "Warning: Maximum number of books reached." << endl;
            break;
        }
    }

    booksFile.close();
    return booksCount;
}

int readMagazines(Magazine magazines[]) {
    ifstream magazinesFile("magazines.csv");
    if (!magazinesFile.is_open()) {
        cerr << "Error: Unable to open magazines.csv" << endl;
        return 1;
    }

    string line;
    getline(magazinesFile, line);

    int magazinesCount = 0;
    char delimiter = ',';

    while (getline(magazinesFile, line)) {
        stringstream ss(line);

        getline(ss, magazines[magazinesCount].publication, delimiter);
        getline(ss, magazines[magazinesCount].rank, delimiter);

        magazinesCount++;

        if (magazinesCount >= MAX_MAGAZINES) {
            cerr << "Warning: Maximum number of magazines reached." << endl;
            break;
        }
    }

    magazinesFile.close();
    return magazinesCount;
}

int readJournals(Journal journals[]) {
    ifstream journalFile("journals.csv");
    if (!journalFile.is_open()) {
        cerr << "Error: Unable to open journals.csv" << endl;
        return 1;
    }

    int journalsCount = 0;

    string line;
    while (getline(journalFile, line)) {
    if (journalsCount < MAX_JOURNALS) {
        stringstream ss(line);
        getline(ss, journals[journalsCount].title, ',');
    } else {
        cerr << "Warning: Maximum number of journals reached." << endl;
        break;
    }
}

    
   
    journalFile.close();
    return journalsCount;
}

void borrowBook(Book books[], User users[], int& userCount, int booksCount) {
    string book_id;
    cout << "Enter book_id: ";
    cin >> book_id;

    // Find the user who is requesting to borrow the book
    int userIndex = -1;
    for (int i = 0; i < userCount; i++) {
        if (users[i].user_id == users[userCount - 1].user_id) {
            userIndex = i;
            break;
        }
    }

    if (userIndex == -1) {
        cout << "Error: User not found." << endl;
        return;
    }

    for (int i = 0; i < booksCount; i++) {
        if (book_id == books[i].item_id && stoi(books[i].count) > 0) {
            bool alreadyBorrowed = false;
            for (int j = 0; j < userCount; j++) {
                if (users[j].user_id == users[userIndex].user_id && users[j].borrowed_item_id == book_id) {
                    alreadyBorrowed = true;
                    break;
                }
            }

            if (alreadyBorrowed) {
                cout << "You have already borrowed this book. Please return it before borrowing again." << endl;
                return;
            }

            cout << "Book borrowed successfully" << endl;
            books[i].count = to_string(stoi(books[i].count) - 1);

            users[userIndex].is_borrowed = true;
            time(&users[userIndex].request_date);

            struct tm* due_date_tm;
            time_t now = time(nullptr);
            due_date_tm = localtime(&now);

            if (users[userIndex].user_type == "student") {
                due_date_tm->tm_mon += 1;
            } else if (users[userIndex].user_type == "faculty") {
                due_date_tm->tm_mon += 6;
            }

            users[userIndex].due_date = mktime(due_date_tm);
            users[userIndex].borrowed_item_id = book_id;

            return;
        }
    }

    cout << "Sorry, all copies of this book are already borrowed or the book does not exist." << endl;
}

void borrowMagazine(Magazine magazines[], User users[], int& userCount, int magazinesCount) {
    string magazine_rank;
    cout << "Enter magazine_rank: ";
    cin >> magazine_rank;
    bool magazineExists = false;
    for (int i = 0; i < magazinesCount; i++) {
        if (magazine_rank == magazines[i].rank && stoi(magazines[i].rank) > 0) {
            magazineExists = true;
            break;
        }
    }

    if (!magazineExists) {
        cout << "Sorry, this magazine does not exist." << endl;
        return;
    }
    for (int i = 0; i < userCount; i++) {
        if (users[i].user_id == users[userCount].user_id && users[i].borrowed_item_id == magazine_rank) {
            cout << "You have already borrowed this magazine. Please return it before borrowing again." << endl;
            return;
        }
    }
    for (int i = 0; i < magazinesCount; i++) {
        if (magazine_rank == magazines[i].rank && stoi(magazines[i].rank) > 0) {
            cout << "Magazine borrowed successfully" << endl;
            magazines[i].rank = to_string(stoi(magazines[i].rank) - 1);

            users[userCount].is_borrowed = true;
            time(&users[userCount].request_date);
            users[userCount].due_date = -1;
            users[userCount].borrowed_item_id = magazine_rank;

            userCount++;
            return;
        }
    }

    cout << "Sorry, this magazine is already borrowed or the magazine does not exist." << endl;
}

void borrowJournal(Journal journals[], User users[], int& userCount, int journalsCount) {
    string journal_title;
    cout << "Enter journal title: ";
    cin.ignore();
    getline(cin, journal_title);
    bool journalExists = false;
    for (int i = 0; i < journalsCount; i++) {
        if (journal_title == journals[i].title) {
            journalExists = true;
            break;
        }
    }

    if (!journalExists) {
        cout << "Sorry, this journal does not exist." << endl;
        return;
    }
    for (int i = 0; i < userCount; i++) {
        if (users[i].user_id == users[userCount].user_id && users[i].borrowed_item_id == journal_title) {
            cout << "You have already borrowed this journal. Please return it before borrowing again." << endl;
            return;
        }
    }

   
    cout << "Journal borrowed successfully" << endl;

    users[userCount].is_borrowed = true;
    time(&users[userCount].request_date);
    users[userCount].due_date = -1; // No due date for journals
    users[userCount].borrowed_item_id = journal_title;

    userCount++;
}


void subscribeJournal(Journal journals[], int journalsCount) {
    string journal_title;
    cout << "Enter the title of the journal you want to subscribe to: ";
    cin.ignore();
    getline(cin, journal_title);

    for (int i = 0; i < journalsCount; i++) {
        if (journal_title == journals[i].title) {
            if (journals[i].isSubscribed) {
                cout << "You are already subscribed to this journal." << endl;
            } else {
                cout << "Subscribed to the journal: " << journals[i].title << endl;
                journals[i].isSubscribed = true;
            }
            return;
        }
    }

    cout << "The specified journal is not available in the library." << endl;
}

void searchLocationBook(Book books[], int booksCount) {
    string book_id;
    cout << "Enter book_id to search: ";
    cin >> book_id;

    for (int i = 0; i < booksCount; i++) {
        if (book_id == books[i].item_id) {
            cout << "Location of Book with ID " << book_id << " is on Shelf 1, Section A" << endl;
            return;
        }
    }

    cout << "Book with ID " << book_id << " not found in the library." << endl;
}

void searchLocationMagazine(Magazine magazines[], int magazinesCount) {
    string magazine_rank;
    cout << "Enter magazine_rank to search: ";
    cin >> magazine_rank;

    for (int i = 0; i < magazinesCount; i++) {
        if (magazine_rank == magazines[i].rank) {
            cout << "Location of Magazine with Rank " << magazine_rank << " is on Shelf 2, Section B" << endl;
            return;
        }
    }

    cout << "Magazine with Rank " << magazine_rank << " not found in the library." << endl;
}

void printBooks(Book books[], int booksCount) {
    cout << "\nBooks in Library:" << endl;
    for (int i = 0; i < booksCount; i++) {
        cout << "Book " << i + 1 << ": " << books[i].item_id << ", " << books[i].title << ", " << books[i].isbn << ", " << books[i].authors << ", " << books[i].original_title << ", Count: " << books[i].count << endl;
    }
}

void printMagazines(Magazine magazines[], int magazinesCount) {
    cout << "\nMagazines in Library:" << endl;
    for (int i = 0; i < magazinesCount; i++) {
        cout << "Magazine " << i + 1 << ": " << magazines[i].publication << ", Rank: " << magazines[i].rank << endl;
    }
}

void printJournals(Journal journals[], int journalsCount) {
    cout << "\nJournals in Library:" << endl;
    for (int i = 0; i < journalsCount; i++) {
        cout << "Journal " << i + 1 << ": " << journals[i].title << ", Publisher: " << journals[i].publisher;
        if (journals[i].isSubscribed) {
            cout << " (Subscribed)";
        }
        cout << endl;
    }
}

int main() {
    Book books[MAX_BOOKS];
    Magazine magazines[MAX_MAGAZINES];
    Journal journals[MAX_JOURNALS];
    User users[MAX_STUDENTS + MAX_FACULTY];

    int booksCount = readBooks(books);
    int magazinesCount = readMagazines(magazines);
    int journalsCount = readJournals(journals);
    int userCount = 0;
    printBooks(books, booksCount);
    printMagazines(magazines, magazinesCount);
    printJournals(journals, journalsCount);

    int choice;
    string item_type; 

    do {
        cout << "\nMenu:" << endl;
        cout << "1. Borrow an item" << endl;
        cout << "2. Search for item location" << endl;
        cout << "3. Register a new user" << endl;
        cout << "4. Purchase a new book" << endl;
        cout << "5. Exit" << endl;
        cout << "6. Subscribe to a journal" << endl; 

        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter user name: ";
                cin >> users[userCount].user_name;
                cout << "Enter user id: ";
                cin >> users[userCount].user_id;
                cout << "Enter user type (student or faculty): ";
                cin >> users[userCount].user_type;

                if (users[userCount].user_type == "student" || users[userCount].user_type == "faculty") {
                    cout << "Enter item type (book, magazine, or journal): ";
                    cin >> item_type; 

                    if (item_type == "book") {
                        borrowBook(books, users, userCount, booksCount);
                    } else if (item_type == "magazine") {
                        borrowMagazine(magazines, users, userCount, magazinesCount);
                    } else if (item_type == "journal") {
                        borrowJournal(journals, users, userCount, journalsCount);
                    } else {
                        cout << "Invalid item type." << endl;
                    }
                } else {
                    cout << "Invalid user type." << endl;
                }
                break;
            case 2:
                cout << "Enter item type to search (book, magazine, or journal): ";
                cin >> item_type; 

                if (item_type == "book") {
                    searchLocationBook(books, booksCount);
                } else if (item_type == "magazine") {
                    searchLocationMagazine(magazines, magazinesCount);
                } else if (item_type == "journal") {
                    cout << "Search for journals is not implemented." << endl;
                } else {
                    cout << "Invalid item type." << endl;
                }
                break;
            case 3:
                if (userCount < MAX_STUDENTS + MAX_FACULTY) {
                    cout << "Enter user name: ";
                    cin >> users[userCount].user_name;
                    cout << "Enter user id: ";
                    cin >> users[userCount].user_id;
                    cout << "Enter user type (student or faculty): ";
                    cin >> users[userCount].user_type;
                    users[userCount].is_borrowed = false;
                    users[userCount].due_date = 0;
                    users[userCount].borrowed_item_id = "";
                    userCount++;
                } else {
                    cout << "Maximum number of users reached." << endl;
                }
                break;
            case 4:
                if (booksCount < MAX_BOOKS) {
                    cout << "Enter book_id: ";
                    cin >> books[booksCount].item_id;
                    cout << "Enter book title: ";
                    cin.ignore();
                    getline(cin, books[booksCount].title);
                    cout << "Enter book original title: ";
                    getline(cin, books[booksCount].original_title);
                    cout << "Enter book ISBN: ";
                    cin >> books[booksCount].isbn;
                    cout << "Enter book authors: ";
                    cin.ignore();
                    getline(cin, books[booksCount].authors);
                    cout << "Enter book count: ";
                    cin >> books[booksCount].count;
                    booksCount++;
                } else {
                    cout << "Maximum number of books reached." << endl;
                }
                break;
            case 6:
                subscribeJournal(journals, journalsCount);
                break;
            case 5:
                cout << "Exiting the program." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 5);

    return 0;
}
