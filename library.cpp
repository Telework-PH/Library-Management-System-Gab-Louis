#include <algorithm>
#include <climits>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <strstream>
#include <vector>

using namespace std;
int inputChoice;
string bookPath = "book.csv";
string patronPath = "patron.csv";
const int maxrow = 10;
char token = ',';

vector<string> headers = {"BOOK ID", "TITLE", "AUTHOR", "PUBLISHER", "STATUS"};

struct Book {
    int id;
    string title, author, publisher, availability;
};

struct Patron {
    int id;
    string name;
    string contactDetails;
};
struct CheckOutRecord {
    int patronID;
    int bookID;
    time_t checkOutDate;
    time_t dueDate;
    bool returned;
};
vector<Book> books;
vector<Patron> patrons;
vector<CheckOutRecord> checkOutRecords;
void header() {
    cout << setw(50);
    cout << "WELCOME TO TELEWORKPH LIBRARY!" << endl;

    cout << "\n\n\n\n\n\n";
    cout << "Enter to continue. . .";
    cin.ignore();
}

void clearScreen() {
    system("cls");
}

void errValidation() {
    while (!(cin >> inputChoice)) {
        cin.clear();
        cin.ignore(9999, '\n');
        cout << "Invalid input. Please enter again: ";
    }
}

