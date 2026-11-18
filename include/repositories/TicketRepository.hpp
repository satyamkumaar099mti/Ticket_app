#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include "../models/Ticket.hpp"
#include "../utils/Utils.hpp"

namespace Repositories {

class TicketRepository {
private:
    std::string filePath;
    std::vector<Models::Ticket> tickets;

    void load() {
        tickets.clear();
        std::ifstream file(filePath);
        if (!file.is_open()) return;

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            auto fields = Utils::parseCSVLine(line);
            if (fields.size() >= 10) {
                std::string id = fields[0];
                std::string title = fields[1];
                std::string desc = fields[2];
                std::string category = fields[3];
                Models::Priority prio = Models::priorityFromString(fields[4]);
                Models::Status status = Models::statusFromString(fields[5]);
                std::string creator = fields[6];
                std::string assignee = fields[7];
                std::string createdAt = fields[8];
                std::string updatedAt = fields[9];

                tickets.emplace_back(id, title, desc, category, prio, status, creator, assignee, createdAt, updatedAt);
            }
        }
        file.close();
    }

    void save() {
        std::ofstream file(filePath, std::ios::trunc);
        if (!file.is_open()) return;

        for (const auto& t : tickets) {
            file << Utils::escapeCSV(t.getId()) << ","
                 << Utils::escapeCSV(t.getTitle()) << ","
                 << Utils::escapeCSV(t.getDescription()) << ","
                 << Utils::escapeCSV(t.getCategory()) << ","
                 << Utils::escapeCSV(Models::priorityToString(t.getPriority())) << ","
                 << Utils::escapeCSV(Models::statusToString(t.getStatus())) << ","
                 << Utils::escapeCSV(t.getCreatorUsername()) << ","
                 << Utils::escapeCSV(t.getAssigneeUsername()) << ","
                 << Utils::escapeCSV(t.getCreatedAt()) << ","
                 << Utils::escapeCSV(t.getUpdatedAt()) << "\n";
        }
        file.close();
    }

public:
    explicit TicketRepository(std::string path = "data_tickets.csv")
        : filePath(std::move(path)) {
        load();
        // Seed default tickets if empty
        if (tickets.empty()) {
            tickets.emplace_back("TCK-101", "Cannot connect to campus Wi-Fi",
                "Laptop gets IP 169.254.x.x when trying to connect in Library 2nd floor.",
                "Network", Models::Priority::HIGH, Models::Status::IN_PROGRESS,
                "john_doe", "agent_alice", "2026-11-01 10:30:00", "2026-11-01 11:15:00");

            tickets.emplace_back("TCK-102", "Lab 3 PC-14 Monitor flickering",
                "Screen turns black periodically during compilation tasks.",
                "Hardware", Models::Priority::MEDIUM, Models::Status::OPEN,
                "john_doe", "Unassigned", "2026-11-03 14:00:00", "2026-11-03 14:00:00");

            tickets.emplace_back("TCK-103", "Reset Student Portal Password",
                "Forgot password after semester break.",
                "Account", Models::Priority::LOW, Models::Status::RESOLVED,
                "john_doe", "agent_bob", "2026-11-04 09:00:00", "2026-11-04 09:20:00");
            save();
        }
    }

    std::vector<Models::Ticket> getAll() const {
        return tickets;
    }

    Models::Ticket* findById(const std::string& id) {
        for (auto& t : tickets) {
            if (t.getId() == id) {
                return &t;
            }
        }
        return nullptr;
    }

    bool addTicket(const Models::Ticket& ticket) {
        tickets.push_back(ticket);
        save();
        return true;
    }

    bool updateTicket(const Models::Ticket& updated) {
        for (auto& t : tickets) {
            if (t.getId() == updated.getId()) {
                t = updated;
                save();
                return true;
            }
        }
        return false;
    }

    bool deleteTicket(const std::string& id) {
        auto it = std::remove_if(tickets.begin(), tickets.end(), [&](const Models::Ticket& t) {
            return t.getId() == id;
        });
        if (it != tickets.end()) {
            tickets.erase(it, tickets.end());
            save();
            return true;
        }
        return false;
    }

    std::string generateNextId() {
        int maxId = 100;
        for (const auto& t : tickets) {
            if (t.getId().rfind("TCK-", 0) == 0) {
                try {
                    int val = std::stoi(t.getId().substr(4));
                    if (val > maxId) maxId = val;
                } catch (...) {}
            }
        }
        return "TCK-" + std::to_string(maxId + 1);
    }
};

} // namespace Repositories

