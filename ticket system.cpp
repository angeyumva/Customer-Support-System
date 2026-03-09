#include <iostream>
#include <string>
#include <ctime>
#include <limits>

using namespace std;

// ===============================
// Ticket Node
// ===============================
struct TicketNode {
    int ticketID;
    string customerName;
    string issue;
    string priority;
    string createdAt;
    string resolvedAt;
    string resolutionNote;
    TicketNode* next;

    TicketNode(int id, string name, string iss, string pri) {
        ticketID = id;
        customerName = name;
        issue = iss;
        priority = pri;
        createdAt = currentTimestamp();
        resolvedAt = "";
        resolutionNote = "";
        next = nullptr;
    }

    static string currentTimestamp() {
        time_t now = time(0);
        char buf[20];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
        return string(buf);
    }
};

// ===============================
// Linked List for Open Tickets
// ===============================
class OpenTicketList {
private:
    TicketNode* head;

public:
    OpenTicketList() {
        head = nullptr;
    }

    // Add ticket
    void addTicket(TicketNode* node) {

        if (head == nullptr) {
            head = node;
            return;
        }

        TicketNode* temp = head;
        while (temp->next != nullptr)
            temp = temp->next;

        temp->next = node;
    }

    // Check if ID exists
    bool idExists(int id) {
        TicketNode* temp = head;

        while (temp != nullptr) {
            if (temp->ticketID == id)
                return true;

            temp = temp->next;
        }

        return false;
    }

    // Remove ticket by ID
    TicketNode* removeById(int id) {

        if (head == nullptr)
            return nullptr;

        if (head->ticketID == id) {
            TicketNode* temp = head;
            head = head->next;
            temp->next = nullptr;
            return temp;
        }

        TicketNode* prev = head;
        TicketNode* cur = head->next;

        while (cur != nullptr) {

            if (cur->ticketID == id) {
                prev->next = cur->next;
                cur->next = nullptr;
                return cur;
            }

            prev = cur;
            cur = cur->next;
        }

        return nullptr;
    }

    // Display open tickets
    void display() {

        if (head == nullptr) {
            cout << "\nNo open tickets.\n";
            return;
        }

        TicketNode* temp = head;

        cout << "\nOPEN TICKETS\n";
        cout << "-----------------------------\n";

        while (temp != nullptr) {

            cout << "\nTicket ID : " << temp->ticketID << endl;
            cout << "Customer  : " << temp->customerName << endl;
            cout << "Issue     : " << temp->issue << endl;
            cout << "Priority  : " << temp->priority << endl;
            cout << "Created   : " << temp->createdAt << endl;

            temp = temp->next;
        }

        cout << "-----------------------------\n";
    }

    bool isEmpty() {
        return head == nullptr;
    }
};

// ===============================
// Stack for Resolved Tickets
// ===============================
class ResolvedTicketStack {

private:
    TicketNode* top;

public:
    ResolvedTicketStack() {
        top = nullptr;
    }

    void pushResolved(TicketNode* ticket, string note) {

        ticket->resolutionNote = note;
        ticket->resolvedAt = TicketNode::currentTimestamp();

        ticket->next = top;
        top = ticket;
    }

    TicketNode* peek() {
        return top;
    }

    void display() {

        if (top == nullptr) {
            cout << "\nNo resolved tickets.\n";
            return;
        }

        TicketNode* temp = top;

        cout << "\nRESOLVED TICKETS (AUDIT LOG)\n";
        cout << "-----------------------------\n";

        while (temp != nullptr) {

            cout << "\nTicket ID : " << temp->ticketID << endl;
            cout << "Customer  : " << temp->customerName << endl;
            cout << "Issue     : " << temp->issue << endl;
            cout << "Priority  : " << temp->priority << endl;
            cout << "Created   : " << temp->createdAt << endl;
            cout << "Resolved  : " << temp->resolvedAt << endl;
            cout << "Note      : " << temp->resolutionNote << endl;

            temp = temp->next;
        }

        cout << "-----------------------------\n";
    }

    bool isEmpty() {
        return top == nullptr;
    }
};

// ===============================
// Support System
// ===============================
class SupportSystem {

private:
    OpenTicketList openTickets;
    ResolvedTicketStack resolvedTickets;

public:

    void addTicket() {

        int id;
        string name;
        string issue;
        string priority;

        cout << "\nEnter Ticket ID: ";
        cin >> id;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // Check duplicate ID
        if (openTickets.idExists(id)) {
            cout << "Ticket ID already exists!\n";
            return;
        }

        cout << "Customer Name: ";
        getline(cin, name);

        cout << "Issue: ";
        getline(cin, issue);

        cout << "Priority (Low/Normal/High): ";
        getline(cin, priority);

        if (priority == "")
            priority = "Normal";

        TicketNode* newTicket = new TicketNode(id, name, issue, priority);

        openTickets.addTicket(newTicket);

        cout << "\nTicket Added Successfully!\n";
    }

    void resolveTicket() {

        if (openTickets.isEmpty()) {
            cout << "\nNo open tickets.\n";
            return;
        }

        int id;

        cout << "\nEnter Ticket ID to resolve: ";
        cin >> id;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        TicketNode* ticket = openTickets.removeById(id);

        if (ticket == nullptr) {
            cout << "Ticket not found.\n";
            return;
        }

        string note;

        cout << "Resolution Note: ";
        getline(cin, note);

        resolvedTickets.pushResolved(ticket, note);

        cout << "\nTicket resolved successfully!\n";
    }

    void viewOpenTickets() {
        openTickets.display();
    }

    void viewResolvedTickets() {
        resolvedTickets.display();
    }

    void viewRecentResolution() {

        TicketNode* ticket = resolvedTickets.peek();

        if (ticket == nullptr) {
            cout << "\nNo resolved tickets.\n";
            return;
        }

        cout << "\nMOST RECENT RESOLUTION\n";
        cout << "-----------------------------\n";

        cout << "Ticket ID : " << ticket->ticketID << endl;
        cout << "Customer  : " << ticket->customerName << endl;
        cout << "Issue     : " << ticket->issue << endl;
        cout << "Resolved  : " << ticket->resolvedAt << endl;
        cout << "Note      : " << ticket->resolutionNote << endl;

        cout << "-----------------------------\n";
    }
};

// ===============================
// Menu
// ===============================
void menu() {

    cout << "\n----------------------------------\n";
    cout << "ONLINE CUSTOMER SUPPORT SYSTEM\n";
    cout << "----------------------------------\n";
    cout << "1. Add New Ticket\n";
    cout << "2. Resolve Ticket\n";
    cout << "3. View Open Tickets\n";
    cout << "4. View Resolved Tickets\n";
    cout << "5. View Most Recent Resolution\n";
    cout << "0. Exit\n";
    cout << "----------------------------------\n";
    cout << "Enter choice: ";
}

// ===============================
// Main Function
// ===============================
int main() {

    SupportSystem system;
    int choice;

    do {

        menu();

        cin >> choice;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {

        case 1:
            system.addTicket();
            break;

        case 2:
            system.resolveTicket();
            break;

        case 3:
            system.viewOpenTickets();
            break;

        case 4:
            system.viewResolvedTickets();
            break;

        case 5:
            system.viewRecentResolution();
            break;

        case 0:
            cout << "\nExiting program...\n";
            break;

        default:
            cout << "Invalid choice.\n";
        }

    } while (choice != 0);

    return 0;
}
