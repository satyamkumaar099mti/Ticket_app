#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
#include <cstdlib>

#include "include/models/User.hpp"
#include "include/models/Ticket.hpp"
#include "include/models/Comment.hpp"
#include "include/repositories/UserRepository.hpp"
#include "include/repositories/TicketRepository.hpp"
#include "include/repositories/CommentRepository.hpp"
#include "include/services/AuthService.hpp"
#include "include/services/TicketService.hpp"
#include "include/ui/ConsoleUI.hpp"

using namespace Models;
using namespace Services;
using namespace Repositories;
using namespace UI;

void clearConsole() {
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#else
    system("clear");
#endif
}

void pauseConsole() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

void displayTicketsTable(const std::vector<Ticket>& list) {
    if (list.empty()) {
        std::cout << "\n  (No tickets found matching current view)\n";
        return;
    }

    std::cout << "\n" << Color::BOLD
              << std::left << std::setw(10) << "ID"
              << std::setw(30) << "TITLE"
              << std::setw(12) << "CATEGORY"
              << std::setw(12) << "PRIORITY"
              << std::setw(14) << "STATUS"
              << std::setw(15) << "ASSIGNEE"
              << Color::RESET << "\n";
    std::cout << std::string(93, '-') << "\n";

    for (const auto& t : list) {
        std::string titleTrunc = t.getTitle();
        if (titleTrunc.length() > 27) {
            titleTrunc = titleTrunc.substr(0, 24) + "...";
        }

        std::string prioColor = Color::RESET;
        if (t.getPriority() == Priority::CRITICAL) prioColor = Color::RED;
        else if (t.getPriority() == Priority::HIGH) prioColor = Color::YELLOW;
        else if (t.getPriority() == Priority::MEDIUM) prioColor = Color::CYAN;

        std::string statusColor = Color::RESET;
        if (t.getStatus() == Status::OPEN) statusColor = Color::YELLOW;
        else if (t.getStatus() == Status::IN_PROGRESS) statusColor = Color::BLUE;
        else if (t.getStatus() == Status::RESOLVED) statusColor = Color::GREEN;
        else if (t.getStatus() == Status::CLOSED) statusColor = Color::WHITE;

        std::cout << std::left << std::setw(10) << t.getId()
                  << std::setw(30) << titleTrunc
                  << std::setw(12) << t.getCategory()
                  << prioColor << std::setw(12) << priorityToString(t.getPriority()) << Color::RESET
                  << statusColor << std::setw(14) << statusToString(t.getStatus()) << Color::RESET
                  << std::setw(15) << t.getAssigneeUsername()
                  << "\n";
    }
}

