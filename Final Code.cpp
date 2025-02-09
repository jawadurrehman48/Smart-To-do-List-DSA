#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <windows.h>

using namespace std;


struct Task {
    int id;
    string name;
    string description;
    int priority; // 1 (high), 2 (medium), 3 (low)
    int dueDate; // in (YYMMDD)
    string progress; // "in progress" or "completed"
    Task* prev;
    Task* next;

    Task(int id, string name, string description, int priority, int dueDate) {
        this->id = id;
        this->name = name;
        this->description = description;
        this->priority = priority;
        this->dueDate = dueDate;
        this->progress = "in progress"; // Default value
        this->prev = nullptr;
        this->next = nullptr;
    }
};

struct StackNode {
    Task taskData;
    StackNode* next;
};

StackNode* stackTop = nullptr;
Task* head = nullptr;
Task* tail = nullptr;
string currentUsername;

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void setUsername() {
    setColor(10); // Green
    cout << "\nEnter your username: ";
    setColor(7); // White
    getline(cin, currentUsername);
    ifstream file(currentUsername + ".txt");
    if (!file.is_open()) {
        ofstream newFile(currentUsername + ".txt");
        newFile.close();
        setColor(14); // Yellow
        cout << "New file created for user \"" << currentUsername << "\".\n";
    }
    else {
        setColor(11); // Cyan
        cout << "Welcome back, \"" << currentUsername << "\".\n";
    }
    setColor(7); // Reset to White
}

void saveToFile() {
    if (currentUsername.empty()) return;
    ofstream file(currentUsername + ".txt");
    if (!file.is_open()) {
        setColor(12); // Red
        cout << "Error: Could not open file to save data.\n";
        setColor(7); // Reset
        return;
    }
    Task* current = head;
    while (current != nullptr) {
        file << current->id << "," << current->name << "," << current->description << ","
            << current->priority << "," << current->dueDate << "," << current->progress << "\n";
        current = current->next;
    }
    file.close();
}

void sortTask() {
    if (head == nullptr || head->next == nullptr)
        return;
    bool swapped = true;
    while (swapped) {
        swapped = false;
        Task* current = head;
        while (current->next) {
            if (current->priority > current->next->priority) {
                int tempPriority = current->priority;
                current->priority = current->next->priority;
                current->next->priority = tempPriority;
                swapped = true;
            }
            current = current->next;
        }
    }
}
bool isIdPresent(int id) {
    Task* current = head;
    while (current != nullptr) {
        if (current->id == id) {
            return true;
        }
        current = current->next;
    }
    return false;
}

void addTask(int id, const string& name, const string& description, int priority, int dueDate) {
    // Check if the ID already exists
    if (isIdPresent(id)) {
        cout << "Error: Task with ID " << id << " already exists." << endl;
        return;
    }

    // Create a new task dynamically
    Task* newTask = new Task(id, name, description, priority, dueDate);
    newTask->next = nullptr; // Initialize next pointer
    newTask->prev = nullptr; // Initialize prev pointer

    // Case 1: The list is empty
    if (head == nullptr) {
        head = tail = newTask; // Set both head and tail to the new task
    }
    // Case 2: Insert at the beginning (highest priority)
    else if (newTask->priority < head->priority) {
        newTask->next = head;
        head->prev = newTask;
        head = newTask; // Update the head to the new task
    }
    // Case 3: Insert in the middle or end
    else {
        Task* current = head;
        
        // Traverse to find the correct position
        while (current->next != nullptr && current->next->priority <= newTask->priority) {
            current = current->next;
        }

        // Insert after 'current'
        newTask->next = current->next;
        newTask->prev = current;
        current->next = newTask;

        // Update the previous pointer of the next node, if not at the end
        if (newTask->next != nullptr) {
            newTask->next->prev = newTask;
        } else {
            // If inserted at the end, update the tail
            tail = newTask;
        }
    }

    cout << "Task added successfully!" << endl;

    // Save the updated list to a file
    saveToFile();
}



