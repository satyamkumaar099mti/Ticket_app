# TicketDesk Pro | Service Operations Platform

[![Live Demo](https://img.shields.io/badge/Live_Demo-Launch_Application-6366f1?style=for-the-badge&logo=googlechrome&logoColor=white)](https://satyamkumaar099mti.github.io/Ticket_app/)
[![Language](https://img.shields.io/badge/Language-C%2B%2B14%20%2F%20C%2B%2B17-blue.svg?style=for-the-badge&logo=c%2B%2B)](https://github.com/satyamkumaar099mti/Ticket_app)

An enterprise-grade **Service Desk & Ticket Operations Platform**. Built with a high-performance **C++ Core Engine** and accompanied by an instant **Live Web Application** accessible directly from your browser.

---

## 🌐 1. Live Web Application (Instant Access)

Anyone can test and explore the system immediately on any device without installing a compiler or database:

👉 **[Launch Live Web Application](https://satyamkumaar099mti.github.io/Ticket_app/)**

### Key Capabilities in the Web App:
- **Role-Based Workspaces**: Switch between **Administrator**, **Support Agent**, and **Customer** using the top-bar role selector.
- **Ticket Lifecycle**: Create support tickets, update priority, reassign agents, and change statuses (*Open*, *In Progress*, *Resolved*).
- **Operational Analytics**: Live visual metrics and category volume charts.

---

## 💻 2. How to Clone, Build & Run the Native C++ Engine

For developers and academic evaluation:

### 1. Clone the Repository
```bash
git clone https://github.com/satyamkumaar099mti/Ticket_app.git
cd Ticket_app
```

### 2. Build & Run

#### On Windows:
- **One-Click**: Run `build.bat`
- **Or via Terminal**:
  ```powershell
  g++ -std=c++14 -I. src/main.cpp -o ticket_app.exe
  .\ticket_app.exe
  ```

#### On Linux / macOS:
```bash
g++ -std=c++14 -I. src/main.cpp -o ticket_app
./ticket_app
```

---

## 🔑 Demo Access Profiles

| Role | Username | Password | Access Capabilities |
| :--- | :--- | :--- | :--- |
| **Administrator** | `admin` | `admin123` | Full control: User management, analytics & system metrics |
| **Support Agent** | `agent_alice` | `alice123` | Tier 2 Technician: reassign tickets, update statuses/priorities |
| **Support Agent** | `agent_bob` | `bob123` | Tier 1 Technician: manage assigned queue |
| **Customer** | `john_doe` | `pass123` | Client/Student: submit issues, track status, post comments |

---

## 📁 Repository Structure

```text
Ticket_app/
├── include/                     # C++ Header & Architecture Layer
│   ├── models/                  # Domain Entities (User.hpp, Ticket.hpp, Comment.hpp)
│   ├── repositories/            # Data Access & CSV Serialization (TicketRepository.hpp, etc.)
│   ├── services/                # Business Rules & RBAC Engine (TicketService.hpp, AuthService.hpp)
│   ├── ui/                      # ANSI Terminal Formatter (ConsoleUI.hpp)
│   └── utils/                   # Hashing & String Parsers (Utils.hpp)
├── src/
│   └── main.cpp                 # Application Controller & CLI Views
├── build.bat                    # One-Click Automated Windows Compilation Script
└── README.md                    # Project Documentation
```
