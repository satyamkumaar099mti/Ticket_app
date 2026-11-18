#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <vector>
#include <cctype>

namespace Utils {

inline std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t timeNow = std::chrono::system_clock::to_time_t(now);
    std::tm* tmPtr = std::localtime(&timeNow);
    char buf[64];
    if (tmPtr && std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tmPtr)) {
        return std::string(buf);
    }
    return "2026-11-06 00:00:00";
}

// Simple and deterministic hash for university demo purposes (no external crypto lib required)
inline std::string hashPassword(const std::string& input) {
    unsigned long hash = 5381;
    for (char c : input) {
        hash = ((hash << 5) + hash) + static_cast<unsigned char>(c); /* hash * 33 + c */
    }
    std::ostringstream oss;
    oss << std::hex << std::setw(16) << std::setfill('0') << hash;
    return oss.str();
}

inline std::string escapeCSV(const std::string& input) {
    std::string output = "\"";
    for (char c : input) {
        if (c == '"') {
            output += "\"\"";
        } else if (c == '\n') {
            output += "\\n";
        } else if (c == '\r') {
            // omit carriage return
        } else {
            output += c;
        }
    }
    output += "\"";
    return output;
}

inline std::string unescapeCSV(const std::string& input) {
    std::string res;
    for (size_t i = 0; i < input.size(); ++i) {
        if (input[i] == '\\' && i + 1 < input.size() && input[i + 1] == 'n') {
            res += '\n';
            ++i;
        } else {
            res += input[i];
        }
    }
    return res;
}

inline std::vector<std::string> parseCSVLine(const std::string& line) {
    std::vector<std::string> fields;
    std::string current;
    bool inQuotes = false;

    for (size_t i = 0; i < line.length(); ++i) {
        char c = line[i];
        if (c == '"') {
            if (inQuotes && i + 1 < line.length() && line[i + 1] == '"') {
                current += '"';
                ++i; // skip escaped quote
            } else {
                inQuotes = !inQuotes;
            }
        } else if (c == ',' && !inQuotes) {
            fields.push_back(unescapeCSV(current));
            current.clear();
        } else {
            current += c;
        }
    }
    fields.push_back(unescapeCSV(current));
    return fields;
}

inline std::string toLower(std::string s) {
    for (char &c : s) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    return s;
}

} // namespace Utils
