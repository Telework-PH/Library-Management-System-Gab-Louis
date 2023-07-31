#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <ctime>

using namespace std;

int inputChoice;
string bookPath = "book.csv";
string patronPath = "patron.csv";
string checkOutPath = "checkout.csv";
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

void loadBooksAndPatrons() {
    ifstream bookFile(bookPath);
    if (!bookFile.is_open()) {
        cout << "Error opening book file." << endl;
        return;
    }

    string line;
    while (getline(bookFile, line)) {
        string id, title, author, publisher, availability;
        stringstream iss(line); // Use stringstream instead of strstream
        getline(iss, id, token);
        if (id == "ID") {
            continue;
        }
        getline(iss, title, token);
        getline(iss, author, token);
        getline(iss, publisher, token);
        getline(iss, availability);
        books.push_back({stoi(id), title, author, publisher, availability});
    }
    bookFile.close();

    ifstream patronFile(patronPath);
    if (!patronFile.is_open()) {
        cout << "Error opening patron file." << endl;
        return;
    }

    while (getline(patronFile, line)) {
        string id, name, contactDetails;
        stringstream iss(line); // Use stringstream instead of strstream
        getline(iss, id, token);
        if (id == "ID") {
            continue;
        }
        getline(iss, name, token);
        getline(iss, contactDetails);
        patrons.push_back({stoi(id), name, contactDetails});
    }
    patronFile.close();
}

void saveBooksAndPatrons() {
    ofstream bookFile(bookPath);
    if (!bookFile.is_open()) {
        cout << "Error opening book file for saving." << endl;
        return;
    }

    bookFile << "ID,TITLE,AUTHOR,PUBLISHER,STATUS" << endl;
    for (const auto& book : books) {
        bookFile << book.id << token << book.title << token << book.author << token
                 << book.publisher << token << book.availability << endl;
    }
    bookFile.close();

    ofstream patronFile(patronPath);
    if (!patronFile.is_open()) {
        cout << "Error opening patron file for saving." << endl;
        return;
    }

    patronFile << "ID,NAME,CONTACT DETAILS" << endl;
    for (const auto& patron : patrons) {
        patronFile << patron.id << token << patron.name << token << patron.contactDetails << endl;
    }
    patronFile.close();
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
        stringstream iss(line.c_str());
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
    book.availability = toLower(book.availability); // Store the lowercase status
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
    }
    file.close();
}

void deleteBook(int search) {
    vector<Book> book_infos;
    ifstream book(bookPath);
    string line;
    while (getline(book, line)) {
        string id, title, author, publisher, availability;
        stringstream iss(line.c_str());
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
    }
    file.close();
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
        stringstream iss(line.c_str());
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
        stringstream iss(line.c_str());
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
        cout << line << "\n";
    }
    file.close();
}

void loadCheckOutRecords() {
    ifstream file(checkOutPath);
    if (!file.is_open()) {
        cout << "No previous check-out records found." << endl;
        return;
    }
    checkOutRecords.clear();
    string line;
    while (getline(file, line)) {
        string patronID, bookID, checkOutDate, dueDate, returned;
        stringstream iss(line);
        getline(iss, patronID, token);
        getline(iss, bookID, token);
        getline(iss, checkOutDate, token);
        getline(iss, dueDate, token);
        getline(iss, returned);

        CheckOutRecord record;
        record.patronID = stoi(patronID);
        record.bookID = stoi(bookID);
        record.checkOutDate = stoi(checkOutDate);
        record.dueDate = stoi(dueDate);
        record.returned = (stoi(returned) == 1);

        checkOutRecords.push_back(record);
    }
    file.close();
}

void saveCheckOutRecords() {
    ofstream file(checkOutPath);
    if (!file.is_open()) {
        cout << "Error saving check-out records." << endl;
        return;
    }
    for (const CheckOutRecord& record : checkOutRecords) {
        file << record.patronID << token
             << record.bookID << token
             << record.checkOutDate << token
             << record.dueDate << token
             << (record.returned ? "1" : "0") << '\n';
    }
    file.close();
}