void viewTicketDetails(TicketService& ticketService, AuthService& authService) {
    std::string id = readLine("Enter Ticket ID to view (e.g. TCK-101): ");
    Ticket* t = ticketService.getTicket(id);
    if (!t) {
        printError("Ticket not found!");
        pauseConsole();
        return;
    }

    User* user = authService.getCurrentUser();
    bool isStaff = (user->getRole() == Role::AGENT || user->getRole() == Role::ADMIN);
    if (!isStaff && t->getCreatorUsername() != user->getUsername()) {
        printError("Access denied: You do not have permission to view this ticket.");
        pauseConsole();
        return;
    }

    while (true) {
        clearConsole();
        printSubBanner("Ticket Details: " + t->getId());
        std::cout << "Title:       " << Color::BOLD << t->getTitle() << Color::RESET << "\n";
        std::cout << "Category:    " << t->getCategory() << "\n";
        std::cout << "Priority:    " << priorityToString(t->getPriority()) << "\n";
        std::cout << "Status:      " << statusToString(t->getStatus()) << "\n";
        std::cout << "Creator:     " << t->getCreatorUsername() << "\n";
        std::cout << "Assignee:    " << t->getAssigneeUsername() << "\n";
        std::cout << "Created:     " << t->getCreatedAt() << "\n";
        std::cout << "Last Update: " << t->getUpdatedAt() << "\n";
        std::cout << "\nDescription:\n  " << t->getDescription() << "\n";

        // Display Comments
        std::cout << "\n--- Ticket Comments & Activity --- \n";
        auto comments = ticketService.getTicketComments(t->getId(), isStaff);
        if (comments.empty()) {
            std::cout << "  (No comments yet)\n";
        } else {
            for (const auto& c : comments) {
                std::cout << "[" << c.getTimestamp() << "] "
                          << Color::BOLD << c.getAuthorUsername() << Color::RESET;
                if (c.getIsInternal()) {
                    std::cout << Color::MAGENTA << " [INTERNAL NOTE]" << Color::RESET;
                }
                std::cout << ":\n  " << c.getContent() << "\n\n";
            }
        }

        std::cout << "\nActions:\n";
        std::cout << "1. Add Comment\n";
        if (isStaff) {
            std::cout << "2. Add Internal Note (Staff only)\n";
            std::cout << "3. Update Status\n";
            std::cout << "4. Update Priority\n";
            std::cout << "5. Assign Agent\n";
        } else if (t->getStatus() == Status::RESOLVED) {
            std::cout << "2. Confirm Resolution & Close Ticket\n";
        }
        std::cout << "0. Back\n";

        std::string choice = readLine("\nSelect an action: ");
        if (choice == "0") {
            break;
        } else if (choice == "1") {
            std::string content = readLine("Enter your comment: ");
            if (!content.empty()) {
                ticketService.addComment(t->getId(), user->getUsername(), content, false);
                printSuccess("Comment posted.");
            }
        } else if (choice == "2" && isStaff) {
            std::string content = readLine("Enter internal note: ");
            if (!content.empty()) {
                ticketService.addComment(t->getId(), user->getUsername(), content, true);
                printSuccess("Internal note saved.");
            }
        } else if (choice == "2" && !isStaff && t->getStatus() == Status::RESOLVED) {
            ticketService.updateStatus(t->getId(), Status::CLOSED);
            ticketService.addComment(t->getId(), user->getUsername(), "Closed by customer.", false);
            printSuccess("Ticket closed.");
        } else if (choice == "3" && isStaff) {
            std::cout << "\nSelect Status: 1. OPEN  2. IN_PROGRESS  3. RESOLVED  4. CLOSED: ";
            std::string s;
            std::getline(std::cin, s);
            if (s == "1") ticketService.updateStatus(t->getId(), Status::OPEN);
            else if (s == "2") ticketService.updateStatus(t->getId(), Status::IN_PROGRESS);
            else if (s == "3") ticketService.updateStatus(t->getId(), Status::RESOLVED);
            else if (s == "4") ticketService.updateStatus(t->getId(), Status::CLOSED);
            printSuccess("Status updated.");
        } else if (choice == "4" && isStaff) {
            std::cout << "\nSelect Priority: 1. LOW  2. MEDIUM  3. HIGH  4. CRITICAL: ";
            std::string p;
            std::getline(std::cin, p);
            if (p == "1") ticketService.updatePriority(t->getId(), Priority::LOW);
            else if (p == "2") ticketService.updatePriority(t->getId(), Priority::MEDIUM);
            else if (p == "3") ticketService.updatePriority(t->getId(), Priority::HIGH);
            else if (p == "4") ticketService.updatePriority(t->getId(), Priority::CRITICAL);
            printSuccess("Priority updated.");
        } else if (choice == "5" && isStaff) {
            std::string targetAgent = readLine("Enter agent username to assign: ");
            if (ticketService.assignTicket(t->getId(), targetAgent)) {
                printSuccess("Ticket assigned to " + targetAgent);
            } else {
                printError("Agent not found or invalid role.");
            }
        }
        pauseConsole();
    }
}

void createNewTicketView(TicketService& ticketService, AuthService& authService) {
    clearConsole();
    printSubBanner("Create New Ticket");
    std::string title = readLine("Ticket Title: ");
    if (title.empty()) {
        printError("Title cannot be empty.");
        pauseConsole();
        return;
    }

    std::string desc = readLine("Detailed Description: ");
    std::cout << "Categories: 1. Hardware  2. Software  3. Network  4. Account  5. Other\n";
    std::string catChoice = readLine("Select Category [1-5]: ");
    std::string category = "Other";
    if (catChoice == "1") category = "Hardware";
    else if (catChoice == "2") category = "Software";
    else if (catChoice == "3") category = "Network";
    else if (catChoice == "4") category = "Account";

    std::cout << "Priority: 1. LOW  2. MEDIUM  3. HIGH  4. CRITICAL\n";
    std::string pChoice = readLine("Select Priority [1-4] (default 2): ");
    Priority prio = Priority::MEDIUM;
    if (pChoice == "1") prio = Priority::LOW;
    else if (pChoice == "3") prio = Priority::HIGH;
    else if (pChoice == "4") prio = Priority::CRITICAL;

    std::string ticketId = ticketService.createTicket(
        title, desc, category, prio, authService.getCurrentUser()->getUsername()
    );

    printSuccess("Ticket created successfully! Your Ticket ID: " + ticketId);
    pauseConsole();
}

