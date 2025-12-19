/*
Author: Haseeb Rehman
Date: December 13, 2025
Course: CS 300 - Data Structures and Algorithms

Summary:
This program reads course data from a CSV file.
It lets users print all courses in sorted order and
look up a single course with its prerequisites.
*/

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

struct Course {
    string number;                 // Example: CSCI200
    string title;                  // Example: Data Structures
    vector<string> prerequisites;  // Example: CSCI101, MATH201
};

string trim(const string& s) {
    size_t start = 0;
    while (start < s.size() && isspace(static_cast<unsigned char>(s[start]))) {
        start++;
    }
    size_t end = s.size();
    while (end > start && isspace(static_cast<unsigned char>(s[end - 1]))) {
        end--;
    }
    return s.substr(start, end - start);
}

string toUpper(string s) {
    for (char& c : s) {
        c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
    }
    return s;
}

vector<string> splitCSV(const string& line) {
    vector<string> fields;
    string field;
    stringstream ss(line);

    while (getline(ss, field, ',')) {
        fields.push_back(trim(field));
    }
    return fields;
}

bool loadCoursesFromFile(const string& filename,
                         unordered_map<string, Course>& courses) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    courses.clear();
    string line;

    while (getline(file, line)) {
        if (trim(line).empty()) {
            continue;
        }

        vector<string> fields = splitCSV(line);
        if (fields.size() < 2) {
            continue;
        }

        Course course;
        course.number = toUpper(fields[0]);
        course.title = fields[1];

        for (size_t i = 2; i < fields.size(); i++) {
            string prereq = toUpper(fields[i]);
            if (!prereq.empty()) {
                course.prerequisites.push_back(prereq);
            }
        }

        courses[course.number] = course;
    }

    return true;
}

vector<string> getSortedCourseNumbers(const unordered_map<string, Course>& courses) {
    vector<string> keys;
    for (const auto& pair : courses) {
        keys.push_back(pair.first);
    }
    sort(keys.begin(), keys.end());
    return keys;
}

void printCourseList(const unordered_map<string, Course>& courses) {
    vector<string> courseNumbers = getSortedCourseNumbers(courses);

    cout << "Here is a sample schedule:" << endl;
    for (const string& number : courseNumbers) {
        cout << number << ", " << courses.at(number).title << endl;
    }
}

void printCourseInformationSampleStyle(const unordered_map<string, Course>& courses,
                                       const string& courseNumberInput) {
    string courseNumber = toUpper(trim(courseNumberInput));
    auto it = courses.find(courseNumber);

    if (it == courses.end()) {
        cout << "Course not found." << endl;
        return;
    }

    const Course& course = it->second;
    cout << course.number << ", " << course.title << endl;

    // Sample style: prerequisites on ONE line, comma-separated.
    if (course.prerequisites.empty()) {
        cout << "Prerequisites: None" << endl;
        return;
    }

    cout << "Prerequisites: ";
    for (size_t i = 0; i < course.prerequisites.size(); i++) {
        cout << course.prerequisites[i];
        if (i + 1 < course.prerequisites.size()) {
            cout << ", ";
        }
    }
    cout << endl;
}

void pickAndPrintCourse(const unordered_map<string, Course>& courses) {
    vector<string> courseNumbers = getSortedCourseNumbers(courses);

    cout << "Select a course from the list:" << endl;
    for (size_t i = 0; i < courseNumbers.size(); i++) {
        cout << (i + 1) << ") " << courseNumbers[i] << endl;
    }

    cout << "Enter selection (1-" << courseNumbers.size() << ", or 0 to cancel): ";
    string input;
    getline(cin, input);

    int selection;
    try {
        selection = stoi(trim(input));
    } catch (...) {
        cout << "Invalid input." << endl;
        return;
    }

    if (selection == 0) {
        return;
    }

    if (selection < 1 || selection > static_cast<int>(courseNumbers.size())) {
        cout << "Invalid selection." << endl;
        return;
    }

    printCourseInformationSampleStyle(courses, courseNumbers[selection - 1]);
}

void displayMenu() {
    cout << "1. Load Data Structure." << endl;
    cout << "2. Print Course List." << endl;
    cout << "3. Print Course." << endl;
    cout << "9. Exit" << endl;
}

int main() {
    unordered_map<string, Course> courses;
    bool dataLoaded = false;

    const string DEFAULT_FILE = "CS 300 ABCU_Advising_Program_Input.csv";

    cout << "Welcome to the course planner." << endl;

    while (true) {
        displayMenu();
        cout << "What would you like to do? ";

        string input;
        getline(cin, input);

        int choice;
        try {
            choice = stoi(trim(input));
        } catch (...) {
            cout << "Invalid input." << endl;
            continue;
        }

        if (choice == 1) {
            // Rubric requires prompting for the filename
            cout << "Enter the file name: ";
            string filename;
            getline(cin, filename);

            if (trim(filename).empty()) {
                filename = DEFAULT_FILE; // Default if user just presses Enter
            }

            if (loadCoursesFromFile(filename, courses)) {
                dataLoaded = true;
            } else {
                cout << "Error: Could not open file." << endl;
            }
        }
        else if (choice == 2) {
            if (!dataLoaded) {
                cout << "Please load data first." << endl;
            } else {
                printCourseList(courses);
            }
        }
        else if (choice == 3) {
            if (!dataLoaded) {
                cout << "Please load data first." << endl;
            } else {
                // Sample prompt text
                cout << "What course do you want to know about? ";
                string courseNumber;
                getline(cin, courseNumber);

                // Optional: if user presses Enter, let them choose from list
                if (trim(courseNumber).empty()) {
                    pickAndPrintCourse(courses);
                } else {
                    printCourseInformationSampleStyle(courses, courseNumber);
                }
            }
        }
        else if (choice == 9) {
            cout << "Thank you for using the course planner!" << endl;
            break;
        }
        else {
            // Sample text for invalid option
            cout << choice << " is not a valid option." << endl;
        }
    }

    return 0;
}
