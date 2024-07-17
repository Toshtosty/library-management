#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

const int MAX_BOOKS = 100;
const int MAX_MEMBERS = 50;
const int MAX_LOANS = 200;

class Book {
private:
    int bookID;
    string title;
    string author;
    bool available;

public:
    Book() : bookID(0), title(""), author(""), available(true) {}

    Book(int id, const string& t, const string& a) : bookID(id), title(t), author(a), available(true) {}

    int getBookID() const { return bookID; }
    const string& getTitle() const { return title; }
    const string& getAuthor() const { return author; }
    bool isAvailable() const { return available; }

    void setAvailable(bool status) { available = status; }
};

class Member {
private:
    int memberID;
    string name;
    vector<int> booksBorrowed;

public:
    Member() : memberID(0), name("") {}

    Member(int id, const string& n) : memberID(id), name(n) {}

    int getMemberID() const { return memberID; }
    const string& getName() const { return name; }
    const vector<int>& getBooksBorrowed() const { return booksBorrowed; }

    void borrowBook(int bookID) {
        booksBorrowed.push_back(bookID);
    }

    void returnBook(int bookID) {
        auto it = find(booksBorrowed.begin(), booksBorrowed.end(), bookID);
        if (it != booksBorrowed.end()) {
            booksBorrowed.erase(it);
        }
    }
};

class Loan {
private:
    int loanID;
    int bookID;
    int memberID;
    string issueDate;
    string dueDate;

public:
    Loan() : loanID(0), bookID(0), memberID(0), issueDate(""), dueDate("") {}

    Loan(int lid, int bid, int mid, const string& issue, const string& due)
        : loanID(lid), bookID(bid), memberID(mid), issueDate(issue), dueDate(due) {}

    int getLoanID() const { return loanID; }
    int getBookID() const { return bookID; }
    int getMemberID() const { return memberID; }
    const string& getIssueDate() const { return issueDate; }
    const string& getDueDate() const { return dueDate; }
};


class Library {
private:
    Book books[MAX_BOOKS];
    Member members[MAX_MEMBERS];
    Loan loans[MAX_LOANS];
    int numBooks;
    int numMembers;
    int numLoans;

public:
    Library() : numBooks(0), numMembers(0), numLoans(0) {}

    void addBook(int id, const string& title, const string& author) {
        if (numBooks < MAX_BOOKS) {
            books[numBooks++] = Book(id, title, author);
        }
    }

    void removeBook(int bookID) {
        for (int i = 0; i < numBooks; ++i) {
            if (books[i].getBookID() == bookID) {
                for (int j = i; j < numBooks - 1; ++j) {
                    books[j] = books[j + 1];
                }
                numBooks--;
                return;
            }
        }
    }

    void addMember(int id, const string& name) {
        if (numMembers < MAX_MEMBERS) {
            members[numMembers++] = Member(id, name);
        }
    }

    void removeMember(int memberID) {
        for (int i = 0; i < numMembers; ++i) {
            if (members[i].getMemberID() == memberID) {
                for (int j = i; j < numMembers - 1; ++j) {
                    members[j] = members[j + 1];
                }
                numMembers--;
                return;
            }
        }
    }

    void issueBook(int bookID, int memberID, const string& issueDate, const string& dueDate) {
        for (int i = 0; i < numBooks; ++i) {
            if (books[i].getBookID() == bookID && books[i].isAvailable()) {
                books[i].setAvailable(false);
                loans[numLoans++] = Loan(numLoans + 1, bookID, memberID, issueDate, dueDate);
                for (int j = 0; j < numMembers; ++j) {
                    if (members[j].getMemberID() == memberID) {
                        members[j].borrowBook(bookID);
                        cout << "Book issued successfully." << endl;
                        return;
                    }
                }
            }
        }
        cout << "Issue failed. Book might already be loaned or member ID is invalid." << endl;
    }

    void returnBook(int bookID, int memberID) {
        for (int i = 0; i < numLoans; ++i) {
            if (loans[i].getBookID() == bookID && loans[i].getMemberID() == memberID) {
                for (int j = 0; j < numBooks; ++j) {
                    if (books[j].getBookID() == bookID) {
                        books[j].setAvailable(true);
                        for (int k = 0; k < numMembers; ++k) {
                            if (members[k].getMemberID() == memberID) {
                                members[k].returnBook(bookID);
                                loans[i] = loans[--numLoans]; // Remove loan by shifting the last element to its place
                                cout << "Book returned successfully." << endl;
                                return;
                            }
                        }
                    }
                }
            }
        }
        cout << "Return failed. Book might not be loaned or invalid book ID." << endl;
    }

    int getNumBooks() const { return numBooks; }
    int getNumMembers() const { return numMembers; }
    const Book& getBook(int index) const { return books[index]; }
    const Member& getMember(int index) const { return members[index]; }
};



int main() {
    Library library;

    library.addBook(1, "The Catcher in the Rye", "J.D. Salinger");
    library.addBook(2, "To Kill a Mockingbird", "Harper Lee");
    library.addBook(3, "1984", "George Orwell");

    library.addMember(101, "Alice Smith");
    library.addMember(102, "Bob Johnson");

    library.issueBook(1, 101, "2022-07-18", "2023-08-18");
    library.issueBook(2, 102, "2023-07-18", "2023-08-18");

    cout << "Books in the library:" << endl;
    for (int i = 0; i < library.getNumBooks(); ++i) {
        const Book& book = library.getBook(i);
        cout << book.getBookID() << ". " << book.getTitle() << " by " << book.getAuthor() << " (Available: " << (book.isAvailable() ? "Yes" : "No") << ")" << endl;
    }

    cout << "\nMembers in the library:" << endl;
    for (int i = 0; i < library.getNumMembers(); ++i) {
        const Member& member = library.getMember(i);
        cout << member.getMemberID() << ". " << member.getName() << endl;
    }

    library.returnBook(2, 102);

    cout << "\nUpdated availability:" << endl;
    for (int i = 0; i < library.getNumBooks(); ++i) {
        const Book& book = library.getBook(i);
        cout << book.getBookID() << ". " << book.getTitle() << " by " << book.getAuthor() << " (Available: " << (book.isAvailable() ? "Yes" : "No") << ")" << endl;
    }

    return 0;
}