string toLower(const string& str) {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

void addBook() {
    Book book;
    fstream fout;
    fout.open(bookPath, ios::out | ios::app);
    cout << "Book Id: ";
    cin >> book.id;
    ifstream file(bookPath);
    string line;
    while (getline(file, line)) {
        string id, title, author, publisher, availability;
        istrstream iss(line.c_str());
        getline(iss, id, token);
        if (id == "ID") {
            continue;
        }

        // Add a check to verify if id is a valid integer
        try {
            if (stoi(id) == book.id) {
                cout << "Book with the same ID already exists. Please enter a unique ID." << endl;
                file.close();
                return;
            }
        } catch (const std::invalid_argument&) {
            cout << "Invalid ID found in the file. Please check the file format." << endl;
            file.close();
            return;
        }
    }
    file.close();
    cout << "Book Title: ";
    cin.ignore();
    getline(cin, book.title);
    cout << "Book Author: ";
    getline(cin, book.author);
    cout << "Book Publisher: ";
    getline(cin, book.publisher);
    cout << "Book Status: ";
    getline(cin, book.availability);
    book.availability = toLower(book.availability);  // Store the lowercase status
    if (book.availability == "available") {
        bool status = true;
    } else if (book.availability == "unavailable") {
        bool status = false;
    } else {
        cout << "Wrong input" << endl;
    }
    fout << book.id << token << book.title << token << book.author << token << book.publisher << token << book.availability << endl;
    fout.close();
}

void searchBook() {
    ifstream file(bookPath);
    if (!file.is_open()) {
        cout << "Error opening file." << endl;
        return;
    }
    vector<Book> book_infos;
    //    cin.ignore();
    string search;
    cout << "Search (ID/Title/Author/Publisher): ";
    getline(cin, search);
    cout << endl;
    bool search_found = false;
    string line;
    string lowerTitleToFind = toLower(search);
    vector<string> found_lines;
    while (getline(file, line)) {
        string lowerLine = toLower(line);
        size_t found = lowerLine.find(lowerTitleToFind);
        if (found != string::npos) {
            search_found = true;
            found_lines.push_back(line);
        }
    }
    file.close();
    if (!search_found) {
        cout << "Book not found!" << endl;
    } else {
        cout << "ID, Title, Author, Publisher" << endl;
        for (const string& found_line : found_lines) {
            cout << found_line << endl;
        }
    }
}

void updateBook(int search) {
    vector<Book> book_infos;
    ifstream book(bookPath);
    string line;
    while (book.peek() != EOF) {
        string id, title, author, publisher, availability;
        getline(book, id, token);
        if (id == "ID") {
            book.ignore(INT_MAX, '\n');
            continue;
        }
        getline(book, title, token);
        getline(book, author, token);
        getline(book, publisher, token);
        getline(book, availability);
        book_infos.push_back({stoi(id), title, author, publisher, availability});
    }

    for (int i = 0; i < book_infos.size(); i++) {
        if (search == book_infos[i].id) {
            cout << "Update Title: ";
            getline(cin >> ws, book_infos[i].title);
            cout << "Update Author: ";
            getline(cin >> ws, book_infos[i].author);
            cout << "Update Publisher: ";
            getline(cin >> ws, book_infos[i].publisher);
            cout << "Update Status: ";
            getline(cin >> ws, book_infos[i].availability);
            toLower(book_infos[i].availability);
            break;
        }
    }

    ofstream file(bookPath);
    for (const auto& hi : book_infos) {
        file << hi.id << token
             << hi.title << token
             << hi.author << token
             << hi.publisher << token
             << hi.availability << '\n';
        file.close();
    }
}

void deleteBook(int search) {
    vector<Book> book_infos;
    ifstream book(bookPath);
    string line;
    while (getline(book, line)) {
        string id, title, author, publisher, availability;
        istrstream iss(line.c_str());
        getline(iss, id, token);
        if (id == "ID") {
            continue;
        }
        getline(iss, title, token);
        getline(iss, author, token);
        getline(iss, publisher, token);
        getline(iss, availability);
        book_infos.push_back({stoi(id), title, author, publisher, availability});
    }

    for (size_t i = 0; i < book_infos.size(); i++) {
        if (search == book_infos[i].id) {
            book_infos.erase(book_infos.begin() + i);
            break;
        }
    }

    ofstream file(bookPath);
    for (const auto& hi : book_infos) {
        file << hi.id << token
             << hi.title << token
             << hi.author << token
             << hi.publisher << token
             << hi.availability << '\n';
        file.close();
    }
}

void displayBooks() {
    ifstream file(bookPath);
    if (!file.is_open()) {
        cout << "Error opening file." << endl;
        return;
    }
    string line;
    while (getline(file, line)) {
        cout << line << "\n";
    }
    file.close();
}
void addPatron() {
    Patron patron;
    fstream fout;
    fout.open(patronPath, ios::out | ios::app);
    cout << "Patron ID: ";
    cin >> patron.id;
    ifstream file(patronPath);
    string line;
    while (getline(file, line)) {
        string id, name, contactDetails;
        istrstream iss(line.c_str());
        getline(iss, id, token);
        if (id == "ID") {
            continue;
        }

        // Add a check to verify if id is a valid integer
        try {
            if (stoi(id) == patron.id) {
                cout << "Patron with the same ID already exists. Please enter a unique ID." << endl;
                file.close();
                return;
            }
        } catch (const std::invalid_argument&) {
            cout << "Invalid ID found in the file. Please check the file format." << endl;
            file.close();
            return;
        }
    }
    file.close();
    cout << "Patron Name: ";
    cin.ignore();
    getline(cin, patron.name);
    cout << "Contact Details: ";
    getline(cin, patron.contactDetails);
    fout << patron.id << token << patron.name << token << patron.contactDetails << endl;
    fout.close();
}

void searchPatron() {
    ifstream file(patronPath);
    if (!file.is_open()) {
        cout << "Error opening file." << endl;
        return;
    }
    vector<Patron> patron_infos;
    string search;
    cout << "Search (ID/Name): ";
    getline(cin, search);
    cout << endl;
    bool search_found = false;
    string line;
    string lowerSearchToFind = toLower(search);
    vector<string> found_lines;
    while (getline(file, line)) {
        string lowerLine = toLower(line);
        size_t found = lowerLine.find(lowerSearchToFind);
        if (found != string::npos) {
            search_found = true;
            found_lines.push_back(line);
        }
    }
    file.close();
    if (!search_found) {
        cout << "Patron not found!" << endl;
    } else {
        cout << "ID, Name, Contact Details" << endl;
        for (const string& found_line : found_lines) {
            cout << found_line << endl;
        }
    }
}

void updatePatron(int search) {
    vector<Patron> patron_infos;
    ifstream file(patronPath);
    string line;
    while (file.peek() != EOF) {
        string id, name, contactDetails;
        getline(file, id, token);
        if (id == "ID") {
            file.ignore(INT_MAX, '\n');
            continue;
        }
        getline(file, name, token);
        getline(file, contactDetails);
        patron_infos.push_back({stoi(id), name, contactDetails});
    }

    for (int i = 0; i < patron_infos.size(); i++) {
        if (search == patron_infos[i].id) {
            cout << "Update Name: ";
            getline(cin >> ws, patron_infos[i].name);
            cout << "Update Contact Details: ";
            getline(cin >> ws, patron_infos[i].contactDetails);
            break;
        }
    }

    ofstream fileOut(patronPath);
    for (const auto& patron : patron_infos) {
        fileOut << patron.id << token
                << patron.name << token
                << patron.contactDetails << endl;
    }
    fileOut.close();
}

void deletePatron(int search) {
    vector<Patron> patron_infos;
    ifstream file(patronPath);
    string line;
    while (getline(file, line)) {
        string id, name, contactDetails;
        istrstream iss(line.c_str());
        getline(iss, id, token);
        if (id == "ID") {
            continue;
        }
        getline(iss, name, token);
        getline(iss, contactDetails);
        patron_infos.push_back({stoi(id), name, contactDetails});
    }

    for (size_t i = 0; i < patron_infos.size(); i++) {
        if (search == patron_infos[i].id) {
            patron_infos.erase(patron_infos.begin() + i);
            break;
        }
    }

    ofstream fileOut(patronPath);
    for (const auto& patron : patron_infos) {
        fileOut << patron.id << token
                << patron.name << token
                << patron.contactDetails << endl;
    }
    fileOut.close();
}

void displayPatrons() {
    ifstream file(patronPath);
    if (!file.is_open()) {
        cout << "Error opening file." << endl;
        return;
    }
    string line;
    while (getline(file, line)) {
        cout << line << endl;
    }
    file.close();
}

void checkOutBook() {
    vector<CheckOutRecord> checkOutRecords;
    int patronID, bookID;
    cout << "Enter Patron ID: ";
    cin >> patronID;
    cout << "Enter Book ID: ";
    cin >> bookID;

    bool patronFound = false, bookFound = false;
    for (const Patron& patron : patrons) {
        if (patron.id == patronID) {
            patronFound = true;
            break;
        }
    }
    for (const Book& book : books) {
        if (book.id == bookID) {
            bookFound = true;
            if (toLower(book.availability) == "available") {
                time_t now = time(0);
                time_t dueDate = now + (7 * 24 * 60 * 60);

                CheckOutRecord checkOutRecord = {patronID, bookID, now, dueDate, false};
                checkOutRecords.push_back(checkOutRecord);

                if (book.availability == "unavailable") {
                    cout << "Book successfully checked out. Due date: " << ctime(&dueDate) << endl;
                }
            } else {
                cout << "Book is currently unavailable for check-out." << endl;
            }
            break;
        }
    }

    if (!patronFound) {
        cout << "Patron not found. Please check the Patron ID." << endl;
    }

    if (!bookFound) {
        cout << "Book not found. Please check the Book ID." << endl;
    }
}

void returnBook() {
    vector<CheckOutRecord> checkOutRecords;
    int patronID, bookID;
    cout << "Enter Patron ID: ";
    cin >> patronID;
    cout << "Enter Book ID: ";
    cin >> bookID;

    bool checkOutFound = false;
    for (CheckOutRecord& checkOutRecord : checkOutRecords) {
        if (checkOutRecord.patronID == patronID && checkOutRecord.bookID == bookID && !checkOutRecord.returned) {
            checkOutFound = true;
            checkOutRecord.returned = true;

            for (Book& book : books) {
                if (book.id == bookID) {
                    book.availability = "available";
                    break;
                }
            }
            time_t now = time(0);
            double secondsLate = difftime(now, checkOutRecord.dueDate);
            if (secondsLate > 0) {
                double daysLate = secondsLate / (24 * 60 * 60);
                double fine = daysLate * 0.50;  // $0.50 per day late
                cout << "Book returned late. Fine: $" << fixed << setprecision(2) << fine << endl;
            } else {
                cout << "Book returned on time. No fine." << endl;
            }
            break;
        }
    }

    if (!checkOutFound) {
        cout << "Check-out record not found. Please check the Patron ID and Book ID." << endl;
    }
}

void displayCheckOuts() {
    vector<CheckOutRecord> checkOutRecords;
    cout << "Check-Out Records:" << endl;
    cout << "Patron ID   Book ID   Check-Out Date       Due Date             Status" << endl;
    for (const CheckOutRecord& checkOutRecord : checkOutRecords) {
        cout << setw(11) << checkOutRecord.patronID;
        cout << setw(10) << checkOutRecord.bookID;
        cout << setw(21) << ctime(&checkOutRecord.checkOutDate);
        cout << setw(21) << ctime(&checkOutRecord.dueDate);
        cout << setw(21) << (checkOutRecord.returned ? "Returned" : "Not Returned") << endl;
    }
}
void manageBook() {
    header();
    do {
        cout << "What do you want to do?" << endl;
        cout << "\t[1] Add a new book" << endl;
        cout << "\t[2] Search a book" << endl;
        cout << "\t[3] Update a book" << endl;
        cout << "\t[4] Delete a book" << endl;
        cout << "\t[5] Display all book" << endl;
        cout << "\t[6] Save and Exit" << endl;
        cout << "\t >> ";
        errValidation();
        switch (inputChoice) {
            case 1:
                clearScreen();
                addBook();
                break;
            case 2:
                clearScreen();
                cin.ignore();
                searchBook();
                break;
            case 3:
                clearScreen();
                int updateID;
                cout << "Updating the list using the ID of the book: ";
                cin >> updateID;
                updateBook(updateID);
                break;
            case 4:
                clearScreen();
                int deleteID;
                cout << "Deleting the list using the ID of the book: ";
                cin >> deleteID;
                deleteBook(deleteID);
                break;
            case 5:
                clearScreen();
                for (int i = 0; i < headers.size(); i++) {
                    cout << headers[i] << token;
                }
                cout << endl;
                displayBooks();
                break;
        }
    } while (inputChoice != 6);
    cout << "Saving and Exiting!!" << endl;
}
void managePatron() {
    header();
    do {
        cout << "What do you want to do?" << endl;
        cout << "\t[1] Add a new patron" << endl;
        cout << "\t[2] Search for a patron" << endl;
        cout << "\t[3] Update a patron" << endl;
        cout << "\t[4] Delete a patron" << endl;
        cout << "\t[5] Display all patrons" << endl;
        cout << "\t[6] Save and Exit" << endl;
        cout << "\t >> ";
        errValidation();
        switch (inputChoice) {
            case 1:
                clearScreen();
                addPatron();
                break;
            case 2:
                clearScreen();
                cin.ignore();
                searchPatron();
                break;
            case 3:
                clearScreen();
                int updatePatronID;
                cout << "Updating the patron using the ID: ";
                cin >> updatePatronID;
                updatePatron(updatePatronID);
                break;
            case 4:
                clearScreen();
                int deletePatronID;
                cout << "Deleting the patron using the ID: ";
                cin >> deletePatronID;
                deletePatron(deletePatronID);
                break;
            case 5:
                clearScreen();
                displayPatrons();
                break;
        }
    } while (inputChoice != 6);
    cout << "Saving and Exiting!!" << endl;
}
void manageCheckout() {
    clearScreen();
    do {
        cout << "What do you want to do with Check-outs and Returns?" << endl;
        cout << "\t[1] Check-out a book" << endl;
        cout << "\t[2] Return a book" << endl;
        cout << "\t[3] Display all check-outs" << endl;
        cout << "\t[4] Back to main menu" << endl;
        cout << "\t >> ";
        errValidation();
        switch (inputChoice) {
            case 1:
                clearScreen();
                checkOutBook();
                break;
            case 2:
                clearScreen();
                returnBook();
                break;
            case 3:
                clearScreen();
                displayCheckOuts();
                break;
        }
    } while (inputChoice != 4);
}
int main() {
    header();
    do {
        cout << "What do you want to do?" << endl;
        cout << "\t[1] Manage Books" << endl;
        cout << "\t[2] Manage Patrons" << endl;
        cout << "\t[3] Manage Patrons" << endl;
        cout << "\t[4] Save and Exit" << endl;
        cout << "\t >> ";
        errValidation();
        switch (inputChoice) {
            case 1:
                clearScreen();
                manageBook();
                break;
            case 2:
                clearScreen();
                managePatron();
                break;
            case 3:
                clearScreen();
                manageCheckout();
                break;
        }
    } while (inputChoice != 4);
    cout << "Saving and Exiting!!" << endl;
    return 0;
}
