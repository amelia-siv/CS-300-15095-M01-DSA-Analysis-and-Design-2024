// ProjectTwo.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Amelia Sivick  
// December 15, 2024  
// Project Two  
// Professor Lebel  
// SNHU  

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>

using namespace std;

// Define Course structure
struct Course {
    string courseNumber;
    string title;
    vector<string> prerequisites;

    Course() = default;
    Course(string number, string name, vector<string> prereqs)
        : courseNumber(number), title(name), prerequisites(prereqs) {}
};

// Define TreeNode structure for the BST
struct TreeNode {
    Course course;
    TreeNode* left;
    TreeNode* right;

    TreeNode(Course course) : course(course), left(nullptr), right(nullptr) {}
};

// Define the Tree class
class Tree {
private:
    TreeNode* root;

    void insertNode(TreeNode*& node, Course course) {
        if (!node) {
            node = new TreeNode(course);
        }
        else if (course.courseNumber < node->course.courseNumber) {
            insertNode(node->left, course);
        }
        else {
            insertNode(node->right, course);
        }
    }

    TreeNode* searchNode(TreeNode* node, const string& courseNumber) const {
        if (!node || node->course.courseNumber == courseNumber) {
            return node;
        }
        if (courseNumber < node->course.courseNumber) {
            return searchNode(node->left, courseNumber);
        }
        return searchNode(node->right, courseNumber);
    }

    void inOrderTraversal(TreeNode* node, vector<Course>& courses) const {
        if (node) {
            inOrderTraversal(node->left, courses);
            courses.push_back(node->course);
            inOrderTraversal(node->right, courses);
        }
    }

    void deleteTree(TreeNode* node) {
        if (node) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

public:
    Tree() : root(nullptr) {}

    ~Tree() { deleteTree(root); }

    void insert(Course course) { insertNode(root, course); }

    Course* search(const string& courseNumber) const {
        TreeNode* node = searchNode(root, courseNumber);
        return node ? &node->course : nullptr;
    }

    vector<Course> inOrderTraversal() const {
        vector<Course> courses;
        inOrderTraversal(root, courses);
        return courses;
    }
};

// Function to clear the input buffer
void clearInputBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Function to load courses from a file
vector<Course> loadFile(const string& filename) {
    vector<Course> courses;
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error: Unable to open file.\n";
        return courses;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string courseNumber, title, prereq;
        vector<string> prerequisites;

        getline(ss, courseNumber, ',');
        getline(ss, title, ',');

        while (getline(ss, prereq, ',')) {
            prerequisites.push_back(prereq);
        }

        courses.emplace_back(courseNumber, title, prerequisites);
    }

    file.close();
    return courses;
}

// Function to print a course's details
void printCourse(const Course& course) {
    cout << "Course Number: " << course.courseNumber << "\n";
    cout << "Title: " << course.title << "\n";
    if (course.prerequisites.empty()) {
        cout << "Prerequisites: None\n";
    }
    else {
        cout << "Prerequisites: ";
        for (size_t i = 0; i < course.prerequisites.size(); ++i) {
            cout << course.prerequisites[i];
            if (i < course.prerequisites.size() - 1) cout << ", ";
        }
        cout << "\n";
    }
}

// Function to print all courses in alphanumeric order
void printAllCourses(const Tree& coursesTree) {
    vector<Course> courses = coursesTree.inOrderTraversal();
    for (const auto& course : courses) {
        cout << course.courseNumber << ": " << course.title << "\n";
    }
}

// Main menu function
void mainMenu() {
    Tree coursesTree;

    while (true) {
        cout << "\n1. Load courses from file\n";
        cout << "2. Print all courses in order\n";
        cout << "3. Print course details\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        switch (choice) {
        case 1: {
            clearInputBuffer();  // Ensure input stream is clean
            cout << "Enter filename: (e.g courses.csv)";
            string filename;
            getline(cin, filename);

            vector<Course> courses = loadFile(filename);
            for (const auto& course : courses) {
                coursesTree.insert(course);
            }

            if (!courses.empty()) {
                cout << "Courses loaded successfully.\n";
            }
            break;
        }
        case 2: {
            if (coursesTree.inOrderTraversal().empty()) {
                cout << "No courses loaded. Please load a file first.\n";
            }
            else {
                printAllCourses(coursesTree);
            }
            break;
        }
        case 3: {
            clearInputBuffer();  // Clean the input buffer
            cout << "Enter course number: ";
            string courseNumber;
            getline(cin, courseNumber);

            Course* course = coursesTree.search(courseNumber);
            if (course) {
                printCourse(*course);
            }
            else {
                cout << "Course not found.\n";
            }
            break;
        }
        case 9: {
            cout << "Exiting program. Goodbye!\n";
            return;
        }
        default: {
            cout << "Invalid choice. Please try again.\n";
            break;
        }
        }
    }
}

// Program entry point
int main() {
    cout << "Welcome to the ABCU Course Finder\n";
    mainMenu();
    return 0;
}