void checkOutBook() {
    int patronID, bookID;
    time_t checkOutDate = time(nullptr);
    cout << "Enter Patron ID: ";
    cin >> patronID;

    // Check if the patron ID exists
    bool patronExists = false;
    for (const auto& patron : patrons) {
        if (patron.id == patronID) {
            patronExists = true;
            break;
        }
    }

    if (!patronExists) {
        cout << "Patron with ID " << patronID << " does not exist." << endl;
        return;
    }

    cout << "Enter Book ID: ";
    cin >> bookID;

    // Check if the book ID exists
    bool bookExists = false;
    for (const auto& book : books) {
        if (book.id == bookID) {
            bookExists = true;
            break;
        }
    }

    if (!bookExists) {
        cout << "Book with ID " << bookID << " does not exist." << endl;
        return;
    }

    // Check if the book is available for check-out
    bool bookAvailable = false;
    for (const auto& book : books) {
        if (book.id == bookID && toLower(book.availability) == "available") {
            bookAvailable = true;
            break;
        }
    }

    if (!bookAvailable) {
        cout << "Book with ID " << bookID << " is not available for check-out." << endl;
        return;
    }

    // Calculate the due date (7 days from the check-out date)
    time_t dueDate = checkOutDate + (7 * 24 * 60 * 60); // 7 days in seconds

    // Add the check-out record to the checkOutRecords vector
    checkOutRecords.push_back({patronID, bookID, checkOutDate, dueDate, false});

    // Update the book availability status
    for (auto& book : books) {
        if (book.id == bookID) {
            book.availability = "unavailable";
            break;
        }
    }

    saveCheckOutRecords();
    saveBooksAndPatrons();

    cout << "Book with ID " << bookID << " has been checked out by Patron with ID " << patronID << "." << endl;
}

void returnBook() {
    int patronID, bookID;
    time_t returnDate = time(nullptr);
    cout << "Enter Patron ID: ";
    cin >> patronID;

    // Check if the patron ID exists
    bool patronExists = false;
    for (const auto& patron : patrons) {
        if (patron.id == patronID) {
            patronExists = true;
            break;
        }
    }

    if (!patronExists) {
        cout << "Patron with ID " << patronID << " does not exist." << endl;
        return;
    }

    cout << "Enter Book ID: ";
    cin >> bookID;

    // Check if the book ID exists
    bool bookExists = false;
    for (const auto& book : books) {
        if (book.id == bookID) {
            bookExists = true;
            break;
        }
    }

    if (!bookExists) {
        cout << "Book with ID " << bookID << " does not exist." << endl;
        return;
    }

    // Find the check-out record and update its return status
    bool bookFound = false;
    for (auto& record : checkOutRecords) {
        if (record.patronID == patronID && record.bookID == bookID && !record.returned) {
            record.returned = true;
            bookFound = true;
            break;
        }
    }

    if (!bookFound) {
        cout << "No active check-out record found for Patron with ID " << patronID << " and Book with ID " << bookID << "." << endl;
        return;
    }

    // Update the book availability status
    for (auto& book : books) {
        if (book.id == bookID) {
            book.availability = "available";
            break;
        }
    }

    saveCheckOutRecords();
    saveBooksAndPatrons();

    cout << "Book with ID " << bookID << " has been returned by Patron with ID " << patronID << "." << endl;
}
void displayCheckOuts() {
    cout << "Check-Out Records:" << endl;
    cout << "Patron ID   Book ID   Check-Out Date       Due Date             Status" << endl;
    for (const CheckOutRecord& record : checkOutRecords) {
        cout << setw(11) << record.patronID;
        cout << setw(10) << record.bookID;
        cout << setw(21) << ctime(&record.checkOutDate);
        cout << setw(21) << ctime(&record.dueDate);
        cout << setw(21) << (record.returned ? "Returned" : "Not Returned") << endl;
    }
}
void manageBook(){
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
                for(int i = 0; i < headers.size(); i++){
                    cout << headers[i] << token;
                }cout << endl;
                displayBooks();
                break;
        }
    }while(inputChoice != 6);
    cout << "Saving and Exiting!!" << endl;
}
void managePatron(){
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
void manageCheckout(){
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
    loadBooksAndPatrons();
    loadCheckOutRecords();
    header();
    while (true) {
        clearScreen();
        cout << "MAIN MENU" << endl;
        cout << "1. Manage Books" << endl;
        cout << "2. Manage Patrons" << endl;
        cout << "3. Manage Check Out/Return" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";
        errValidation();
        switch (inputChoice) {
            case 1:
                manageBook();
                break;
            case 2:
                managePatron();
                break;
            case 3:
                manageCheckout();
                break;
            case 4:
                saveBooksAndPatrons();
                saveCheckOutRecords();
                cout << "Thank you for using the Library Management System. Goodbye!" << endl;
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
    }

    return 0;
}
