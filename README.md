# TicketDesk Pro | C++ University Project

[![Language](https://img.shields.io/badge/Language-C%2B%2B14%20%2F%20C%2B%2B17-blue.svg?style=for-the-badge&logo=c%2B%2B)](https://github.com/satyamkumaar099mti/Ticket_app)
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg?style=for-the-badge)](https://github.com/satyamkumaar099mti/Ticket_app)

A complete **Ticket Management System** engineered in **pure Object-Oriented C++**. It implements a layered architecture, Role-Based Access Control (RBAC), multi-criteria search algorithms, data analytics, and CSV persistence with zero external dependencies.

---

## ⚡ How to Clone, Build & Run

### 1. Prerequisites
- Any standard C++ compiler (`g++` / MinGW, `clang`, or MSVC).

### 2. Clone Repository
```bash
git clone https://github.com/satyamkumaar099mti/Ticket_app.git
cd Ticket_app
```

### 3. Build & Run

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

The application auto-seeds sample accounts on first run:

| Role | Username | Password | Access Capabilities |
| :--- | :--- | :--- | :--- |
| **Administrator** | `admin` | `admin123` | Full control: User management, analytics & system metrics |
| **Support Agent** | `agent_alice` | `alice123` | Tier 2 Technician: reassign tickets, update statuses/priorities, internal staff notes |
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
