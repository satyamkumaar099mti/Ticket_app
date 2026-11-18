#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "../models/User.hpp"
#include "../utils/Utils.hpp"

namespace Repositories {

class UserRepository {
private:
    std::string filePath;
    std::vector<Models::User> users;

    void load() {
        users.clear();
        std::ifstream file(filePath);
        if (!file.is_open()) return;

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            auto fields = Utils::parseCSVLine(line);
            if (fields.size() >= 7) {
                std::string id = fields[0];
                std::string username = fields[1];
                std::string passwordHash = fields[2];
                std::string fullName = fields[3];
                std::string email = fields[4];
                Models::Role role = Models::roleFromString(fields[5]);
                bool active = (fields[6] == "1" || fields[6] == "true");

                users.emplace_back(id, username, passwordHash, fullName, email, role, active);
            }
        }
        file.close();
    }

    void save() {
        std::ofstream file(filePath, std::ios::trunc);
        if (!file.is_open()) return;

        for (const auto& user : users) {
            file << Utils::escapeCSV(user.getId()) << ","
                 << Utils::escapeCSV(user.getUsername()) << ","
                 << Utils::escapeCSV(user.getPasswordHash()) << ","
                 << Utils::escapeCSV(user.getFullName()) << ","
                 << Utils::escapeCSV(user.getEmail()) << ","
                 << Utils::escapeCSV(Models::roleToString(user.getRole())) << ","
                 << (user.isActive() ? "1" : "0") << "\n";
        }
        file.close();
    }

public:
    explicit UserRepository(std::string path = "data_users.csv")
        : filePath(std::move(path)) {
        load();
        // Seed default admin and sample accounts if empty
        if (users.empty()) {
            users.emplace_back("U-1001", "admin", Utils::hashPassword("admin123"), "System Administrator", "admin@support.local", Models::Role::ADMIN, true);
            users.emplace_back("U-1002", "agent_alice", Utils::hashPassword("alice123"), "Alice Smith (Tier 2)", "alice@support.local", Models::Role::AGENT, true);
            users.emplace_back("U-1003", "agent_bob", Utils::hashPassword("bob123"), "Bob Johnson (Tier 1)", "bob@support.local", Models::Role::AGENT, true);
            users.emplace_back("U-1004", "john_doe", Utils::hashPassword("pass123"), "John Doe", "john@customer.com", Models::Role::CUSTOMER, true);
            save();
        }
    }

    std::vector<Models::User> getAll() const {
        return users;
    }

    Models::User* findByUsername(const std::string& username) {
        for (auto& user : users) {
            if (user.getUsername() == username) {
                return &user;
            }
        }
        return nullptr;
    }

    Models::User* findById(const std::string& id) {
        for (auto& user : users) {
            if (user.getId() == id) {
                return &user;
            }
        }
        return nullptr;
    }

    bool addUser(const Models::User& user) {
        if (findByUsername(user.getUsername()) != nullptr) {
            return false;
        }
        users.push_back(user);
        save();
        return true;
    }

    bool updateUser(const Models::User& updatedUser) {
        for (auto& user : users) {
            if (user.getId() == updatedUser.getId()) {
                user = updatedUser;
                save();
                return true;
            }
        }
        return false;
    }

    std::string generateNextId() {
        int maxId = 1000;
        for (const auto& u : users) {
            if (u.getId().rfind("U-", 0) == 0) {
                try {
                    int val = std::stoi(u.getId().substr(2));
                    if (val > maxId) maxId = val;
                } catch (...) {}
            }
        }
        return "U-" + std::to_string(maxId + 1);
    }
};

} // namespace Repositories