void search() {
    int choice;

    // Validate search choice input
    while (true) {
        cout << "Search by:\n 1. Due Date\n 2. Task Priority\n 3. Task ID" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter a valid choice (1, 2, or 3).\n";
        } else if (choice < 1 || choice > 3) {
            cout << "Invalid choice. Please select between 1 and 3.\n";
        } else {
            cin.ignore(1000, '\n');
            break;
        }
    }

    switch (choice) {
        case 1: {
            int dueDate;

            // Validate dueDate input
            while (true) {
                cout << "Enter Due Date (YYMMDD): ";
                cin >> dueDate;

                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Invalid input. Please enter a valid Due Date (integer).\n";
                } else {
                    cin.ignore(1000, '\n');
                    break;
                }
            }

            Task* current = head;
            while (current != nullptr) {
                if (current->dueDate == dueDate) {
                    cout << "Task Found:" << endl;
                    cout << "ID: " << current->id << endl;
                    cout << "Name: " << current->name << endl;
                    cout << "Description: " << current->description << endl;
                    cout << "Priority: " << current->priority << endl;
                    cout << "Due Date: " << current->dueDate << endl;
                    cout << "Progress: " << current->progress << endl;
                    return;
                }
                current = current->next;
            }
            cout << "Task with Due Date " << dueDate << " not found." << endl;
            break;
        }

        case 2: {
            int priority;
            string pr;

            // Validate Priority input
            while (true) {
                cout << "Enter Priority (Low, Medium, High): ";
                cin >> pr;

                if (pr == "high" || pr == "High") {
                    priority = 1;
                    break;
                } else if (pr == "medium" || pr == "Medium") {
                    priority = 2;
                    break;
                } else if (pr == "low" || pr == "Low") {
                    priority = 3;
                    break;
                } else {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Invalid input. Please enter 'Low', 'Medium', or 'High'.\n";
                }
            }

            Task* current = head;
            while (current != nullptr) {
                if (current->priority == priority) {
                    cout << "Task Found:" << endl;
                    cout << "ID: " << current->id << endl;
                    cout << "Name: " << current->name << endl;
                    cout << "Description: " << current->description << endl;
                    cout << "Priority: " << current->priority << endl;
                    cout << "Due Date: " << current->dueDate << endl;
                    cout << "Progress: " << current->progress << endl;
                    return;
                }
                current = current->next;
            }
            cout << "Task with Priority " << priority << " not found." << endl;
            break;
        }

        case 3: {
            int id;

            // Validate Task ID input
            while (true) {
                cout << "Enter Task ID: ";
                cin >> id;

                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Invalid input. Please enter a valid Task ID (integer).\n";
                } else {
                    cin.ignore(1000, '\n');
                    break;
                }
            }

            Task* current = head;
            while (current != nullptr) {
                if (current->id == id) {
                    cout << "Task Found:" << endl;
                    cout << "ID: " << current->id << endl;
                    cout << "Name: " << current->name << endl;
                    cout << "Description: " << current->description << endl;
                    cout << "Priority: " << current->priority << endl;
                    cout << "Due Date: " << current->dueDate << endl;
                    cout << "Progress: " << current->progress << endl;
                    return;
                }
                current = current->next;
            }
            cout << "Task with ID " << id << " not found." << endl;
            break;
        }

        default:
            cout << "Invalid choice." << endl;
            break;
    }
}

void pushToStack(Task task) {
    StackNode* newNode = new StackNode{ task, stackTop };
    stackTop = newNode;
}

Task popFromStack() {
    if (!stackTop) {
        cout << "Undo stack is empty.\n";
        return Task{ -1, "", "", -1, -1 };
    }
    StackNode* temp = stackTop;
    Task taskData = stackTop->taskData;
    stackTop = stackTop->next;
    delete temp;
    return taskData;
}

