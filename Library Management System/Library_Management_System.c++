#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

class Book {
private:
    int id;
    std::string title;
    std::string author;
    bool isIssued;

public:
    Book(int bookId, std::string bookTitle, std::string bookAuthor)
        : id(bookId), title(bookTitle), author(bookAuthor), isIssued(false) {}

    int getId() const { return id; }
    std::string getTitle() const { return title; }
    std::string getAuthor() const { return author; }
    bool getIssuedStatus() const { return isIssued; }

    void issueBook() { isIssued = true; }
    void returnBook() { isIssued = false; }

    void display() const {
        std::cout << std::left << std::setw(10) << id 
                  << std::setw(25) << title 
                  << std::setw(20) << author 
                  << (isIssued ? "Issued" : "Available") << "\n";
    }
};

class Member {
private:
    int id;
    std::string name;

public:
    Member(int memberId, std::string memberName) : id(memberId), name(memberName) {}

    int getId() const { return id; }
    std::string getName() const { return name; }

    void display() const {
        std::cout << std::left << std::setw(15) << id << name << "\n";
    }
};

class Library {
private:
    std::vector<Book> books;
    std::vector<Member> members;

    // Helper method to find a book by ID
    Book* findBookById(int bookId) {
        for (auto& book : books) {
            if (book.getId() == bookId) return &book;
        }
        return nullptr;
    }

    // Helper method to find a member by ID
    Member* findMemberById(int memberId) {
        for (auto& member : members) {
            if (member.getId() == memberId) return &member;
        }
        return nullptr;
    }

public:
    void addBook() {
        int id;
        std::string title, author;
        std::cout << "Enter Book ID: ";
        std::cin >> id;
        if (findBookById(id) != nullptr) {
            std::cout << "Error: Book ID already exists!\n";
            return;
        }
        std::cin.ignore(); // Clear buffer
        std::cout << "Enter Book Title: ";
        std::getline(std::cin, title);
        std::cout << "Enter Author Name: ";
        std::getline(std::cin, author);

        books.emplace_back(id, title, author);
        std::cout << "Book added successfully!\n";
    }

    void addMember() {
        int id;
        std::string name;
        std::cout << "Enter Member ID: ";
        std::cin >> id;
        if (findMemberById(id) != nullptr) {
            std::cout << "Error: Member ID already exists!\n";
            return;
        }
        std::cin.ignore();
        std::cout << "Enter Member Name: ";
        std::getline(std::cin, name);

        members.emplace_back(id, name);
        std::cout << "Member registered successfully!\n";
    }

    void issueBook() {
        int bookId, memberId;
        std::cout << "Enter Book ID to issue: ";
        std::cin >> bookId;
        Book* book = findBookById(bookId);

        if (!book) {
            std::cout << "Book not found.\n";
            return;
        }
        if (book->getIssuedStatus()) {
            std::cout << "Book is already issued to someone else.\n";
            return;
        }

        std::cout << "Enter Member ID: ";
        std::cin >> memberId;
        Member* member = findMemberById(memberId);

        if (!member) {
            std::cout << "Member not registered.\n";
            return;
        }

        book->issueBook();
        std::cout << "Book '" << book->getTitle() << "' successfully issued to " << member->getName() << ".\n";
    }

    void returnBook() {
        int bookId;
        std::cout << "Enter Book ID to return: ";
        std::cin >> bookId;
        Book* book = findBookById(bookId);

        if (!book) {
            std::cout << "Book not found.\n";
            return;
        }
        if (!book->getIssuedStatus()) {
            std::cout << "This book is already in the library inventory.\n";
            return;
        }

        book->returnBook();
        std::cout << "Book '" << book->getTitle() << "' returned successfully.\n";
    }

    void searchBooks() const {
        std::cin.ignore();
        std::string query;
        std::cout << "Enter search keyword (Title or Author): ";
        std::getline(std::cin, query);

        // Convert query to lowercase for case-insensitive matching
        std::transform(query.begin(), query.end(), query.begin(), ::tolower);

        bool found = false;
        std::cout << "\n------------------- Search Results -------------------\n";
        std::cout << std::left << std::setw(10) << "Book ID" << std::setw(25) << "Title" << std::setw(20) << "Author" << "Status\n";
        std::cout << "------------------------------------------------------\n";

        for (const auto& book : books) {
            std::string titleLower = book.getTitle();
            std::string authorLower = book.getAuthor();
            std::transform(titleLower.begin(), titleLower.end(), titleLower.begin(), ::tolower);
            std::transform(authorLower.begin(), authorLower.end(), authorLower.begin(), ::tolower);

            if (titleLower.find(query) != std::string::npos || authorLower.find(query) != std::string::npos) {
                book.display();
                found = true;
            }
        }
        if (!found) std::cout << "No matching books found.\n";
    }

    void displayAllBooks() const {
        if (books.empty()) {
            std::cout << "Library inventory is empty.\n";
            return;
        }
        std::cout << "\n------------------- Library Inventory -------------------\n";
        std::cout << std::left << std::setw(10) << "Book ID" << std::setw(25) << "Title" << std::setw(20) << "Author" << "Status\n";
        std::cout << "---------------------------------------------------------\n";
        for (const auto& book : books) book.display();
    }
};

int main() {
    Library lib;
    int choice;

    while (true) {
        std::cout << "\n================ LIBRARY MANAGEMENT SYSTEM ================\n";
        std::cout << "1. Add Book\n";
        std::cout << "2. Add Member\n";
        std::cout << "3. Issue Book\n";
        std::cout << "4. Return Book\n";
        std::cout << "5. Search Book (by Title/Author)\n";
        std::cout << "6. Display All Books\n";
        std::cout << "7. Exit\n";
        std::cout << "===========================================================\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear(); 
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1: lib.addBook(); break;
            case 2: lib.addMember(); break;
            case 3: lib.issueBook(); break;
            case 4: lib.returnBook(); break;
            case 5: lib.searchBooks(); break;
            case 6: lib.displayAllBooks(); break;
            case 7: std::cout << "Exiting system. Goodbye!\n"; return 0;
            default: std::cout << "Invalid choice! Please select between 1 and 7.\n";
        }
    }
}