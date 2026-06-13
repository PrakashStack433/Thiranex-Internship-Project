#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <limits>

using namespace std;

class Account {
private:
    int accountNumber;
    char name[50];
    double balance;

public:
    // Function to take user input for creating a new account
    void createAccount() {
        cout << "\nEnter Account Number: ";
        while (!(cin >> accountNumber)) {
            cout << "Invalid input. Please enter a numeric Account Number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore(); // Clear newline character from buffer
        
        cout << "Enter Account Holder Name: ";
        cin.getline(name, 50);
        
        cout << "Enter Initial Deposit Amount: $";
        while (!(cin >> balance) || balance < 0) {
            cout << "Invalid amount. Enter a valid positive deposit: $";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cout << "\nAccount Created Successfully!\n";
    }

    // Function to display account details
    void showAccount() const {
        cout << "\n------------------------------------";
        cout << "\nAccount Number : " << accountNumber;
        cout << "\nAccount Holder : " << name;
        cout << "\nCurrent Balance: $" << fixed << setprecision(2) << balance;
        cout << "\n------------------------------------\n";
    }

    // Function to handle deposits
    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "\n$" << amount << " deposited successfully.";
        } else {
            cout << "\nInvalid deposit amount.";
        }
    }

    // Function to handle withdrawals
    bool withdraw(double amount) {
        if (amount <= 0) {
            cout << "\nInvalid withdrawal amount.";
            return false;
        }
        if (amount > balance) {
            cout << "\nInsufficient balance! Transaction failed.";
            return false;
        }
        balance -= amount;
        cout << "\n$" << amount << " withdrawn successfully.";
        return true;
    }

    // Getter for account number
    int getAccountNumber() const {
        return accountNumber;
    }
};

// --- File Handling Helper Functions ---

// Function to write a new record to the file
void writeAccount() {
    Account acc;
    acc.createAccount();
    
    ofstream outFile("bank_data.dat", ios::binary | ios::app);
    if (!outFile) {
        cout << "Error opening file! Could not save data.\n";
        return;
    }
    outFile.write(reinterpret_cast<char*>(&acc), sizeof(Account));
    outFile.close();
}

// Function to display details of a specific account
void displayBalance(int accNum) {
    Account acc;
    ifstream inFile("bank_data.dat", ios::binary);
    if (!inFile) {
        cout << "Error opening file! No records found.\n";
        return;
    }
    
    bool found = false;
    while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
        if (acc.getAccountNumber() == accNum) {
            acc.showAccount();
            found = true;
            break;
        }
    }
    inFile.close();
    if (!found) {
        cout << "\nAccount number " << accNum << " does not exist.\n";
    }
}

// Function to update an account balance (for Deposits and Withdrawals)
void updateAccountBalance(int accNum, int actionType) {
    Account acc;
    fstream file("bank_data.dat", ios::binary | ios::in | ios::out);
    if (!file) {
        cout << "Error opening file! No records found.\n";
        return;
    }
    
    bool found = false;
    while (file.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
        if (acc.getAccountNumber() == accNum) {
            found = true;
            acc.showAccount();
            
            if (actionType == 1) { // Deposit
                double amount;
                cout << "Enter amount to deposit: $";
                cin >> amount;
                acc.deposit(amount);
            } 
            else if (actionType == 2) { // Withdraw
                double amount;
                cout << "Enter amount to withdraw: $";
                cin >> amount;
                acc.withdraw(amount);
            }
            
            // Move file pointer back to the start of this specific record
            int pos = -1 * static_cast<int>(sizeof(Account));
            file.seekp(pos, ios::cur);
            
            // Overwrite with updated data
            file.write(reinterpret_cast<char*>(&acc), sizeof(Account));
            cout << "\nRecord Updated.\n";
            break;
        }
    }
    file.close();
    if (!found) {
        cout << "\nAccount number " << accNum << " does not exist.\n";
    }
}

// --- Main Program Loop ---
int main() {
    int choice;
    int accNum;

    do {
        cout << "\n====================================";
        cout << "\n     BANK MANAGEMENT SYSTEM UI      ";
        cout << "\n====================================";
        cout << "\n1. Open New Account";
        cout << "\n2. Deposit Funds";
        cout << "\n3. Withdraw Funds";
        cout << "\n4. Balance Inquiry";
        cout << "\n5. Exit";
        cout << "\n====================================";
        cout << "\nSelect an option (1-5): ";
        
        if (!(cin >> choice)) {
            cout << "Invalid option. Please enter a number between 1 and 5.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1:
                writeAccount();
                break;
            case 2:
                cout << "\nEnter Account Number: ";
                cin >> accNum;
                updateAccountBalance(accNum, 1); // 1 for Deposit
                break;
            case 3:
                cout << "\nEnter Account Number: ";
                cin >> accNum;
                updateAccountBalance(accNum, 2); // 2 for Withdraw
                break;
            case 4:
                cout << "\nEnter Account Number: ";
                cin >> accNum;
                displayBalance(accNum);
                break;
            case 5:
                cout << "\nThank you for using our Banking System. Goodbye!\n";
                break;
            default:
                cout << "\nInvalid choice! Please try again.\n";
        }
    } while (choice != 5);

    return 0;
}