void editTask(Task* head) {
    int id;

    // Validate Task ID input
    while (true) {
        cout << "Enter the ID of the task you want to edit: ";
        cin >> id;

        if (cin.fail()) { 
            cin.clear(); // Clear the error state
            cin.ignore(1000, '\n'); // Discard invalid input
            cout << "Invalid input. Please enter a valid Task ID (integer).\n";
        } else {
            cin.ignore(1000, '\n'); // Clear remaining characters in the input buffer
            break;
        }
    }

    Task* current = head;
    while (current != nullptr) {
        if (current->id == id) {
            pushToStack(*current);
            cout << "Editing Task: " << endl;

            // Edit Name
            cout << "Current Name: " << current->name << "\nEnter new Name (or press Enter to keep unchanged): ";
            cin.ignore(); // Clear newline left from previous input
            string newName;
            getline(cin, newName);
            if (!newName.empty()) {
                current->name = newName;
            }

            // Edit Description
            cout << "Current Description: " << current->description << "\nEnter new Description (or press Enter to keep unchanged): ";
            string newDescription;
            getline(cin, newDescription);
            if (!newDescription.empty()) {
                current->description = newDescription;
            }

            // Edit Priority
            cout << "Current Priority (High, Medium, Low): " 
                 << (current->priority == 1 ? "High" : (current->priority == 2 ? "Medium" : "Low")) << "\n";
            cout << "Enter new Priority (1 for High, 2 for Medium, 3 for Low, or press Enter to keep unchanged): ";
            string newPriority;
            getline(cin, newPriority);
            if (!newPriority.empty()) {
                try {
                    int priority = stoi(newPriority);
                    if (priority < 1 || priority > 3) throw invalid_argument("Invalid priority");
                    current->priority = priority;
                } catch (...) {
                    cout << "Invalid input. Priority remains unchanged.\n";
                }
            }

            // Edit Due Date
            cout << "Current Due Date (YYMMDD): " << current->dueDate << "\nEnter new Due Date (or press Enter to keep unchanged): ";
            string newDueDate;
            getline(cin, newDueDate);
            if (!newDueDate.empty()) {
                try {
                    current->dueDate = stoi(newDueDate);
                } catch (...) {
                    cout << "Invalid input. Due Date remains unchanged.\n";
                }
            }

            // Edit Progress
            cout << "Current Progress (in progress, completed): " << current->progress << "\nEnter new Progress (or press Enter to keep unchanged): ";
            string newProgress;
            getline(cin, newProgress);
            if (!newProgress.empty() && (newProgress == "in progress" || newProgress == "completed")) {
                current->progress = newProgress;
            } else if (!newProgress.empty()) {
                cout << "Invalid input. Progress remains unchanged.\n";
            }

            // Save and sort tasks
            sortTask();
            cout << "Task updated successfully!" << endl;
            saveToFile();
            return;
        }
        current = current->next;
    }

    cout << "Task with ID " << id << " not found." << endl;
}



void deleteTask() {
    int id;

    // Validate Task ID input
    while (true) {
        cout << "Enter the ID of the task you want to delete: ";
        cin >> id;

        if (cin.fail()) { 
            cin.clear(); // Clear the error state
            cin.ignore(1000, '\n'); // Discard invalid input
            cout << "Invalid input. Please enter a valid Task ID (integer).\n";
        } else {
            cin.ignore(1000, '\n'); // Clear remaining characters in the input buffer
            break;
        }
    }

    Task* current = head;
    while (current) {
        if (current->id == id) {
            pushToStack(*current);
            if (current == head && current == tail) {
                head = tail = nullptr;
            } else if (current == head) {
                head = head->next;
                head->prev = nullptr;
            } else if (current == tail) {
                tail = tail->prev;
                tail->next = nullptr;
            } else {
                current->prev->next = current->next;
                current->next->prev = current->prev;
            }
            delete current;
            cout << "Task deleted successfully." << endl;
            saveToFile();
            return;
        }
        current = current->next;
    }

    cout << "Task with ID " << id << " not found." << endl;
}




void undo() {
    Task restoredTask = popFromStack();
    if (restoredTask.id != -1) {
        addTask(restoredTask.id, restoredTask.name, restoredTask.description, restoredTask.priority, restoredTask.dueDate);
        cout << "Task restored successfully." << endl;
    }
    else {
        cout << "No task to restore." << endl;
    }
}

void markCompleted(int id) {
    Task* current = head;
    while (current) {
        if (current->id == id) {
            current->progress = "completed";
            cout << "Task marked as completed.\n";
            saveToFile();
            return;
        }
        current = current->next;
    }
    cout << "Task not found.\n";
}


void display(Task* head) {
    setColor(13); // Light Purple
    cout << "\n================== Task List ==================\n";
    setColor(7); // White

    Task* current = head;
    while (current != nullptr) {
        setColor(11); // Cyan
        cout << "\n----------------------------------------------\n";
        setColor(14); // Yellow
        cout << "ID: " << current->id << "\n";
        cout << "Name: " << current->name << "\n";
        cout << "Description: " << current->description << "\n";
        setColor(10); // Green
        cout << "Priority: " << (current->priority == 1 ? "High" : (current->priority == 2 ? "Medium" : "Low")) << "\n";
        cout << "Due Date: " << current->dueDate << "\n";
        cout << "Progress: " << current->progress << "\n";
        setColor(11); // Cyan
        cout << "----------------------------------------------\n";
        current = current->next;
    }
    setColor(13); // Light Purple
    cout << "==============================================\n\n";
    setColor(7); // Reset to White
}

