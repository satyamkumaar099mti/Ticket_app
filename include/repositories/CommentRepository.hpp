#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "../models/Comment.hpp"
#include "../utils/Utils.hpp"

namespace Repositories {

class CommentRepository {
private:
    std::string filePath;
    std::vector<Models::Comment> comments;

    void load() {
        comments.clear();
        std::ifstream file(filePath);
        if (!file.is_open()) return;

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            auto fields = Utils::parseCSVLine(line);
            if (fields.size() >= 6) {
                std::string id = fields[0];
                std::string ticketId = fields[1];
                std::string author = fields[2];
                std::string content = fields[3];
                std::string timestamp = fields[4];
                bool isInternal = (fields[5] == "1" || fields[5] == "true");

                comments.emplace_back(id, ticketId, author, content, timestamp, isInternal);
            }
        }
        file.close();
    }

    void save() {
        std::ofstream file(filePath, std::ios::trunc);
        if (!file.is_open()) return;

        for (const auto& c : comments) {
            file << Utils::escapeCSV(c.getId()) << ","
                 << Utils::escapeCSV(c.getTicketId()) << ","
                 << Utils::escapeCSV(c.getAuthorUsername()) << ","
                 << Utils::escapeCSV(c.getContent()) << ","
                 << Utils::escapeCSV(c.getTimestamp()) << ","
                 << (c.getIsInternal() ? "1" : "0") << "\n";
        }
        file.close();
    }

public:
    explicit CommentRepository(std::string path = "data_comments.csv")
        : filePath(std::move(path)) {
        load();
    }

    std::vector<Models::Comment> getByTicketId(const std::string& ticketId, bool includeInternal = false) const {
        std::vector<Models::Comment> result;
        for (const auto& c : comments) {
            if (c.getTicketId() == ticketId) {
                if (!c.getIsInternal() || includeInternal) {
                    result.push_back(c);
                }
            }
        }
        return result;
    }

    void addComment(const Models::Comment& comment) {
        comments.push_back(comment);
        save();
    }

    std::string generateNextId() {
        return "C-" + std::to_string(comments.size() + 101);
    }
};

} // namespace Repositories