void searchTicketsView(TicketService& ticketService, AuthService& authService) {
    clearConsole();
    printSubBanner("Search & Filter Tickets");
    std::string keyword = readLine("Search Keyword (title/desc/id, leave empty for all): ");
    std::string cat = readLine("Category filter (Hardware/Software/Network/Account or empty): ");
    std::string stat = readLine("Status filter (OPEN/IN_PROGRESS/RESOLVED/CLOSED or empty): ");
    std::string prio = readLine("Priority filter (LOW/MEDIUM/HIGH/CRITICAL or empty): ");

    auto results = ticketService.searchAndFilter(keyword, cat, stat, prio, *authService.getCurrentUser());
    displayTicketsTable(results);
    pauseConsole();
}

void reportsDashboardView(TicketService& ticketService) {
    clearConsole();
    printBanner("Analytics & Operational Dashboard");
    auto stats = ticketService.getStatistics();

    std::cout << "\n" << Color::BOLD << "--- System Metrics ---" << Color::RESET << "\n";
    std::cout << "Total Tickets:     " << stats.totalTickets << "\n";
    std::cout << "Open Tickets:      " << stats.openTickets << "\n";
    std::cout << "In Progress:       " << stats.inProgressTickets << "\n";
    std::cout << "Resolved Tickets:  " << stats.resolvedTickets << "\n";
    std::cout << "Closed Tickets:    " << stats.closedTickets << "\n";
    std::cout << "Critical Tickets:  " << stats.criticalTickets << "\n";

    std::cout << "\n" << Color::BOLD << "--- Tickets by Category ---" << Color::RESET << "\n";
    for (const auto& pair : stats.categoryCounts) {
        std::cout << "  * " << std::left << std::setw(15) << pair.first << ": " << pair.second << "\n";
    }

    std::cout << "\n" << Color::BOLD << "--- Agent Active Workload ---" << Color::RESET << "\n";
    for (const auto& pair : stats.agentWorkload) {
        std::cout << "  * " << std::left << std::setw(15) << pair.first << ": " << pair.second << " tickets\n";
    }

    pauseConsole();
}

void userManagementView(UserRepository& userRepo) {
    clearConsole();
    printSubBanner("User Management (Admin Only)");
    std::cout << "1. List All Users\n";
    std::cout << "2. Create New Agent/Staff Account\n";
    std::cout << "3. Toggle User Active Status\n";
    std::cout << "0. Back\n";

    std::string choice = readLine("\nSelect option: ");
    if (choice == "1") {
        std::cout << "\n" << std::left << std::setw(10) << "ID"
                  << std::setw(16) << "USERNAME"
                  << std::setw(24) << "FULL NAME"
                  << std::setw(18) << "ROLE"
                  << std::setw(8) << "STATUS" << "\n";
        std::cout << std::string(76, '-') << "\n";
        for (const auto& u : userRepo.getAll()) {
            std::cout << std::left << std::setw(10) << u.getId()
                      << std::setw(16) << u.getUsername()
                      << std::setw(24) << u.getFullName()
                      << std::setw(18) << roleToString(u.getRole())
                      << (u.isActive() ? "Active" : "Disabled") << "\n";
        }
        pauseConsole();
    } else if (choice == "2") {
        std::string username = readLine("Username: ");
        std::string password = readLine("Password: ");
        std::string name = readLine("Full Name: ");
        std::string email = readLine("Email: ");
        std::string r = readLine("Role (2 for Agent, 3 for Admin): ");
        Role role = (r == "3") ? Role::ADMIN : Role::AGENT;

        std::string id = userRepo.generateNextId();
        User nu(id, username, Utils::hashPassword(password), name, email, role, true);
        if (userRepo.addUser(nu)) {
            printSuccess("Account created successfully!");
        } else {
            printError("Username already exists.");
        }
        pauseConsole();
    } else if (choice == "3") {
        std::string username = readLine("Enter username: ");
        User* u = userRepo.findByUsername(username);
        if (u) {
            u->setActive(!u->isActive());
            userRepo.updateUser(*u);
            printSuccess("Status updated to: " + std::string(u->isActive() ? "Active" : "Disabled"));
        } else {
            printError("User not found.");
        }
        pauseConsole();
    }
}