void loadFromFile() {
    if (currentUsername.empty()) return;
    ifstream file(currentUsername + ".txt");
    if (!file.is_open()) {
        setColor(14); // Yellow
        cout << "No existing data for user \"" << currentUsername << "\". A new file will be created.\n";
        setColor(7); // Reset
        return;
    }
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int id, priority, dueDate;
        string name, description, progress;

        ss >> id;
        ss.ignore();
        getline(ss, name, ',');
        getline(ss, description, ',');
        ss >> priority;
        ss.ignore();
        ss >> dueDate;
        ss.ignore();
        getline(ss, progress);

        addTask(id, name, description, priority, dueDate);
        Task* lastTask = tail;
        if (lastTask) lastTask->progress = progress;
    }
    file.close();
}
void getIntInput(const string& prompt, int& value) {
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cin.clear(); // Clear error flag
            cin.ignore(1000, '\n'); // Discard invalid input
            cout << "Invalid input. Please enter a valid integer.\n";
        } else {
            cin.ignore(1000, '\n'); // Clear any remaining characters in the input buffer
            break;
        }
    }
}


int main() {
    setColor(13); // Light Purple
    cout << "\n\n\n\t\t\t\t---------------------------------\n";
    Sleep(350);
    setColor(11); // Cyan
    cout << "\t\t\t\t\tSmart To-Do List\n";
    Sleep(350);
    setColor(13); // Light Purple
    cout << "\t\t\t\t---------------------------------\n";
    setColor(7); // Reset
    cout << endl;
    Sleep(350);

    setUsername();
    loadFromFile();

    int choice;
    do {
        setColor(11); // Cyan
        cout << "\n\t\t\t\t================== Task Management System ==================\n";
        setColor(7); // White
        cout << "\n\t\t\t\t\t--------------------------------------------" << endl;
        cout << "\t\t\t\t\t\t||1. Add Task               ||   \n";
        cout << "\t\t\t\t\t\t||2. Edit Task              ||   \n";
        cout << "\t\t\t\t\t\t||3. Mark Task as Completed ||   \n";
        cout << "\t\t\t\t\t\t||4. Delete Task            ||   \n";
        cout << "\t\t\t\t\t\t||5. Search Task            ||   \n";
        cout << "\t\t\t\t\t\t||6. Undo Last Operation    ||   \n";
        cout << "\t\t\t\t\t\t||7. Display Tasks          ||   \n";
        cout << "\t\t\t\t\t\t||8. Exit                   ||   \n";
        cout << "\t\t\t\t\t---------------------------------------------" << endl;
        cout << "\nEnter your choice: ";

        setColor(14); // Yellow
        cin >> choice;
        setColor(7); // Reset

        switch (choice) {
        case 1: {
            int id, priority, dueDate;
            string name, description;
            setColor(10); // Green
            cout << "Enter Task ID: ";
            setColor(7); // White
            cin >> id;
            cin.ignore();
            setColor(10);
            cout << "Enter Task Name: ";
            setColor(7);
            getline(cin, name);
            setColor(10);
            cout << "Enter Task Description: ";
            setColor(7);
            getline(cin, description);
            setColor(10);
            cout << "Enter Task Priority (1 for High, 2 for Medium, 3 for Low): ";
            setColor(7);
            cin >> priority;
            setColor(10);
            cout << "Enter Due Date (YYMMDD): ";
            setColor(7);
            cin >> dueDate;
            addTask(id, name, description, priority, dueDate);
            break;
        }
        case 2:
            //editTask;
            editTask(head);
            break;
        case 3: {
            int id;
            setColor(10);
            cout << "Enter task ID to mark as completed: ";
            setColor(7);
            cin >> id;
            markCompleted(id);
            break;
        }
        case 4: {
            int id;
            setColor(10);
            cout << "Enter Task ID to delete: ";
            setColor(7);
            cin >> id;
            deleteTask();
            break;
        }
        case 5:
            search();
            break;
        case 6:
            undo();
            break;
        case 7:
            display(head);
            break;
        case 8:
            setColor(12); // Red
            cout << "Exiting...\n";
            setColor(7); // Reset
            break;
        default:
            setColor(12); // Red
            cout << "Invalid choice, please try again.\n";
            setColor(7); // Reset
        }
    } while (choice != 8);
}
