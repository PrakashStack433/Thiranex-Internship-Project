#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>

using namespace std;

struct Student {
    int id;
    string name;
    int age;
    string course;
};

void addStudent();
void displayAllStudents();
void updateStudent();
void deleteStudent();
void drawMenu();

const string FILE_NAME = "students.txt";

int main() {
    int choice;

    while (true) {
        drawMenu();
        cout << "Enter your choice (1-5): ";
        
        if (!(cin >> choice)) {
            cout << "\n[!] Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                displayAllStudents();
                break;
            case 3:
                updateStudent();
                break;
            case 4:
                deleteStudent();
                break;
            case 5:
                cout << "\nThank you for using the Student Management System. Goodbye!\n";
                return 0;
            default:
                cout << "\n[!] Invalid choice! Please select an option between 1 and 5.\n";
        }
    }
    return 0;
}

void drawMenu() {
    cout << "\n====================================" << endl;
    cout << "     STUDENT MANAGEMENT SYSTEM      " << endl;
    cout << "====================================" << endl;
    cout << "1. Add New Student" << endl;
    cout << "2. Display All Students" << endl;
    cout << "3. Update Student Record" << endl;
    cout << "4. Delete Student Record" << endl;
    cout << "5. Exit Application" << endl;
    cout << "------------------------------------" << endl;
}

bool idExists(int id) {
    ifstream file(FILE_NAME);
    if (!file) return false;

    int curId, curAge;
    string curName, curCourse;
    while (file >> curId >> ws && getline(file, curName, ',') && file >> curAge >> ws && getline(file, curCourse)) {
        if (curId == id) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

// 1. ADD STUDENT
void addStudent() {
    ofstream file(FILE_NAME, ios::app); 
    if (!file) {
        cout << "\n[!] Error opening file for writing.\n";
        return;
    }

    Student s;
    cout << "\n--- Add New Student ---" << endl;
    
    cout << "Enter Student ID (Integer): ";
    while (!(cin >> s.id) || s.id <= 0) {
        cout << "[!] Invalid ID. Please enter a positive integer: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }

    if (idExists(s.id)) {
        cout << "\n[!] Error: Student with ID " << s.id << " already exists!\n";
        file.close();
        return;
    }

    cin.ignore(); 
    cout << "Enter Student Name: ";
    getline(cin, s.name);

    cout << "Enter Student Age: ";
    while (!(cin >> s.age) || s.age <= 0) {
        cout << "[!] Invalid Age. Please enter a valid age: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }

    cin.ignore();
    cout << "Enter Course/Major: ";
    getline(cin, s.course);

    file << s.id << " " << s.name << "," << s.age << " " << s.course << "\n";
    file.close();

    cout << "\n[+] Student record added successfully!\n";
}

void displayAllStudents() {
    ifstream file(FILE_NAME);
    if (!file) {
        cout << "\n[!] No records found. Try adding a student first.\n";
        return;
    }

    int id, age;
    string name, course;
    bool hasRecords = false;

    cout << "\n-------------------------------------------------------------\n";
    cout << left << setw(10) << "ID" << setw(25) << "Name" << setw(8) << "Age" << setw(20) << "Course" << endl;
    cout << "-------------------------------------------------------------\n";

    // Read until end of file
    while (file >> id >> ws && getline(file, name, ',') && file >> age >> ws && getline(file, course)) {
        cout << left << setw(10) << id << setw(25) << name << setw(8) << age << setw(20) << course << endl;
        hasRecords = true;
    }
    file.close();

    if (!hasRecords) {
        cout << "   No records available to display.\n";
    }
    cout << "-------------------------------------------------------------\n";
}

void updateStudent() {
    ifstream file(FILE_NAME);
    if (!file) {
        cout << "\n[!] No records found.\n";
        return;
    }

    int targetId;
    cout << "\nEnter Student ID to Update: ";
    cin >> targetId;

    if (!idExists(targetId)) {
        cout << "\n[!] Student record with ID " << targetId << " not found.\n";
        file.close();
        return;
    }

    vector<Student> students;
    int id, age;
    string name, course;

    while (file >> id >> ws && getline(file, name, ',') && file >> age >> ws && getline(file, course)) {
        if (id == targetId) {
            Student s;
            s.id = id;
            cout << "\n--- Student Found! Enter New Details --- \n";
            cin.ignore();
            cout << "Enter New Name: ";
            getline(cin, s.name);
            cout << "Enter New Age: ";
            cin >> s.age;
            cin.ignore();
            cout << "Enter New Course: ";
            getline(cin, s.course);
            students.push_back(s);
        } else {
            students.push_back({id, name, age, course});
        }
    }
    file.close();

    ofstream outFile(FILE_NAME, ios::trunc);
    for (const auto& s : students) {
        outFile << s.id << " " << s.name << "," << s.age << " " << s.course << "\n";
    }
    outFile.close();

    cout << "\n[+] Student record updated successfully!\n";
}

void deleteStudent() {
    ifstream file(FILE_NAME);
    if (!file) {
        cout << "\n[!] No records found.\n";
        return;
    }

    int targetId;
    cout << "\nEnter Student ID to Delete: ";
    cin >> targetId;

    if (!idExists(targetId)) {
        cout << "\n[!] Student record with ID " << targetId << " not found.\n";
        file.close();
        return;
    }

    vector<Student> students;
    int id, age;
    string name, course;
    bool found = false;

    while (file >> id >> ws && getline(file, name, ',') && file >> age >> ws && getline(file, course)) {
        if (id == targetId) {
            found = true; // Skip this record
        } else {
            students.push_back({id, name, age, course});
        }
    }
    file.close();

    // Rewrite data back into the file
    ofstream outFile(FILE_NAME, ios::trunc);
    for (const auto& s : students) {
        outFile << s.id << " " << s.name << "," << s.age << " " << s.course << "\n";
    }
    outFile.close();

    cout << "\n[-] Student record deleted successfully!\n";
}