void authenticatedDashboard(AuthService& authService, TicketService& ticketService, UserRepository& userRepo) {
    while (authService.isAuthenticated()) {
        clearConsole();
        User* user = authService.getCurrentUser();
        printBanner("TicketDesk Pro - Welcome, " + user->getFullName() + " (" + roleToString(user->getRole()) + ")");

        std::cout << "\nMy Active Tickets Summary:\n";
        auto myTickets = ticketService.getTicketsForUser(*user);
        displayTicketsTable(myTickets);

        std::cout << "\n" << Color::BOLD << "Main Menu:" << Color::RESET << "\n";
        std::cout << "1. View / Update Specific Ticket\n";
        std::cout << "2. Create New Ticket\n";
        std::cout << "3. Search & Filter Tickets\n";
        if (user->getRole() == Role::ADMIN || user->getRole() == Role::AGENT) {
            std::cout << "4. Reports & System Metrics\n";
        }
        if (user->getRole() == Role::ADMIN) {
            std::cout << "5. User Administration\n";
        }
        std::cout << "9. Logout\n";

        std::string choice = readLine("\nChoose an option: ");
        if (choice == "1") {
            viewTicketDetails(ticketService, authService);
        } else if (choice == "2") {
            createNewTicketView(ticketService, authService);
        } else if (choice == "3") {
            searchTicketsView(ticketService, authService);
        } else if (choice == "4" && (user->getRole() == Role::ADMIN || user->getRole() == Role::AGENT)) {
            reportsDashboardView(ticketService);
        } else if (choice == "5" && user->getRole() == Role::ADMIN) {
            userManagementView(userRepo);
        } else if (choice == "9") {
            authService.logout();
            printInfo("Logged out successfully.");
            pauseConsole();
            break;
        }
    }
}

void loginFlow(AuthService& authService, TicketService& ticketService, UserRepository& userRepo) {
    clearConsole();
    printSubBanner("User Login");
    std::string username = readLine("Username: ");
    std::string password = readLine("Password: ");

    if (authService.login(username, password)) {
        printSuccess("Authentication successful!");
        pauseConsole();
        authenticatedDashboard(authService, ticketService, userRepo);
    } else {
        printError("Invalid username, password, or account is disabled.");
        pauseConsole();
    }
}

void registerFlow(AuthService& authService) {
    clearConsole();
    printSubBanner("Customer Registration");
    std::string username = readLine("Desired Username: ");
    std::string password = readLine("Password: ");
    std::string name = readLine("Full Name: ");
    std::string email = readLine("Email: ");

    if (username.empty() || password.empty() || name.empty()) {
        printError("All fields are required.");
        pauseConsole();
        return;
    }

    if (authService.registerCustomer(username, password, name, email)) {
        printSuccess("Account created successfully! You can now log in.");
    } else {
        printError("Username is already taken.");
    }
    pauseConsole();
}

int main() {
    UserRepository userRepo("data_users.csv");
    TicketRepository ticketRepo("data_tickets.csv");
    CommentRepository commentRepo("data_comments.csv");

    AuthService authService(userRepo);
    TicketService ticketService(ticketRepo, commentRepo, userRepo);

    while (true) {
        clearConsole();
        printBanner("TicketDesk Pro - University C++ Project");
        std::cout << "1. Login\n";
        std::cout << "2. Register New Customer Account\n";
        std::cout << "3. View Default Demo Credentials\n";
        std::cout << "0. Exit\n";

        std::string choice = readLine("\nSelect an option: ");
        if (choice == "1") {
            loginFlow(authService, ticketService, userRepo);
        } else if (choice == "2") {
            registerFlow(authService);
        } else if (choice == "3") {
            clearConsole();
            printSubBanner("Seed Credentials for Demo / Testing");
            std::cout << "Administrator:  Username: admin        | Password: admin123\n";
            std::cout << "Support Agent:  Username: agent_alice  | Password: alice123\n";
            std::cout << "Support Agent:  Username: agent_bob    | Password: bob123\n";
            std::cout << "Customer User:  Username: john_doe     | Password: pass123\n";
            pauseConsole();
        } else if (choice == "0") {
            std::cout << "\nExiting TicketDesk. Goodbye!\n";
            break;
        }
    }

    return 0;
}

