#pragma once
#include <string>
#include <vector>
#include "Comment.hpp"

namespace Models {

enum class Priority {
    LOW = 1,
    MEDIUM = 2,
    HIGH = 3,
    CRITICAL = 4
};

inline std::string priorityToString(Priority p) {
    switch (p) {
        case Priority::LOW:      return "LOW";
        case Priority::MEDIUM:   return "MEDIUM";
        case Priority::HIGH:     return "HIGH";
        case Priority::CRITICAL: return "CRITICAL";
        default:                 return "UNKNOWN";
    }
}

inline Priority priorityFromString(const std::string& str) {
    if (str == "LOW" || str == "1") return Priority::LOW;
    if (str == "MEDIUM" || str == "2") return Priority::MEDIUM;
    if (str == "HIGH" || str == "3") return Priority::HIGH;
    if (str == "CRITICAL" || str == "4") return Priority::CRITICAL;
    return Priority::MEDIUM;
}

enum class Status {
    OPEN = 1,
    IN_PROGRESS = 2,
    RESOLVED = 3,
    CLOSED = 4
};

inline std::string statusToString(Status s) {
    switch (s) {
        case Status::OPEN:        return "OPEN";
        case Status::IN_PROGRESS: return "IN_PROGRESS";
        case Status::RESOLVED:    return "RESOLVED";
        case Status::CLOSED:      return "CLOSED";
        default:                  return "UNKNOWN";
    }
}

inline Status statusFromString(const std::string& str) {
    if (str == "OPEN" || str == "1") return Status::OPEN;
    if (str == "IN_PROGRESS" || str == "2") return Status::IN_PROGRESS;
    if (str == "RESOLVED" || str == "3") return Status::RESOLVED;
    if (str == "CLOSED" || str == "4") return Status::CLOSED;
    return Status::OPEN;
}

class Ticket {
private:
    std::string id;
    std::string title;
    std::string description;
    std::string category;
    Priority priority;
    Status status;
    std::string creatorUsername;
    std::string assigneeUsername;
    std::string createdAt;
    std::string updatedAt;

public:
    Ticket()
        : priority(Priority::MEDIUM),
          status(Status::OPEN),
          assigneeUsername("Unassigned") {}

    Ticket(std::string id, std::string title, std::string description,
           std::string category, Priority priority, Status status,
           std::string creatorUsername, std::string assigneeUsername,
           std::string createdAt, std::string updatedAt)
        : id(std::move(id)),
          title(std::move(title)),
          description(std::move(description)),
          category(std::move(category)),
          priority(priority),
          status(status),
          creatorUsername(std::move(creatorUsername)),
          assigneeUsername(std::move(assigneeUsername)),
          createdAt(std::move(createdAt)),
          updatedAt(std::move(updatedAt)) {
        if (this->assigneeUsername.empty()) {
            this->assigneeUsername = "Unassigned";
        }
    }

    const std::string& getId() const { return id; }
    const std::string& getTitle() const { return title; }
    const std::string& getDescription() const { return description; }
    const std::string& getCategory() const { return category; }
    Priority getPriority() const { return priority; }
    Status getStatus() const { return status; }
    const std::string& getCreatorUsername() const { return creatorUsername; }
    const std::string& getAssigneeUsername() const { return assigneeUsername; }
    const std::string& getCreatedAt() const { return createdAt; }
    const std::string& getUpdatedAt() const { return updatedAt; }

    void setTitle(const std::string& t) { title = t; }
    void setDescription(const std::string& d) { description = d; }
    void setCategory(const std::string& c) { category = c; }
    void setPriority(Priority p) { priority = p; }
    void setStatus(Status s) { status = s; }
    void setAssigneeUsername(const std::string& a) { assigneeUsername = a; }
    void setUpdatedAt(const std::string& u) { updatedAt = u; }
};

} // namespace Models

